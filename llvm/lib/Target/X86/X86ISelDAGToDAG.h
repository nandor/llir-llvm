//===-- X86ISelDAGToDAG.h - A DAG pattern matching inst selector for X8 ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_X86_X86ISELDAGTODAG_H
#define LLVM_LIB_TARGET_X86_X86ISELDAGTODAG_H

#include "X86RegisterInfo.h"
#include "X86Subtarget.h"
#include "X86TargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

namespace llvm {
class TargetMachine;
struct X86ISelAddressMode;

//===--------------------------------------------------------------------===//
/// X86-Specific DAG matcher.
///
class X86DAGMatcher : public virtual DAGMatcher {
protected:
  /// Keep a pointer to the X86Subtarget around so that we can
  /// make the right decision when generating code for different targets.
  const X86Subtarget *Subtarget;

  /// If true, selector should try to optimize for minimum code size.
  bool OptForMinSize;

  /// Disable direct TLS access through segment registers.
  bool IndirectTlsSegRefs;

public:
  explicit X86DAGMatcher(
      X86TargetMachine &tm,
      CodeGenOpt::Level OL,
      const X86Subtarget *Subtarget = nullptr
  );

  void Select(SDNode *N);

  bool foldOffsetIntoAddress(uint64_t Offset, X86ISelAddressMode &AM);
  bool matchLoadInAddress(LoadSDNode *N, X86ISelAddressMode &AM);
  bool matchWrapper(SDValue N, X86ISelAddressMode &AM);
  bool matchAddress(SDValue N, X86ISelAddressMode &AM);
  bool matchVectorAddress(SDValue N, X86ISelAddressMode &AM);
  bool matchAdd(SDValue &N, X86ISelAddressMode &AM, unsigned Depth);
  bool matchAddressRecursively(SDValue N, X86ISelAddressMode &AM,
                               unsigned Depth);
  bool matchAddressBase(SDValue N, X86ISelAddressMode &AM);
  bool selectAddr(SDNode *Parent, SDValue N, SDValue &Base,
                  SDValue &Scale, SDValue &Index, SDValue &Disp,
                  SDValue &Segment);
  bool selectVectorAddr(MemSDNode *Parent, SDValue BasePtr, SDValue IndexOp,
                        SDValue ScaleOp, SDValue &Base, SDValue &Scale,
                        SDValue &Index, SDValue &Disp, SDValue &Segment);
  bool selectMOV64Imm32(SDValue N, SDValue &Imm);
  bool selectLEAAddr(SDValue N, SDValue &Base,
                     SDValue &Scale, SDValue &Index, SDValue &Disp,
                     SDValue &Segment);
  bool selectLEA64_32Addr(SDValue N, SDValue &Base,
                          SDValue &Scale, SDValue &Index, SDValue &Disp,
                          SDValue &Segment);
  bool selectTLSADDRAddr(SDValue N, SDValue &Base,
                         SDValue &Scale, SDValue &Index, SDValue &Disp,
                         SDValue &Segment);
  bool selectRelocImm(SDValue N, SDValue &Op);

  bool tryFoldLoad(SDNode *Root, SDNode *P, SDValue N,
                   SDValue &Base, SDValue &Scale,
                   SDValue &Index, SDValue &Disp,
                   SDValue &Segment);

  // Convenience method where P is also root.
  bool tryFoldLoad(SDNode *P, SDValue N,
                   SDValue &Base, SDValue &Scale,
                   SDValue &Index, SDValue &Disp,
                   SDValue &Segment) {
    return tryFoldLoad(P, P, N, Base, Scale, Index, Disp, Segment);
  }

  bool tryFoldBroadcast(SDNode *Root, SDNode *P, SDValue N,
                        SDValue &Base, SDValue &Scale,
                        SDValue &Index, SDValue &Disp,
                        SDValue &Segment);

  bool isProfitableToFormMaskedOp(SDNode *N) const;

  /// Implement addressing mode selection for inline asm expressions.
  bool SelectInlineAsmMemoryOperand(const SDValue &Op,
                                    unsigned ConstraintID,
                                    std::vector<SDValue> &OutOps) override;

  // Try to fold a vector load. This makes sure the load isn't non-temporal.
  bool tryFoldVecLoad(SDNode *Root, SDNode *P, SDValue N,
                      SDValue &Base, SDValue &Scale,
                      SDValue &Index, SDValue &Disp,
                      SDValue &Segment);


  void getAddressOperands(X86ISelAddressMode &AM, const SDLoc &DL,
                          MVT VT, SDValue &Base, SDValue &Scale,
                          SDValue &Index, SDValue &Disp,
                          SDValue &Segment);

  // Utility function to determine whether we should avoid selecting
  // immediate forms of instructions for better code size or not.
  // At a high level, we'd like to avoid such instructions when
  // we have similar constants used within the same basic block
  // that can be kept in a register.
  //
  bool shouldAvoidImmediateInstFormsForSize(SDNode *N) const;

  /// Return a target constant with the specified value of type i8.
  inline SDValue getI8Imm(unsigned Imm, const SDLoc &DL) {
    return CurDAG->getTargetConstant(Imm, DL, MVT::i8);
  }

  /// Return a target constant with the specified value, of type i32.
  inline SDValue getI32Imm(unsigned Imm, const SDLoc &DL) {
    return CurDAG->getTargetConstant(Imm, DL, MVT::i32);
  }

