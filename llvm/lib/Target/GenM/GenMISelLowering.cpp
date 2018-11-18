//===-- GenMISelLowering.cpp - GenM DAG Lowering Implementation -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the interfaces that GenM uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#include "GenMISelLowering.h"
#include "GenMMachineFunctionInfo.h"
#include "GenMRegisterInfo.h"
#include "GenMTargetMachine.h"
#include "GenMTargetObjectFile.h"
#include "MCTargetDesc/GenMMCTargetDesc.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/KnownBits.h"
using namespace llvm;

static void Fail(const SDLoc &DL, SelectionDAG &DAG, const char *msg)
{
  DAG.getContext()->diagnose(DiagnosticInfoUnsupported(
      DAG.getMachineFunction().getFunction(),
      msg,
      DL.getDebugLoc()
  ));
}

GenMTargetLowering::GenMTargetLowering(
    const TargetMachine &TM,
    const GenMSubtarget &STI)
  : TargetLowering(TM)
  , Subtarget(&STI)
{
  auto MVTPtr = MVT::i64;

  setBooleanContents(ZeroOrOneBooleanContent);
  setHasFloatingPointExceptions(false);
  setSchedulingPreference(Sched::RegPressure);
  setMaxAtomicSizeInBitsSupported(64);

  addRegisterClass(MVT::i32, &GenM::I32RegClass);
  addRegisterClass(MVT::i64, &GenM::I64RegClass);
  addRegisterClass(MVT::f32, &GenM::F32RegClass);
  addRegisterClass(MVT::f64, &GenM::F64RegClass);
  computeRegisterProperties(Subtarget->getRegisterInfo());

  setOperationAction(ISD::FrameIndex, MVTPtr, Custom);
  setOperationAction(ISD::GlobalAddress, MVTPtr, Custom);
  setOperationAction(ISD::ExternalSymbol, MVTPtr, Custom);
  setOperationAction(ISD::JumpTable, MVTPtr, Custom);
  setOperationAction(ISD::BlockAddress, MVTPtr, Custom);
  setOperationAction(ISD::BRIND, MVT::Other, Custom);
  setOperationAction(ISD::BR_JT, MVT::Other, Custom);
  setOperationAction(ISD::CopyToReg, MVT::Other, Custom);

  // Handle variable arguments.
  setOperationAction(ISD::VASTART, MVT::Other, Custom);
  setOperationAction(ISD::VAARG, MVT::Other, Expand);
  setOperationAction(ISD::VACOPY, MVT::Other, Expand);
  setOperationAction(ISD::VAEND, MVT::Other, Expand);

  // Expand conditional branches and selects.
  for (auto T : { MVT::i32, MVT::i64, MVT::f32, MVT::f64 }) {
    for (auto Op : {ISD::BR_CC, ISD::SELECT_CC}) {
      setOperationAction(Op, T, Expand);
    }
  }

  // Preserve undefined values.
  for (auto T : { MVT::i32, MVT::i64, MVT::f32, MVT::f64 }) {
    setOperationAction(ISD::UNDEF, T, Legal);
  }

  // Deal with floating point operations.
  for (auto T : { MVT::f32, MVT::f64 }) {
    setOperationAction(ISD::ConstantFP, T, Legal);

    // Expand floating-point comparisons.
    for (auto CC : { ISD::SETO, ISD::SETUO }) {
      setCondCodeAction(CC, T, Expand);
    }
  }

  // Disable some integer operations.
  for (auto T : {MVT::i32, MVT::i64}) {
    // Expand unavailable integer operations.
    SmallVector<unsigned, 64> Ops{
        ISD::BSWAP, ISD::SMUL_LOHI, ISD::UMUL_LOHI, ISD::MULHS, ISD::MULHU,
        ISD::SDIVREM, ISD::UDIVREM, ISD::SHL_PARTS, ISD::SRA_PARTS,
        ISD::SRL_PARTS, ISD::ADDC, ISD::ADDE, ISD::SUBC, ISD::SUBE
    };

    for (auto Op : Ops) {
      setOperationAction(Op, T, Expand);
    }
  }

  // Disable in-register sign extension.
  for (auto T : { MVT::i8, MVT::i16, MVT::i32, MVT::i64 }) {
    setOperationAction(ISD::SIGN_EXTEND_INREG, T, Expand);
  }

  // Expand extending loads and stores.
  setLoadExtAction(ISD::EXTLOAD, MVT::f64, MVT::f32, Expand);
  setTruncStoreAction(MVT::f64, MVT::f32, Expand);
  for (auto T : MVT::integer_valuetypes()) {
    for (auto Ext : {ISD::EXTLOAD, ISD::ZEXTLOAD, ISD::SEXTLOAD}) {
      setLoadExtAction(Ext, T, MVT::i1, Promote);
    }
  }
  // Preserve traps since they terminate basic blocks.
  setOperationAction(ISD::TRAP, MVT::Other, Legal);

  // Disable jump tables.
  setMinimumJumpTableEntries(INT_MAX);
}

