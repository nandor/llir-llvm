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
#include "llvm/CodeGen/MachineJumpTableInfo.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Intrinsics.h"
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
  addRegisterClass(MVT::f80, &GenM::F80RegClass);
  computeRegisterProperties(Subtarget->getRegisterInfo());

  // Custom lowerings for most operations.
  setOperationAction(ISD::FRAMEADDR, MVTPtr, Custom);
  setOperationAction(ISD::FrameIndex, MVTPtr, Custom);
  setOperationAction(ISD::GlobalAddress, MVTPtr, Custom);
  setOperationAction(ISD::ExternalSymbol, MVTPtr, Custom);
  setOperationAction(ISD::JumpTable, MVTPtr, Custom);
  setOperationAction(ISD::BlockAddress, MVTPtr, Custom);
  setOperationAction(ISD::BRIND, MVT::Other, Custom);
  setOperationAction(ISD::BR_JT, MVT::Other, Custom);
  setOperationAction(ISD::CopyToReg, MVT::Other, Custom);
  setOperationAction(ISD::BR_JT, MVT::Other, Custom);

  // Handle alloca.
  setOperationAction(ISD::STACKSAVE, MVT::Other, Expand);
  setOperationAction(ISD::STACKRESTORE, MVT::Other, Expand);
  setOperationAction(ISD::DYNAMIC_STACKALLOC, MVTPtr, Custom);
  setStackPointerRegisterToSaveRestore(GenM::RSP);

  // Handle variable arguments.
  setOperationAction(ISD::VASTART, MVT::Other, Custom);
  setOperationAction(ISD::VAARG, MVT::Other, Custom);
  setOperationAction(ISD::VACOPY, MVT::Other, Custom);
  setOperationAction(ISD::VAEND, MVT::Other, Expand);

  // Expand conditional branches and selects.
  for (auto T : { MVT::i32, MVT::i64, MVT::f32, MVT::f64, MVT::f80 }) {
    for (auto Op : {ISD::BR_CC, ISD::SELECT_CC}) {
      setOperationAction(Op, T, Expand);
    }
  }

  // Deal with floating point operations.
  for (auto T : { MVT::f32, MVT::f64, MVT::f80 }) {
    setOperationAction(ISD::ConstantFP, T, Legal);
    setOperationAction(ISD::FEXP,   T, Legal);
    setOperationAction(ISD::FEXP2,  T, Legal);
    setOperationAction(ISD::FLOG,   T, Legal);
    setOperationAction(ISD::FLOG2,  T, Legal);
    setOperationAction(ISD::FLOG10, T, Legal);

    // Expand floating-point comparisons.
    for (auto CC : { ISD::SETO, ISD::SETUO }) {
      setCondCodeAction(CC, T, Expand);
    }

    // Expand operations which are not yet supported.
    setOperationAction(ISD::FMA, T, Expand);
  }

  // Disable some integer operations.
  for (auto T : {MVT::i32, MVT::i64}) {
    // Custom lowering for some actions.
    setOperationAction(ISD::SADDO, T, Custom);
    setOperationAction(ISD::UADDO, T, Custom);
    setOperationAction(ISD::SSUBO, T, Custom);
    setOperationAction(ISD::USUBO, T, Custom);
    setOperationAction(ISD::SMULO, T, Custom);
    setOperationAction(ISD::UMULO, T, Custom);

    // Expand unavailable integer operations.
    setOperationAction(ISD::BSWAP, T, Expand);
    setOperationAction(ISD::SMUL_LOHI, T, Expand);
    setOperationAction(ISD::UMUL_LOHI, T, Expand);
    setOperationAction(ISD::MULHS, T, Expand);
    setOperationAction(ISD::MULHU, T, Expand);
    setOperationAction(ISD::SDIVREM, T, Expand);
    setOperationAction(ISD::UDIVREM, T, Expand);
    setOperationAction(ISD::SHL_PARTS, T, Expand);
    setOperationAction(ISD::SRA_PARTS, T, Expand);
    setOperationAction(ISD::SRL_PARTS, T, Expand);
    setOperationAction(ISD::ADDC, T, Expand);
    setOperationAction(ISD::ADDE, T, Expand);
    setOperationAction(ISD::SUBC, T, Expand);
    setOperationAction(ISD::SUBE, T, Expand);
  }

  // Disable in-register sign extension.
  for (auto T : { MVT::i1, MVT::i8, MVT::i16, MVT::i32, MVT::i64 }) {
    setOperationAction(ISD::SIGN_EXTEND_INREG, T, Expand);
  }

  // Expand extending loads and stores.
  setLoadExtAction(ISD::EXTLOAD, MVT::f64, MVT::f32, Expand);
  setLoadExtAction(ISD::EXTLOAD, MVT::f80, MVT::f32, Expand);
  setLoadExtAction(ISD::EXTLOAD, MVT::f80, MVT::f64, Expand);
  setTruncStoreAction(MVT::f64, MVT::f32, Expand);
  setTruncStoreAction(MVT::f80, MVT::f32, Expand);
  setTruncStoreAction(MVT::f80, MVT::f64, Expand);

  // Disable extending from bits.
  for (auto T : MVT::integer_valuetypes()) {
    for (auto Ext : {ISD::EXTLOAD, ISD::ZEXTLOAD, ISD::SEXTLOAD}) {
      setLoadExtAction(Ext, T, MVT::i1, Promote);
    }
  }

  // Preserve traps since they terminate basic blocks.
  setOperationAction(ISD::TRAP, MVT::Other, Legal);

  // Custom lowering for intrinsics.
  setOperationAction(ISD::INTRINSIC_WO_CHAIN, MVT::Other, Custom);
  setOperationAction(ISD::INTRINSIC_W_CHAIN, MVT::Other, Custom);
  setOperationAction(ISD::INTRINSIC_VOID, MVT::Other, Custom);
}

