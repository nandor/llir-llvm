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
  setBooleanContents(ZeroOrOneBooleanContent);
  setHasFloatingPointExceptions(false);
  setSchedulingPreference(Sched::RegPressure);
  setMaxAtomicSizeInBitsSupported(64);

  addRegisterClass(MVT::i32, &GenM::I32RegClass);
  addRegisterClass(MVT::i64, &GenM::I64RegClass);
  computeRegisterProperties(Subtarget->getRegisterInfo());

  setOperationAction(ISD::FrameIndex, MVT::i64, Custom);
  setOperationAction(ISD::GlobalAddress, MVT::i64, Custom);
  setOperationAction(ISD::ExternalSymbol, MVT::i64, Custom);

  // Handle variable arguments.
  setOperationAction(ISD::VASTART, MVT::Other, Custom);
  setOperationAction(ISD::VAARG, MVT::Other, Expand);
  setOperationAction(ISD::VACOPY, MVT::Other, Expand);
  setOperationAction(ISD::VAEND, MVT::Other, Expand);

  // Expand conditional branches and selects.
  for (auto T : { MVT::i32, MVT::i64 }) {
    for (auto Op : {ISD::BR_CC, ISD::SELECT_CC}) {
      setOperationAction(Op, T, Expand);
    }
  }
}