SDValue GenMTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const
{
  switch (Op.getOpcode()) {
    case ISD::FrameIndex:     return LowerFrameIndex(Op, DAG);
    case ISD::GlobalAddress:  return LowerGlobalAddress(Op, DAG);
    case ISD::ExternalSymbol: return LowerExternalSymbol(Op, DAG);
    case ISD::JumpTable:      return LowerJumpTable(Op, DAG);
    case ISD::BR_JT:          return LowerBR_JT(Op, DAG);
    case ISD::VASTART:        return LowerVASTART(Op, DAG);
    case ISD::CopyToReg:      return LowerCopyToReg(Op, DAG);
    default: {
      llvm_unreachable("unimplemented operation lowering");
      return SDValue();
    }
  }
}

SDValue GenMTargetLowering::LowerFrameIndex(SDValue Op, SelectionDAG &DAG) const
{
  const auto *FI = cast<FrameIndexSDNode>(Op);
  return DAG.getTargetFrameIndex(FI->getIndex(), Op.getValueType());
}

SDValue GenMTargetLowering::LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const
{
  SDLoc DL(Op);

  const auto *GA = cast<GlobalAddressSDNode>(Op);
  if (GA->getAddressSpace() != 0) {
    Fail(DL, DAG, "GenM only expects the 0 address space");
  }
  if (GA->getTargetFlags() != 0) {
    Fail(DL, DAG, "Unexpected target flags");
  }

  EVT VT = Op.getValueType();
  return DAG.getNode(
      GenMISD::SYMBOL,
      DL,
      VT,
      DAG.getTargetGlobalAddress(
          GA->getGlobal(),
          DL,
          VT,
          GA->getOffset()
      )
  );
}

SDValue GenMTargetLowering::LowerExternalSymbol(SDValue Op, SelectionDAG &DAG) const
{
  SDLoc DL(Op);
  const auto *ES = cast<ExternalSymbolSDNode>(Op);
  EVT VT = Op.getValueType();
  if (ES->getTargetFlags() != 0) {
    Fail(DL, DAG, "Unexpected target flags");
  }
  return DAG.getNode(
      GenMISD::SYMBOL,
      DL,
      VT,
      DAG.getTargetExternalSymbol(ES->getSymbol(), VT)
  );
}

SDValue GenMTargetLowering::LowerBR_JT(SDValue Op, SelectionDAG &DAG) const
{
  SDLoc DL(Op);
  SDValue Chain = Op.getOperand(0);
  SDValue JT = Op.getOperand(1);
  SDValue Index = Op.getOperand(2);

  return DAG.getNode(
      GenMISD::BR_JT,
      DL,
      MVT::Other,
      Chain,
      Index,
      JT
  );
}

SDValue GenMTargetLowering::LowerJumpTable(SDValue Op, SelectionDAG &DAG) const
{
  const JumpTableSDNode *JT = cast<JumpTableSDNode>(Op);
  return DAG.getTargetJumpTable(
      JT->getIndex(),
      Op.getValueType(),
      JT->getTargetFlags()
  );
}

SDValue GenMTargetLowering::LowerVASTART(SDValue Op, SelectionDAG &DAG) const
{
  SDLoc DL(Op);
  MachineFunction *MF = &DAG.getMachineFunction();
  EVT PtrVT = getPointerTy(MF->getDataLayout());

  const Value *SV = cast<SrcValueSDNode>(Op.getOperand(2))->getValue();
  unsigned Reg = MF->addLiveIn(GenM::VA, &GenM::I64RegClass);

  // Store the VAReg in the value.
  return DAG.getStore(
      Op.getOperand(0),
      DL,
      DAG.getCopyFromReg(
          DAG.getEntryNode(),
          DL,
          Reg,
          PtrVT
      ),
      Op.getOperand(1),
      MachinePointerInfo(SV),
      0
  );
}