SDValue GenMTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const
{
  switch (Op.getOpcode()) {
    case ISD::FRAMEADDR:          return LowerFRAMEADDR(Op, DAG);
    case ISD::FrameIndex:         return LowerFrameIndex(Op, DAG);
    case ISD::GlobalAddress:      return LowerGlobalAddress(Op, DAG);
    case ISD::ExternalSymbol:     return LowerExternalSymbol(Op, DAG);
    case ISD::JumpTable:          return LowerJumpTable(Op, DAG);
    case ISD::BR_JT:              return LowerBR_JT(Op, DAG);
    case ISD::DYNAMIC_STACKALLOC: return LowerDynamicStackalloc(Op, DAG);
    case ISD::VASTART:            return LowerVASTART(Op, DAG);
    case ISD::VAARG:              return LowerVAARG(Op, DAG);
    case ISD::VACOPY:             return LowerVACOPY(Op, DAG);
    case ISD::CopyToReg:          return LowerCopyToReg(Op, DAG);
    case ISD::SADDO:              return LowerALUO(Op, DAG);
    case ISD::UADDO:              return LowerALUO(Op, DAG);
    case ISD::SSUBO:              return LowerALUO(Op, DAG);
    case ISD::USUBO:              return LowerALUO(Op, DAG);
    case ISD::SMULO:              return LowerALUO(Op, DAG);
    case ISD::UMULO:              return LowerALUO(Op, DAG);
    case ISD::INTRINSIC_WO_CHAIN: return LowerINTRINSIC_WO_CHAIN(Op, DAG);
    case ISD::INTRINSIC_W_CHAIN:  return LowerINTRINSIC_W_CHAIN(Op, DAG);
    case ISD::INTRINSIC_VOID:     return LowerINTRINSIC_VOID(Op, DAG);
    default: {
      llvm_unreachable("unimplemented operation lowering");
      return SDValue();
    }
  }
}

