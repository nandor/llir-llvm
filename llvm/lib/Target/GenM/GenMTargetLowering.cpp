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

#include "GenMTargetLowering.h"
#include "GenMMachineFunctionInfo.h"
#include "GenMRegisterInfo.h"
#include "GenMTargetMachine.h"
#include "GenMTargetObjectFile.h"
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
{
}

SDValue GenMTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const
{
  assert(!"not implemented");
}

bool GenMTargetLowering::useSoftFloat() const
{
  assert(!"not implemented");
}

void GenMTargetLowering::computeKnownBitsForTargetNode(
   const SDValue Op,
   KnownBits &Known,
   const APInt &DemandedElts,
   const SelectionDAG &DAG,
   unsigned Depth) const
{
  assert(!"not implemented");
}

MachineBasicBlock *GenMTargetLowering::EmitInstrWithCustomInserter(
    MachineInstr &MI,
    MachineBasicBlock *MBB) const
{
  assert(!"not implemented");
}

const char *GenMTargetLowering::getTargetNodeName(unsigned Opcode) const
{
  assert(!"not implemented");
}

GenMTargetLowering::ConstraintType
GenMTargetLowering::getConstraintType(StringRef Constraint) const
{
  assert(!"not implemented");
}

GenMTargetLowering::ConstraintWeight
GenMTargetLowering::getSingleConstraintMatchWeight(
    AsmOperandInfo &info,
    const char *constraint) const
{
  assert(!"not implemented");
}

void GenMTargetLowering::LowerAsmOperandForConstraint(
    SDValue Op,
    std::string &Constraint,
    std::vector<SDValue> &Ops,
    SelectionDAG &DAG) const
{
  assert(!"not implemented");
}

unsigned
GenMTargetLowering::getInlineAsmMemConstraint(StringRef ConstraintCode) const
{
  assert(!"not implemented");
}

std::pair<unsigned, const TargetRegisterClass *>
GenMTargetLowering::getRegForInlineAsmConstraint(
    const TargetRegisterInfo *TRI,
    StringRef Constraint,
    MVT VT) const
{
  assert(!"not implemented");
}

bool GenMTargetLowering::isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const
{
  assert(!"not implemented");
}

unsigned GenMTargetLowering::getRegisterByName(
    const char* RegName,
    EVT VT,
    SelectionDAG &DAG) const
{
  assert(!"not implemented");
}

unsigned GenMTargetLowering::getExceptionPointerRegister(
    const Constant *PersonalityFn) const
{
  assert(!"not implemented");
}

unsigned GenMTargetLowering::getExceptionSelectorRegister(
    const Constant *PersonalityFn) const
{
  assert(!"not implemented");
}

bool GenMTargetLowering::useLoadStackGuardNode() const
{
  assert(!"not implemented");
}

void GenMTargetLowering::insertSSPDeclarations(Module &M) const
{
  assert(!"not implemented");
}

EVT GenMTargetLowering::getSetCCResultType(
    const DataLayout &DL,
    LLVMContext &Context,
    EVT VT) const
{
  assert(!"not implemented");
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
  if (CallConv != CallingConv::C) {
    Fail(DL, DAG, "calling convention not supported");
  }

  MachineFunction &MF = DAG.getMachineFunction();
  auto *MFI = MF.getInfo<GenMMachineFunctionInfo>();

  for (const auto &In : Ins) {
    InVals.push_back(DAG.getNode())
  }

  return Chain;
}

SDValue GenMTargetLowering::LowerCall(
    TargetLowering::CallLoweringInfo &CLI,
    SmallVectorImpl<SDValue> &InVals) const
{
  assert(!"not implemented");
}

SDValue GenMTargetLowering::LowerReturn(
    SDValue Chain,
    CallingConv::ID CallConv,
    bool isVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    const SmallVectorImpl<SDValue> &OutVals,
    const SDLoc &dl, SelectionDAG &DAG) const
{
  assert(!"not implemented");
}

SDValue GenMTargetLowering::PerformDAGCombine(
    SDNode *N,
    DAGCombinerInfo &DCI) const
{
  assert(!"not implemented");
}

bool GenMTargetLowering::ShouldShrinkFPConstant(EVT VT) const
{
  assert(!"not implemented");
}

bool GenMTargetLowering::shouldInsertFencesForAtomic(const Instruction *I) const
{
  assert(!"not implemented");
}

GenMTargetLowering::AtomicExpansionKind
GenMTargetLowering::shouldExpandAtomicRMWInIR(AtomicRMWInst *AI) const
{
  assert(!"not implemented");
}

void GenMTargetLowering::ReplaceNodeResults(
    SDNode *N,
    SmallVectorImpl<SDValue>& Results,
    SelectionDAG &DAG) const
{
  assert(!"not implemented");
}