SDValue GenMTargetLowering::LowerCopyToReg(SDValue Op, SelectionDAG &DAG) const
{
  SDValue Src = Op.getOperand(2);
  if (isa<FrameIndexSDNode>(Src.getNode())) {
    SDValue Chain = Op.getOperand(0);
    SDLoc DL(Op);

    EVT VT = Src.getValueType();
    SDValue Copy(DAG.getMachineNode(
        VT == MVT::i32 ? GenM::COPY_I32 : GenM::COPY_I64,
        DL,
        VT,
        Src
    ), 0);

    unsigned Reg = cast<RegisterSDNode>(Op.getOperand(1))->getReg();
    if (Op.getNode()->getNumValues() == 1) {
      return DAG.getCopyToReg(Chain, DL, Reg, Copy);
    } else {
      return DAG.getCopyToReg(
          Chain,
          DL,
          Reg,
          Copy,
          Op.getNumOperands() == 4 ? Op.getOperand(3) : SDValue()
      );
    }
  }
  return SDValue();
}

bool GenMTargetLowering::useSoftFloat() const
{
  return false;
}

MachineBasicBlock *GenMTargetLowering::EmitInstrWithCustomInserter(
    MachineInstr &MI,
    MachineBasicBlock *MBB) const
{
  llvm_unreachable("EmitInstrWithCustomInserter");
}

const char *GenMTargetLowering::getTargetNodeName(unsigned Opcode) const
{
  switch (static_cast<GenMISD::NodeType>(Opcode)) {
  case GenMISD::FIRST_NUMBER: return nullptr;
  case GenMISD::RETURN:       return "GenMISD::RETURN";
  case GenMISD::ARGUMENT:     return "GenMISD::ARGUMENT";
  case GenMISD::CALL:         return "GenMISD::CALL";
  case GenMISD::TCALL:        return "GenMISD::TCALL";
  case GenMISD::VOID:         return "GenMISD::VOID";
  case GenMISD::SYMBOL:       return "GenMISD::SYMBOL";
  case GenMISD::BR_JT:        return "GenMISD::BR_JT";
  }
}

std::pair<unsigned, const TargetRegisterClass *>
GenMTargetLowering::getRegForInlineAsmConstraint(
    const TargetRegisterInfo *TRI,
    StringRef Constraint,
    MVT VT) const
{
  llvm_unreachable("getRegForInlineAsmConstraint");
}

bool GenMTargetLowering::isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const
{
  return true;
}

MVT GenMTargetLowering::getScalarShiftAmountTy(
    const DataLayout &DL,
    EVT VT) const
{
  return VT.getSizeInBits() <= 32 ? MVT::i32 : MVT::i64;
}

EVT GenMTargetLowering::getSetCCResultType(
    const DataLayout &DL,
    LLVMContext &Context,
    EVT VT) const
{
  if (!VT.isVector()) {
    return MVT::i32;
  } else {
    llvm_unreachable("getSetCCResultType");
  }
}

static bool isCallingConvSupported(CallingConv::ID ID)
{
  switch (ID) {
    case CallingConv::C:    return true;
    case CallingConv::Fast: return true;
    default: return false;
  }
}

SDValue GenMTargetLowering::LowerFormalArguments(
    SDValue Chain,
    CallingConv::ID CallConv,
    bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins,
    const SDLoc &DL,
    SelectionDAG &DAG,
    SmallVectorImpl<SDValue> &InVals) const
{
  MachineFunction &MF = DAG.getMachineFunction();
  auto *MFI = MF.getInfo<GenMMachineFunctionInfo>();

  if (!isCallingConvSupported(CallConv)) {
    Fail(DL, DAG, "unsupported calling convention");
  }

  for (const auto &In : Ins) {
    // TODO(nand): check for argument types.
    InVals.push_back(DAG.getNode(
        GenMISD::ARGUMENT,
        DL,
        In.VT,
        DAG.getTargetConstant(InVals.size(), DL, MVT::i32)
    ));
    MFI->addParam(In.VT);
  }

  MFI->setNumFixedArgs(Ins.size());

  return Chain;
}