SDValue GenMTargetLowering::LowerFRAMEADDR(SDValue Op, SelectionDAG &DAG) const
{
  return DAG.getCopyFromReg(
      Op.getOperand(0),
      SDLoc(Op),
      GenM::FRAME_ADDR,
      MVT::i64
  );
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

  const auto *JT = cast<JumpTableSDNode>(Op.getOperand(1));
  SDValue Index = Op.getOperand(2);

  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Index);

  // Add a BB operand for each case.
  MachineJumpTableInfo *MJTI = DAG.getMachineFunction().getJumpTableInfo();
  for (auto MBB : MJTI->getJumpTables()[JT->getIndex()].MBBs) {
    Ops.push_back(DAG.getBasicBlock(MBB));
  }

  // Create a custom node.
  return DAG.getNode(GenMISD::SWITCH, DL, MVT::Other, Ops);
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

SDValue GenMTargetLowering::LowerDynamicStackalloc(SDValue Op, SelectionDAG &DAG) const
{
  SDVTList VTs = DAG.getVTList(MVT::i64, MVT::Other);
  return DAG.getNode(
      GenMISD::ALLOCA,
      SDLoc(Op),
      VTs,
      Op.getOperand(0),
      Op.getOperand(1),
      Op.getOperand(2)
  );
}

SDValue GenMTargetLowering::LowerVASTART(SDValue Op, SelectionDAG &DAG) const
{
  return DAG.getNode(
      GenMISD::VASTART,
      SDLoc(Op),
      MVT::Other,
      Op.getOperand(0),
      Op.getOperand(1)
  );
}

SDValue GenMTargetLowering::LowerVAARG(SDValue Op, SelectionDAG &DAG) const
{
  llvm_unreachable("not implemented");
}

SDValue GenMTargetLowering::LowerVACOPY(SDValue Op, SelectionDAG &DAG) const
{
  // See the X86ISelLowering implementation
  SDValue Chain = Op.getOperand(0);
  SDValue DstPtr = Op.getOperand(1);
  SDValue SrcPtr = Op.getOperand(2);
  const Value *DstSV = cast<SrcValueSDNode>(Op.getOperand(3))->getValue();
  const Value *SrcSV = cast<SrcValueSDNode>(Op.getOperand(4))->getValue();
  SDLoc DL(Op);

  return DAG.getMemcpy(Chain, DL, DstPtr, SrcPtr,
                       DAG.getIntPtrConstant(24, DL), 8, /*isVolatile*/false,
                       false, false,
                       MachinePointerInfo(DstSV), MachinePointerInfo(SrcSV));
}

