//===-- LLIRISelLowering.h - LLIR DAG Lowering Interface --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that LLIR uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIRTARGETLOWERING_H
#define LLVM_LIB_TARGET_LLIR_LLIRTARGETLOWERING_H

#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

namespace LLIRISD {
enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  // Arguments.
  ARGUMENT,
  // Calls.
  CALL,
  TCALL,
  VOID,
  TVOID,
  // Symbols.
  SYMBOL,
  // Control flow.
  SWITCH,
  RETURN,
  // Varargs.
  VASTART,
  // Overflow checks.
  SADDO,
  UADDO,
  SSUBO,
  USUBO,
  SMULO,
  UMULO,
  // Alloca.
  ALLOCA,
  // RDTSC
  RDTSC,
  // Atomics.
  LL,
  SC,
};
} // end namespace LLIRISD

class LLIRSubtarget;

class LLIRTargetLowering : public TargetLowering {
public:
  LLIRTargetLowering(const TargetMachine &TM, const LLIRSubtarget &STI);

  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  SDValue LowerFRAMEADDR(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFrameIndex(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerExternalSymbol(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerJumpTable(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerConstantPool(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerBR_JT(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerDynamicStackalloc(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerVASTART(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerVAARG(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerVACOPY(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerCopyToReg(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerALUO(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerINTRINSIC_WO_CHAIN(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerINTRINSIC_W_CHAIN(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerINTRINSIC_VOID(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerREADCYCLECOUNTER(SDValue Op, SelectionDAG &DAG) const;

  bool useSoftFloat() const override { return false; }
  bool canLowerUndef() const override { return true; }

  MachineBasicBlock *
  EmitInstrWithCustomInserter(MachineInstr &MI,
                              MachineBasicBlock *MBB) const override;

  const char *getTargetNodeName(unsigned Opcode) const override;

  std::pair<unsigned, const TargetRegisterClass *>
  getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                               StringRef Constraint, MVT VT) const override;

  bool isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const override;

  MVT getScalarShiftAmountTy(const DataLayout &, EVT) const override;

  EVT getSetCCResultType(const DataLayout &DL, LLVMContext &Context,
                         EVT VT) const override;

  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool isVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &dl, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerF128Call(SDValue Op, SelectionDAG &DAG,
                        RTLIB::Libcall Call) const;
  SDValue LowerI128Call(SDValue Op, SelectionDAG &DAG,
                        RTLIB::Libcall Call) const;

  SDValue LowerSETCC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFP_EXTEND(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFP_ROUND(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFP_TO_INT(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerINT_TO_FP(SDValue Op, SelectionDAG &DAG) const;

  void ReplaceNodeResults(SDNode *N, SmallVectorImpl<SDValue>&Results,
                          SelectionDAG &DAG) const override;

  bool CanLowerReturn(CallingConv::ID CallConv, MachineFunction &MF,
                      bool isVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      LLVMContext &Context) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &dl,
                      SelectionDAG &DAG) const override;

  AtomicExpansionKind
  shouldExpandAtomicRMWInIR(AtomicRMWInst *AI) const override;

  Value *emitLoadLinked(IRBuilder<> &Builder, Value *Addr,
                        AtomicOrdering Ord) const override;
  Value *emitStoreConditional(IRBuilder<> &Builder, Value *Val, Value *Addr,
                              AtomicOrdering Ord) const override;

  Value *emitLoadLinkedAArch64(IRBuilder<> &Builder, Value *Addr,
                               AtomicOrdering Ord) const;
  Value *emitStoreConditionalAArch64(IRBuilder<> &Builder, Value *Val,
                                     Value *Addr,
                                     AtomicOrdering Ord) const;

  bool mayBeEmittedAsTailCall(const CallInst *CI) const override;

private:
  const LLIRSubtarget *Subtarget;
};

} // end namespace llvm

#endif