  /// Return a target constant with the specified value, of type i64.
  inline SDValue getI64Imm(uint64_t Imm, const SDLoc &DL) {
    return CurDAG->getTargetConstant(Imm, DL, MVT::i64);
  }

  SDValue getExtractVEXTRACTImmediate(SDNode *N, unsigned VecWidth,
                                      const SDLoc &DL) {
    assert((VecWidth == 128 || VecWidth == 256) && "Unexpected vector width");
    uint64_t Index = N->getConstantOperandVal(1);
    MVT VecVT = N->getOperand(0).getSimpleValueType();
    return getI8Imm((Index * VecVT.getScalarSizeInBits()) / VecWidth, DL);
  }

  SDValue getInsertVINSERTImmediate(SDNode *N, unsigned VecWidth,
                                    const SDLoc &DL) {
    assert((VecWidth == 128 || VecWidth == 256) && "Unexpected vector width");
    uint64_t Index = N->getConstantOperandVal(2);
    MVT VecVT = N->getSimpleValueType(0);
    return getI8Imm((Index * VecVT.getScalarSizeInBits()) / VecWidth, DL);
  }

  // Helper to detect unneeded and instructions on shift amounts. Called
  // from PatFrags in tablegen.
  bool isUnneededShiftMask(SDNode *N, unsigned Width) const;

  /// Return an SDNode that returns the value of the global base register.
  /// Output instructions required to initialize the global base register,
  /// if necessary.
  SDNode *getGlobalBaseReg();

  /// Return a reference to the TargetInstrInfo, casted to the target-specific
  /// type.
  const X86InstrInfo *getInstrInfo() const {
    return Subtarget->getInstrInfo();
  }

  bool isSExtAbsoluteSymbolRef(unsigned Width, SDNode *N) const;

  /// Returns whether this is a relocatable immediate in the range
  /// [-2^Width .. 2^Width-1].
  template <unsigned Width> bool isSExtRelocImm(SDNode *N) const {
    if (auto *CN = dyn_cast<ConstantSDNode>(N))
      return isInt<Width>(CN->getSExtValue());
    return isSExtAbsoluteSymbolRef(Width, N);
  }

  // Indicates we should prefer to use a non-temporal load for this load.
  bool useNonTemporalLoad(LoadSDNode *N) const;

  bool foldLoadStoreIntoMemOperand(SDNode *Node);
  MachineSDNode *matchBEXTRFromAndImm(SDNode *Node);
  bool matchBitExtract(SDNode *Node);
  bool shrinkAndImmediate(SDNode *N);
  bool isMaskZeroExtended(SDNode *N) const;
  bool tryShiftAmountMod(SDNode *N);
  bool tryShrinkShlLogicImm(SDNode *N);
  bool tryVPTERNLOG(SDNode *N);
  bool matchVPTERNLOG(SDNode *Root, SDNode *ParentA, SDNode *ParentBC,
                      SDValue A, SDValue B, SDValue C, uint8_t Imm);
  bool tryVPTESTM(SDNode *Root, SDValue Setcc, SDValue Mask);
  bool tryMatchBitSelect(SDNode *N);

  MachineSDNode *emitPCMPISTR(unsigned ROpc, unsigned MOpc, bool MayFoldLoad,
                              const SDLoc &dl, MVT VT, SDNode *Node);
  MachineSDNode *emitPCMPESTR(unsigned ROpc, unsigned MOpc, bool MayFoldLoad,
                              const SDLoc &dl, MVT VT, SDNode *Node,
                              SDValue &InFlag);

  bool tryOptimizeRem8Extend(SDNode *N);

  bool onlyUsesZeroFlag(SDValue Flags) const;
  bool hasNoSignFlagUses(SDValue Flags) const;
  bool hasNoCarryFlagUses(SDValue Flags) const;

  bool IsProfitableToFold(SDValue N, SDNode *U, SDNode *Root) const override;

  virtual const X86TargetMachine &getTargetMachine() const = 0;

  /// Address-mode matching performs shift-of-and to and-of-shift
  /// reassociation in order to expose more scaled addressing
  /// opportunities.
  bool ComplexPatternFuncMutatesDAG() const override {
    return true;
  }

  void PreprocessISelDAG() override;
  void PostprocessISelDAG() override;

  #define GET_DAGISEL_DECL
  #include "X86GenDAGISel.inc"
};

//===--------------------------------------------------------------------===//
/// ISel - X86-specific code to select X86 machine instructions for
/// SelectionDAG operations.
///
class X86DAGToDAGISel final : public X86DAGMatcher, public SelectionDAGISel {
public:
  explicit X86DAGToDAGISel(X86TargetMachine &tm, CodeGenOpt::Level OptLevel);

  StringRef getPassName() const override {
    return "X86 DAG->DAG Instruction Selection";
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  void emitFunctionEntryCode() override;

private:
  void Select(SDNode *N) override { X86DAGMatcher::Select(N); }

  void emitSpecialCodeForMain();

  /// Return a reference to the TargetMachine, casted to the target-specific
  /// type.
  const X86TargetMachine &getTargetMachine() const override {
    return static_cast<const X86TargetMachine &>(TM);
  }
};

} // End llvm namespace

#endif