SDValue GenMTargetLowering::LowerCopyToReg(SDValue Op, SelectionDAG &DAG) const
{
  SDValue Src = Op.getOperand(2);
  if (isa<FrameIndexSDNode>(Src.getNode())) {
    SDValue Chain = Op.getOperand(0);
    SDLoc DL(Op);

    EVT VT = Src.getValueType();
    SDValue Copy(DAG.getMachineNode(
        VT == MVT::i32 ? GenM::MOV_I32 : GenM::MOV_I64,
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

SDValue GenMTargetLowering::LowerALUO(SDValue Op, SelectionDAG &DAG) const
{
  SDLoc DL(Op);
  SDNode *N = Op.getNode();
  SDValue LHS = N->getOperand(0);
  SDValue RHS = N->getOperand(1);

  unsigned vop, op;
  switch (Op.getOpcode()) {
    case ISD::SADDO: vop = ISD::ADD; op = GenMISD::SADDO; break;
    case ISD::UADDO: vop = ISD::ADD; op = GenMISD::UADDO; break;
    case ISD::SSUBO: vop = ISD::SUB; op = GenMISD::SSUBO; break;
    case ISD::USUBO: vop = ISD::SUB; op = GenMISD::USUBO; break;
    case ISD::SMULO: vop = ISD::MUL; op = GenMISD::SMULO; break;
    case ISD::UMULO: vop = ISD::MUL; op = GenMISD::UMULO; break;
  }

  SDValue Result = DAG.getNode(vop, DL, N->getValueType(0), LHS, RHS);
  SDValue Flag = DAG.getNode(op, DL, N->getValueType(1), LHS, RHS);
  return DAG.getNode(ISD::MERGE_VALUES, DL, N->getVTList(), Result, Flag);
}

SDValue GenMTargetLowering::LowerINTRINSIC_WO_CHAIN(SDValue Op, SelectionDAG &DAG) const {
  llvm_unreachable("invalid intrinsic");
}

SDValue GenMTargetLowering::LowerINTRINSIC_W_CHAIN(SDValue Op, SelectionDAG &DAG) const {
  switch (cast<ConstantSDNode>(Op.getOperand(1))->getZExtValue()) {
  case llvm::Intrinsic::x86_rdtsc: {
    return DAG.getNode(
        GenMISD::RDTSC,
        SDLoc(Op),
        DAG.getVTList(MVT::i64, MVT::Other),
        Op.getOperand(0)
    );
  }
  }
  llvm_unreachable("invalid intrinsic");
}

SDValue GenMTargetLowering::LowerINTRINSIC_VOID(SDValue Op, SelectionDAG &DAG) const {
  llvm_unreachable("invalid intrinsic");
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
  case GenMISD::TVOID:        return "GenMISD::TVOID";
  case GenMISD::SYMBOL:       return "GenMISD::SYMBOL";
  case GenMISD::SWITCH:       return "GenMISD::SWITCH";
  case GenMISD::VASTART:      return "GenMISD::VASTART";
  case GenMISD::SADDO:        return "GenMISD::SADDO";
  case GenMISD::UADDO:        return "GenMISD::UADDO";
  case GenMISD::SSUBO:        return "GenMISD::SSUBO";
  case GenMISD::USUBO:        return "GenMISD::USUBO";
  case GenMISD::SMULO:        return "GenMISD::SMULO";
  case GenMISD::UMULO:        return "GenMISD::UMULO";
  case GenMISD::ALLOCA:       return "GenMISD::ALLOCA";
  case GenMISD::RDTSC:        return "GenMISD::RDTSC";
  }
  llvm_unreachable("invalid opcode");
}

std::pair<unsigned, const TargetRegisterClass *>
GenMTargetLowering::getRegForInlineAsmConstraint(
    const TargetRegisterInfo *TRI,
    StringRef Constraint,
    MVT VT) const
{
  if (!Constraint.empty()) {
    llvm_unreachable("not implemented");
  }
  return TargetLowering::getRegForInlineAsmConstraint(TRI, Constraint, VT);
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

  MF.getRegInfo().addLiveIn(GenM::ARGUMENTS);

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

  // Collect the types of return values.
  SmallVector<EVT, 8> InTys;
  for (const auto &In : CLI.Ins) {
    // TODO(nand): analyse argument types.
    InTys.push_back(In.VT);
  }
  InTys.push_back(MVT::Other);

  // Construct the call node.
  if (CLI.Ins.empty()) {
    return DAG.getNode(
        CLI.IsTailCall ? GenMISD::TVOID : GenMISD::VOID,
        DL,
        DAG.getVTList(InTys),
        Ops
    );
  } else {
    if (CLI.IsTailCall) {
      return DAG.getNode(
          GenMISD::TCALL,
          DL,
          DAG.getVTList(InTys),
          Ops
      ).getValue(1);
    } else {
      SDValue Call = DAG.getNode(
          CLI.IsTailCall ? GenMISD::TCALL : GenMISD::CALL,
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

bool GenMTargetLowering::mayBeEmittedAsTailCall(const CallInst *CI) const {
  // Make sure tail calls aren't disabled.
  auto Attr = CI->getParent()->getParent()->getFnAttribute("disable-tail-calls");
  if (!CI->isTailCall() || Attr.getValueAsString() == "true") {
    return false;
  }

  ImmutableCallSite CS(CI);
  switch (CS.getCallingConv()) {
    // C calling conventions:
    case CallingConv::C:
    case CallingConv::Win64:
    case CallingConv::X86_64_SysV:
      return true;
    // Callee pop conventions:
    case CallingConv::X86_ThisCall:
    case CallingConv::X86_StdCall:
    case CallingConv::X86_VectorCall:
    case CallingConv::X86_FastCall:
      return true;
    // Guarantee TCO.
    case CallingConv::Fast:
    case CallingConv::GHC:
    case CallingConv::X86_RegCall:
    case CallingConv::HiPE:
    case CallingConv::HHVM:
      return true;
    // Don't tail call otherwise.
    default:
      return false;
  }
}
