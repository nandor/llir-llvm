//===-- GenMISelLowering.h - GenM DAG Lowering Interface --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that GenM uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_GENMTARGETLOWERING_H
#define LLVM_LIB_TARGET_GENM_GENMTARGETLOWERING_H

#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class GenMSubtarget;

class GenMTargetLowering : public TargetLowering {
public:
  GenMTargetLowering(const TargetMachine &TM, const GenMSubtarget &STI);
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  bool useSoftFloat() const override;

  /// computeKnownBitsForTargetNode - Determine which of the bits specified
  /// in Mask are known to be either zero or one and return them in the
  /// KnownZero/KnownOne bitsets.
  void computeKnownBitsForTargetNode(
     const SDValue Op,
     KnownBits &Known,
     const APInt &DemandedElts,
     const SelectionDAG &DAG,
     unsigned Depth = 0
   ) const override;

  MachineBasicBlock *EmitInstrWithCustomInserter(
      MachineInstr &MI,
      MachineBasicBlock *MBB
  ) const override;

  const char *getTargetNodeName(unsigned Opcode) const override;

  ConstraintType getConstraintType(StringRef Constraint) const override;
  ConstraintWeight getSingleConstraintMatchWeight(
      AsmOperandInfo &info,
      const char *constraint
  ) const override;

  void LowerAsmOperandForConstraint(
      SDValue Op,
      std::string &Constraint,
      std::vector<SDValue> &Ops,
      SelectionDAG &DAG
  ) const override;

  unsigned getInlineAsmMemConstraint(StringRef ConstraintCode) const override;

  std::pair<unsigned, const TargetRegisterClass *> getRegForInlineAsmConstraint(
      const TargetRegisterInfo *TRI,
      StringRef Constraint,
      MVT VT
  ) const override;

  bool isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const override;

  MVT getScalarShiftAmountTy(const DataLayout &, EVT) const override {
    return MVT::i32;
  }

  unsigned getRegisterByName(const char* RegName, EVT VT,
                             SelectionDAG &DAG) const override;

  /// If a physical register, this returns the register that receives the
  /// exception address on entry to an EH pad.
  unsigned
  getExceptionPointerRegister(const Constant *PersonalityFn) const override;

  /// If a physical register, this returns the register that receives the
  /// exception typeid on entry to a landing pad.
  unsigned
  getExceptionSelectorRegister(const Constant *PersonalityFn) const override;

  /// Override to support customized stack guard loading.
  bool useLoadStackGuardNode() const override;
  void insertSSPDeclarations(Module &M) const override;

  /// getSetCCResultType - Return the ISD::SETCC ValueType
  EVT getSetCCResultType(
      const DataLayout &DL,
      LLVMContext &Context,
      EVT VT) const override;

  SDValue LowerFormalArguments(
      SDValue Chain,
      CallingConv::ID CallConv,
      bool isVarArg,
      const SmallVectorImpl<ISD::InputArg> &Ins,
      const SDLoc &dl,
      SelectionDAG &DAG,
      SmallVectorImpl<SDValue> &InVals
  ) const override;

  SDValue LowerCall(
      TargetLowering::CallLoweringInfo &CLI,
      SmallVectorImpl<SDValue> &InVals
  ) const override;

  SDValue LowerReturn(
      SDValue Chain,
      CallingConv::ID CallConv,
      bool isVarArg,
      const SmallVectorImpl<ISD::OutputArg> &Outs,
      const SmallVectorImpl<SDValue> &OutVals,
      const SDLoc &dl, SelectionDAG &DAG
  ) const override;

  SDValue PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI) const override;

  bool ShouldShrinkFPConstant(EVT VT) const override;

  bool shouldInsertFencesForAtomic(const Instruction *I) const override;

  AtomicExpansionKind shouldExpandAtomicRMWInIR(
      AtomicRMWInst *AI
  ) const override;

  void ReplaceNodeResults(
      SDNode *N,
      SmallVectorImpl<SDValue>& Results,
      SelectionDAG &DAG
  ) const override;
};

} // end namespace llvm

#endif    // GENM_TARGETLOWERING_H