SDValue GenMTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const
{
  switch (Op.getOpcode()) {
    case ISD::FrameIndex: return LowerFrameIndex(Op, DAG);
    case ISD::GlobalAddress: return LowerGlobalAddress(Op, DAG);
    case ISD::ExternalSymbol: return LowerExternalSymbol(Op, DAG);
    case ISD::VASTART: return LowerVASTART(Op, DAG);
    default: {
      Op.dump();
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

  return DAG.getTargetGlobalAddress(
      GA->getGlobal(),
      DL,
      Op.getValueType(),
      GA->getOffset()
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
  return DAG.getTargetExternalSymbol(ES->getSymbol(), VT);
}

SDValue GenMTargetLowering::LowerVASTART(SDValue Op, SelectionDAG &DAG) const
{
  SDLoc DL(Op);
  EVT PtrVT = getPointerTy(DAG.getMachineFunction().getDataLayout());

  auto *MFI = DAG.getMachineFunction().getInfo<GenMMachineFunctionInfo>();
  const Value *SV = cast<SrcValueSDNode>(Op.getOperand(2))->getValue();

  // Store the VAReg in the value.
  return DAG.getStore(
      Op.getOperand(0),
      DL,
      DAG.getCopyFromReg(
          DAG.getEntryNode(),
          DL,
          MFI->getVAReg(),
          PtrVT
      ),
      Op.getOperand(1),
      MachinePointerInfo(SV),
      0
  );
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
  case GenMISD::VOID:         return "GenMISD::VOID";
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
  llvm_unreachable("isOffsetFoldingLegal");
}

MVT GenMTargetLowering::getScalarShiftAmountTy(
    const DataLayout &DL,
    EVT VT) const
{
  switch (VT.getSizeInBits()) {
    case 32: return MVT::i32;
    case 64: return MVT::i64;
    default: llvm_unreachable("unable to represent shift amount");
  }
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

SDValue GenMTargetLowering::LowerFormalArguments(
    SDValue Chain,
    CallingConv::ID CallConv,
    bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins,
    const SDLoc &DL,
    SelectionDAG &DAG,
    SmallVectorImpl<SDValue> &InVals) const
{
  if (CallConv != CallingConv::C) {
    Fail(DL, DAG, "calling convention not supported");
  }

  MachineFunction &MF = DAG.getMachineFunction();
  auto *MFI = MF.getInfo<GenMMachineFunctionInfo>();

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

  if (IsVarArg) {
    MachineRegisterInfo &MRI = MF.getRegInfo();
    MVT PtrVT = getPointerTy(MF.getDataLayout());
    unsigned VAReg = MRI.createVirtualRegister(getRegClassFor(PtrVT));
    MFI->setVAReg(VAReg);
    Chain = DAG.getCopyToReg(
      Chain,
      DL,
      VAReg,
      DAG.getNode(
          GenMISD::ARGUMENT,
          DL,
          PtrVT,
          DAG.getTargetConstant(Ins.size(), DL, MVT::i32)
      )
    );
  }

  return Chain;
}

SDValue GenMTargetLowering::LowerCall(
    TargetLowering::CallLoweringInfo &CLI,
    SmallVectorImpl<SDValue> &InVals) const
{
  SelectionDAG &DAG = CLI.DAG;
  SDLoc DL = CLI.DL;
  MachineFunction &MF = DAG.getMachineFunction();
  const DataLayout &Layout = MF.getDataLayout();
  MVT PtrVT = getPointerTy(MF.getDataLayout());
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;

  if (CLI.CallConv != CallingConv::C) {
    Fail(DL, DAG, "calling convention not supported");
  }
  if (CLI.IsTailCall) {
    // TODO(nand): enable tail calls
    CLI.IsTailCall = false;
  }
  if (CLI.Ins.size() > 1) {
    Fail(DL, DAG, "more than 1 return value not supported");
  }

  // Argument to the call node.
  SmallVector<SDValue, 16> Ops;

  // Collect all fixed arguments.
  unsigned NumFixedArgs = 0;
  for (unsigned i = 0; i < CLI.Outs.size(); ++i) {
    const ISD::OutputArg &Out = CLI.Outs[i];
    NumFixedArgs += Out.IsFixed;
  }

  // Collect all variable arguments.
  if (CLI.IsVarArg) {
    auto FixedEnd = CLI.OutVals.begin() + NumFixedArgs;

    // Compute the number of bytes required on the stack to hold args.
    SmallVector<CCValAssign, 16> ArgLocs;
    CCState CCInfo(CLI.CallConv, true, MF, ArgLocs, *DAG.getContext());
    for (SDValue Arg : make_range(FixedEnd, CLI.OutVals.end())) {
      EVT VT = Arg.getValueType();
      Type *Ty = VT.getTypeForEVT(*DAG.getContext());
      unsigned Offset = CCInfo.AllocateStack(
          Layout.getTypeAllocSize(Ty),
          Layout.getABITypeAlignment(Ty)
      );
      CCInfo.addLoc(CCValAssign::getMem(
          ArgLocs.size(),
          VT.getSimpleVT(),
          Offset,
          VT.getSimpleVT(),
          CCValAssign::Full
      ));
    }

    // Allocate an object on the stack to hold args.
    SDValue FINode;
    SmallVector<SDValue, 8> Chains;
    if (unsigned NumBytes = CCInfo.getAlignedCallFrameSize()) {
      int FI = MF.getFrameInfo().CreateStackObject(
          NumBytes,
          Layout.getStackAlignment(),
          false
      );
      unsigned I = 0;
      for (SDValue Arg : make_range(FixedEnd, CLI.OutVals.end())) {
        FINode = DAG.getFrameIndex(FI, getPointerTy(Layout));

        unsigned Offset = ArgLocs[I++].getLocMemOffset();
        SDValue Add = DAG.getNode(
            ISD::ADD,
            DL,
            PtrVT,
            FINode,
            DAG.getConstant(Offset, DL, PtrVT)
        );
        Chains.push_back(DAG.getStore(
            Chain,
            DL,
            Arg,
            Add,
            MachinePointerInfo::getFixedStack(MF, FI, Offset), 0
        ));
      }
      if (!Chains.empty()) {
        Chain = DAG.getNode(ISD::TokenFactor, DL, MVT::Other, Chains);
      }
    } else {
      FINode = DAG.getIntPtrConstant(0, DL);
    }

    Ops.push_back(Chain);
    Ops.push_back(Callee);
    Ops.append(CLI.OutVals.begin(), FixedEnd);
    Ops.push_back(FINode);
  } else {
    Ops.push_back(Chain);
    Ops.push_back(Callee);
    Ops.append(CLI.OutVals.begin(), CLI.OutVals.end());
  }

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
  if (CallConv != CallingConv::C || Outs.size() > 1) {
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