SDValue GenMTargetLowering::LowerCall(
    TargetLowering::CallLoweringInfo &CLI,
    SmallVectorImpl<SDValue> &InVals) const
{
  SelectionDAG &DAG = CLI.DAG;
  SDLoc DL = CLI.DL;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  CallingConv::ID CallConv = CLI.CallConv;

  MachineFunction &MF = DAG.getMachineFunction();
  const auto *CI = dyn_cast_or_null<CallInst>(CLI.CS.getInstruction());
  const Function *Fn = CI ? CI->getCalledFunction() : nullptr;
  const Module *M = MF.getMMI().getModule();
  Metadata *IsCFProtectionSupported = M->getModuleFlag("cf-protection-branch");

  if (CLI.Ins.size() > 1) {
    Fail(DL, DAG, "more than 1 return value not supported");
  }

  bool HasCINSCR = CI && CI->hasFnAttr("no_caller_saved_registers");
  bool HasFnNSCR = Fn && Fn->hasFnAttribute("no_caller_saved_registers");
  if (HasCINSCR || HasFnNSCR) {
    Fail(DL, DAG, "unsupported no_caller_saved_registers");
  }

  if (!isCallingConvSupported(CallConv)) {
    Fail(DL, DAG, "unsupported calling convention");
  }

  if (IsCFProtectionSupported) {
    Fail(DL, DAG, "CF protection not supported");
  }

  // Argument to the call node.
  SmallVector<SDValue, 16> Ops;

  // Collect all fixed arguments.
  unsigned NumFixedArgs = 0;
  for (unsigned i = 0; i < CLI.Outs.size(); ++i) {
    const ISD::OutputArg &Out = CLI.Outs[i];
    NumFixedArgs += Out.IsFixed;
  }

  // Add all arguments to the call.
  Ops.push_back(Chain);
  Ops.push_back(Callee);
  Ops.push_back(DAG.getTargetConstant(static_cast<int>(CallConv), DL, MVT::i32));
  if (CLI.IsVarArg) {
    Ops.push_back(DAG.getTargetConstant(NumFixedArgs, DL, MVT::i32));
  }
  Ops.append(CLI.OutVals.begin(), CLI.OutVals.end());

  if (CLI.IsTailCall) {
    SmallVector<EVT, 8> InTys = { MVT::Other };
    return DAG.getNode(GenMISD::TCALL, DL, DAG.getVTList(InTys), Ops);
  } else {
    // Collect the types of return values.
    SmallVector<EVT, 8> InTys;
    for (const auto &In : CLI.Ins) {
      // TODO(nand): analyse argument types.
      InTys.push_back(In.VT);
    }
    InTys.push_back(MVT::Other);

    // Construct the call node.
    if (CLI.Ins.empty()) {
      return DAG.getNode(GenMISD::VOID, DL, DAG.getVTList(InTys), Ops);
    } else {
      SDValue Call = DAG.getNode(
          GenMISD::CALL,
          DL,
          DAG.getVTList(InTys),
          Ops
      );
      InVals.push_back(Call);
      return Call.getValue(1);
    }
  }
}

bool GenMTargetLowering::CanLowerReturn(
    CallingConv::ID CallConv,
    MachineFunction &MF,
    bool isVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    LLVMContext &Context) const
{
  return Outs.size() <= 1;
}

SDValue GenMTargetLowering::LowerReturn(
    SDValue Chain,
    CallingConv::ID CallConv,
    bool isVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    const SmallVectorImpl<SDValue> &OutVals,
    const SDLoc &DL,
    SelectionDAG &DAG) const
{
  if (Outs.size() > 1) {
    Fail(DL, DAG, "calling convention not supported");
  }

  SmallVector<SDValue, 4> RetOps(1, Chain);
  RetOps.append(OutVals.begin(), OutVals.end());
  Chain = DAG.getNode(GenMISD::RETURN, DL, MVT::Other, RetOps);

  return Chain;
}

GenMTargetLowering::AtomicExpansionKind
GenMTargetLowering::shouldExpandAtomicRMWInIR(AtomicRMWInst *AI) const
{
  llvm_unreachable("not implemented");
}

