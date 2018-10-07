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

  setOperationAction(ISD::FrameIndex, MVT::i64, Custom);

  computeRegisterProperties(Subtarget->getRegisterInfo());

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
    case ISD::FrameIndex: {
      int FI = cast<FrameIndexSDNode>(Op)->getIndex();
      return DAG.getTargetFrameIndex(FI, Op.getValueType());
    }
    default: {
      llvm_unreachable("unimplemented operation lowering");
      return SDValue();
    }
  }
}

bool GenMTargetLowering::useSoftFloat() const
{
  return false;
}

MachineBasicBlock *GenMTargetLowering::EmitInstrWithCustomInserter(
    MachineInstr &MI,
    MachineBasicBlock *MBB) const
{
  llvm_unreachable("not implemented");
}

const char *GenMTargetLowering::getTargetNodeName(unsigned Opcode) const
{
  switch (static_cast<GenMISD::NodeType>(Opcode)) {
  case GenMISD::FIRST_NUMBER: return nullptr;
  case GenMISD::RETURN:       return "GenMISD::RETURN";
  case GenMISD::ARGUMENT:     return "GenMISD::ARGUMENT";
  }
}

GenMTargetLowering::ConstraintType
GenMTargetLowering::getConstraintType(StringRef Constraint) const
{
  llvm_unreachable("not implemented");
}

GenMTargetLowering::ConstraintWeight
GenMTargetLowering::getSingleConstraintMatchWeight(
    AsmOperandInfo &info,
    const char *constraint) const
{
  llvm_unreachable("not implemented");
}

void GenMTargetLowering::LowerAsmOperandForConstraint(
    SDValue Op,
    std::string &Constraint,
    std::vector<SDValue> &Ops,
    SelectionDAG &DAG) const
{
  llvm_unreachable("not implemented");
}

unsigned
GenMTargetLowering::getInlineAsmMemConstraint(StringRef ConstraintCode) const
{
  llvm_unreachable("not implemented");
}

std::pair<unsigned, const TargetRegisterClass *>
GenMTargetLowering::getRegForInlineAsmConstraint(
    const TargetRegisterInfo *TRI,
    StringRef Constraint,
    MVT VT) const
{
  llvm_unreachable("not implemented");
}

bool GenMTargetLowering::isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const
{
  llvm_unreachable("not implemented");
}

unsigned GenMTargetLowering::getRegisterByName(
    const char* RegName,
    EVT VT,
    SelectionDAG &DAG) const
{
  llvm_unreachable("not implemented");
}

unsigned GenMTargetLowering::getExceptionPointerRegister(
    const Constant *PersonalityFn) const
{
  llvm_unreachable("not implemented");
}

unsigned GenMTargetLowering::getExceptionSelectorRegister(
    const Constant *PersonalityFn) const
{
  llvm_unreachable("not implemented");
}

bool GenMTargetLowering::useLoadStackGuardNode() const
{
  llvm_unreachable("not implemented");
}

void GenMTargetLowering::insertSSPDeclarations(Module &M) const
{
  llvm_unreachable("not implemented");
}

EVT GenMTargetLowering::getSetCCResultType(
    const DataLayout &DL,
    LLVMContext &Context,
    EVT VT) const
{
  if (!VT.isVector()) {
    return MVT::i32;
  } else {
    llvm_unreachable("not implemented");
  }
}

SDValue GenMTargetLowering::LowerFormalArguments(
    SDValue Chain,
    CallingConv::ID CallConv,
    bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins,
    const SDLoc &DL,
    SelectionDAG &DAG,
    SmallVectorImpl<SDValue> &InVals) const
{
  if (CallConv != CallingConv::C || isVarArg) {
    Fail(DL, DAG, "calling convention not supported");
  }

  MachineFunction &MF = DAG.getMachineFunction();
  auto *MFI = MF.getInfo<GenMMachineFunctionInfo>();
  MF.getRegInfo().addLiveIn(GenM::ARGS);

  for (const auto &In : Ins) {
    InVals.push_back(DAG.getNode(
        GenMISD::ARGUMENT, DL, In.VT,
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
  llvm_unreachable("not implemented");
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
  if (CallConv != CallingConv::C|| Outs.size() > 1) {
    Fail(DL, DAG, "calling convention not supported");
  }
  if (isVarArg) {
    Fail(DL, DAG, "vararg calls not supported");
  }

  SmallVector<SDValue, 4> RetOps(1, Chain);
  RetOps.append(OutVals.begin(), OutVals.end());
  Chain = DAG.getNode(GenMISD::RETURN, DL, MVT::Other, RetOps);
  return Chain;
}

bool GenMTargetLowering::ShouldShrinkFPConstant(EVT VT) const
{
  llvm_unreachable("not implemented");
}

bool GenMTargetLowering::shouldInsertFencesForAtomic(const Instruction *I) const
{
  llvm_unreachable("not implemented");
}

GenMTargetLowering::AtomicExpansionKind
GenMTargetLowering::shouldExpandAtomicRMWInIR(AtomicRMWInst *AI) const
{
  llvm_unreachable("not implemented");
}

void GenMTargetLowering::ReplaceNodeResults(
    SDNode *N,
    SmallVectorImpl<SDValue>& Results,
    SelectionDAG &DAG) const
{
  llvm_unreachable("not implemented");
}
