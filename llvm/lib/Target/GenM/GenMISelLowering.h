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

namespace GenMISD {
enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  ARGUMENT,
  RETURN,
  CALL,
  VOID,
  SYMBOL,
  BR_JT,
};
} // end namespace GenMISD

class GenMSubtarget;

class GenMTargetLowering : public TargetLowering {
public:
  GenMTargetLowering(const TargetMachine &TM, const GenMSubtarget &STI);

  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  SDValue LowerFrameIndex(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerExternalSymbol(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerJumpTable(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerBR_JT(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerVASTART(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerCopyToReg(SDValue Op, SelectionDAG &DAG) const;

  bool useSoftFloat() const override;

  MachineBasicBlock *EmitInstrWithCustomInserter(
      MachineInstr &MI,
      MachineBasicBlock *MBB
  ) const override;

  const char *getTargetNodeName(unsigned Opcode) const override;

  std::pair<unsigned, const TargetRegisterClass *> getRegForInlineAsmConstraint(
      const TargetRegisterInfo *TRI,
      StringRef Constraint,
      MVT VT
  ) const override;

  bool isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const override;

  MVT getScalarShiftAmountTy(const DataLayout &, EVT) const override;

  EVT getSetCCResultType(
      const DataLayout &DL,
      LLVMContext &Context,
      EVT VT
  ) const override;

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

  bool CanLowerReturn(
      CallingConv::ID CallConv,
      MachineFunction &MF,
      bool isVarArg,
      const SmallVectorImpl<ISD::OutputArg> &Outs,
      LLVMContext &Context
  ) const override;

  SDValue LowerReturn(
      SDValue Chain,
      CallingConv::ID CallConv,
      bool isVarArg,
      const SmallVectorImpl<ISD::OutputArg> &Outs,
      const SmallVectorImpl<SDValue> &OutVals,
      const SDLoc &dl, SelectionDAG &DAG
  ) const override;

  AtomicExpansionKind shouldExpandAtomicRMWInIR(
      AtomicRMWInst *AI
  ) const override;

private:
  const GenMSubtarget *Subtarget;
};

} // end namespace llvm

#endif    // GENM_TARGETLOWERING_H
