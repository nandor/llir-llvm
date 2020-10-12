//===-- AArch64ISelDAGToDAG.h - A dag to dag inst selector for AArch64 ----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AARCH64_AARCH64ISELDAGTODAG_H
#define LLVM_LIB_TARGET_AARCH64_AARCH64ISELDAGTODAG_H

#include "AArch64RegisterInfo.h"
#include "AArch64Subtarget.h"
#include "AArch64TargetMachine.h"
#include "MCTargetDesc/AArch64AddressingModes.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

namespace llvm {
class TargetMachine;

//===--------------------------------------------------------------------===//
/// AArch64DAGToDAGISel - AArch64 specific code to select AArch64 machine
/// instructions for SelectionDAG operations.
///
class AArch64DAGMatcher : public virtual DAGMatcher {
protected:
  /// Subtarget - Keep a pointer to the AArch64Subtarget around so that we can
  /// make the right decision when generating code for different targets.
  const AArch64Subtarget *Subtarget;

public:
  explicit AArch64DAGMatcher(AArch64TargetMachine &tm,
                             CodeGenOpt::Level OptLevel,
                             const AArch64Subtarget *Subtarget = nullptr);

  void Select(SDNode *Node);

  /// SelectInlineAsmMemoryOperand - Implement addressing mode selection for
  /// inline asm expressions.
  bool SelectInlineAsmMemoryOperand(const SDValue &Op, unsigned ConstraintID,
                                    std::vector<SDValue> &OutOps) override;

  template <signed Low, signed High, signed Scale>
  bool SelectRDVLImm(SDValue N, SDValue &Imm);

  bool tryMLAV64LaneV128(SDNode *N);
  bool tryMULLV64LaneV128(unsigned IntNo, SDNode *N);
  bool SelectArithExtendedRegister(SDValue N, SDValue &Reg, SDValue &Shift);
  bool SelectArithImmed(SDValue N, SDValue &Val, SDValue &Shift);
  bool SelectNegArithImmed(SDValue N, SDValue &Val, SDValue &Shift);
  bool SelectArithShiftedRegister(SDValue N, SDValue &Reg, SDValue &Shift) {
    return SelectShiftedRegister(N, false, Reg, Shift);
  }
  bool SelectLogicalShiftedRegister(SDValue N, SDValue &Reg, SDValue &Shift) {
    return SelectShiftedRegister(N, true, Reg, Shift);
  }
  bool SelectAddrModeIndexed7S8(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexed7S(N, 1, Base, OffImm);
  }
  bool SelectAddrModeIndexed7S16(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexed7S(N, 2, Base, OffImm);
  }
  bool SelectAddrModeIndexed7S32(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexed7S(N, 4, Base, OffImm);
  }
  bool SelectAddrModeIndexed7S64(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexed7S(N, 8, Base, OffImm);
  }
  bool SelectAddrModeIndexed7S128(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexed7S(N, 16, Base, OffImm);
  }
  bool SelectAddrModeIndexedS9S128(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexedBitWidth(N, true, 9, 16, Base, OffImm);
  }
  bool SelectAddrModeIndexedU6S128(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexedBitWidth(N, false, 6, 16, Base, OffImm);
  }
  bool SelectAddrModeIndexed8(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexed(N, 1, Base, OffImm);
  }
  bool SelectAddrModeIndexed16(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexed(N, 2, Base, OffImm);
  }
  bool SelectAddrModeIndexed32(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexed(N, 4, Base, OffImm);
  }
  bool SelectAddrModeIndexed64(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexed(N, 8, Base, OffImm);
  }
  bool SelectAddrModeIndexed128(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeIndexed(N, 16, Base, OffImm);
  }
  bool SelectAddrModeUnscaled8(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeUnscaled(N, 1, Base, OffImm);
  }
  bool SelectAddrModeUnscaled16(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeUnscaled(N, 2, Base, OffImm);
  }
  bool SelectAddrModeUnscaled32(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeUnscaled(N, 4, Base, OffImm);
  }
  bool SelectAddrModeUnscaled64(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeUnscaled(N, 8, Base, OffImm);
  }
  bool SelectAddrModeUnscaled128(SDValue N, SDValue &Base, SDValue &OffImm) {
    return SelectAddrModeUnscaled(N, 16, Base, OffImm);
  }

  template <int Width>
  bool SelectAddrModeWRO(SDValue N, SDValue &Base, SDValue &Offset,
                         SDValue &SignExtend, SDValue &DoShift) {
    return SelectAddrModeWRO(N, Width / 8, Base, Offset, SignExtend, DoShift);
  }

  template <int Width>
  bool SelectAddrModeXRO(SDValue N, SDValue &Base, SDValue &Offset,
                         SDValue &SignExtend, SDValue &DoShift) {
    return SelectAddrModeXRO(N, Width / 8, Base, Offset, SignExtend, DoShift);
  }

  bool SelectDupZeroOrUndef(SDValue N) {
    switch (N->getOpcode()) {
    case ISD::UNDEF:
      return true;
    case AArch64ISD::DUP:
    case ISD::SPLAT_VECTOR: {
      auto Opnd0 = N->getOperand(0);
      if (auto CN = dyn_cast<ConstantSDNode>(Opnd0))
        if (CN->isNullValue())
          return true;
      if (auto CN = dyn_cast<ConstantFPSDNode>(Opnd0))
        if (CN->isZero())
          return true;
      break;
    }
    default:
      break;
    }

    return false;
  }

  bool SelectDupZero(SDValue N) {
    switch (N->getOpcode()) {
    case AArch64ISD::DUP:
    case ISD::SPLAT_VECTOR: {
      auto Opnd0 = N->getOperand(0);
      if (auto CN = dyn_cast<ConstantSDNode>(Opnd0))
        if (CN->isNullValue())
          return true;
      if (auto CN = dyn_cast<ConstantFPSDNode>(Opnd0))
        if (CN->isZero())
          return true;
      break;
    }
    }

    return false;
  }

  template <MVT::SimpleValueType VT>
  bool SelectSVEAddSubImm(SDValue N, SDValue &Imm, SDValue &Shift) {
    return SelectSVEAddSubImm(N, VT, Imm, Shift);
  }

  template <MVT::SimpleValueType VT>
  bool SelectSVELogicalImm(SDValue N, SDValue &Imm) {
    return SelectSVELogicalImm(N, VT, Imm);
  }

  template <unsigned Low, unsigned High, bool AllowSaturation = false>
  bool SelectSVEShiftImm(SDValue N, SDValue &Imm) {
    return SelectSVEShiftImm(N, Low, High, AllowSaturation, Imm);
  }

  // Returns a suitable CNT/INC/DEC/RDVL multiplier to calculate VSCALE*N.
  template <signed Min, signed Max, signed Scale, bool Shift>
  bool SelectCntImm(SDValue N, SDValue &Imm) {
    if (!isa<ConstantSDNode>(N))
      return false;

    int64_t MulImm = cast<ConstantSDNode>(N)->getSExtValue();
    if (Shift)
      MulImm = 1LL << MulImm;

    if ((MulImm % std::abs(Scale)) != 0)
      return false;

    MulImm /= Scale;
    if ((MulImm >= Min) && (MulImm <= Max)) {
      Imm = CurDAG->getTargetConstant(MulImm, SDLoc(N), MVT::i32);
      return true;
    }

    return false;
  }

  /// Form sequences of consecutive 64/128-bit registers for use in NEON
  /// instructions making use of a vector-list (e.g. ldN, tbl). Vecs must have
  /// between 1 and 4 elements. If it contains a single element that is returned
  /// unchanged; otherwise a REG_SEQUENCE value is returned.
  SDValue createDTuple(ArrayRef<SDValue> Vecs);
  SDValue createQTuple(ArrayRef<SDValue> Vecs);
  // Form a sequence of SVE registers for instructions using list of vectors,
  // e.g. structured loads and stores (ldN, stN).
  SDValue createZTuple(ArrayRef<SDValue> Vecs);

  /// Generic helper for the createDTuple/createQTuple
  /// functions. Those should almost always be called instead.
  SDValue createTuple(ArrayRef<SDValue> Vecs, const unsigned RegClassIDs[],
                      const unsigned SubRegs[]);

  void SelectTable(SDNode *N, unsigned NumVecs, unsigned Opc, bool isExt);

  bool tryIndexedLoad(SDNode *N);

  bool trySelectStackSlotTagP(SDNode *N);
  void SelectTagP(SDNode *N);

  void SelectLoad(SDNode *N, unsigned NumVecs, unsigned Opc,
                  unsigned SubRegIdx);
  void SelectPostLoad(SDNode *N, unsigned NumVecs, unsigned Opc,
                      unsigned SubRegIdx);
  void SelectLoadLane(SDNode *N, unsigned NumVecs, unsigned Opc);
  void SelectPostLoadLane(SDNode *N, unsigned NumVecs, unsigned Opc);
  void SelectPredicatedLoad(SDNode *N, unsigned NumVecs, unsigned Scale,
                            unsigned Opc_rr, unsigned Opc_ri);

  bool SelectAddrModeFrameIndexSVE(SDValue N, SDValue &Base, SDValue &OffImm);
  /// SVE Reg+Imm addressing mode.
  template <int64_t Min, int64_t Max>
  bool SelectAddrModeIndexedSVE(SDNode *Root, SDValue N, SDValue &Base,
                                SDValue &OffImm);
  /// SVE Reg+Reg address mode.
  template <unsigned Scale>
  bool SelectSVERegRegAddrMode(SDValue N, SDValue &Base, SDValue &Offset) {
    return SelectSVERegRegAddrMode(N, Scale, Base, Offset);
  }

  void SelectStore(SDNode *N, unsigned NumVecs, unsigned Opc);
  void SelectPostStore(SDNode *N, unsigned NumVecs, unsigned Opc);
  void SelectStoreLane(SDNode *N, unsigned NumVecs, unsigned Opc);
  void SelectPostStoreLane(SDNode *N, unsigned NumVecs, unsigned Opc);
  void SelectPredicatedStore(SDNode *N, unsigned NumVecs, unsigned Scale,
                             unsigned Opc_rr, unsigned Opc_ri);
  std::tuple<unsigned, SDValue, SDValue>
  findAddrModeSVELoadStore(SDNode *N, unsigned Opc_rr, unsigned Opc_ri,
                           const SDValue &OldBase, const SDValue &OldOffset,
                           unsigned Scale);

  bool tryBitfieldExtractOp(SDNode *N);
  bool tryBitfieldExtractOpFromSExt(SDNode *N);
  bool tryBitfieldInsertOp(SDNode *N);
  bool tryBitfieldInsertInZeroOp(SDNode *N);
  bool tryShiftAmountMod(SDNode *N);
  bool tryHighFPExt(SDNode *N);

  bool tryReadRegister(SDNode *N);
  bool tryWriteRegister(SDNode *N);

// Include the pieces autogenerated from the target description.
#include "AArch64GenDAGISel.inc"

protected:
  bool SelectShiftedRegister(SDValue N, bool AllowROR, SDValue &Reg,
                             SDValue &Shift);
  bool SelectAddrModeIndexed7S(SDValue N, unsigned Size, SDValue &Base,
                               SDValue &OffImm) {
    return SelectAddrModeIndexedBitWidth(N, true, 7, Size, Base, OffImm);
  }
  bool SelectAddrModeIndexedBitWidth(SDValue N, bool IsSignedImm, unsigned BW,
                                     unsigned Size, SDValue &Base,
                                     SDValue &OffImm);
  bool SelectAddrModeIndexed(SDValue N, unsigned Size, SDValue &Base,
                             SDValue &OffImm);
  bool SelectAddrModeUnscaled(SDValue N, unsigned Size, SDValue &Base,
                              SDValue &OffImm);
  bool SelectAddrModeWRO(SDValue N, unsigned Size, SDValue &Base,
                         SDValue &Offset, SDValue &SignExtend,
                         SDValue &DoShift);
  bool SelectAddrModeXRO(SDValue N, unsigned Size, SDValue &Base,
                         SDValue &Offset, SDValue &SignExtend,
                         SDValue &DoShift);
  bool isWorthFolding(SDValue V) const;
  bool SelectExtendedSHL(SDValue N, unsigned Size, bool WantExtend,
                         SDValue &Offset, SDValue &SignExtend);

  template <unsigned RegWidth>
  bool SelectCVTFixedPosOperand(SDValue N, SDValue &FixedPos) {
    return SelectCVTFixedPosOperand(N, FixedPos, RegWidth);
  }

  bool SelectCVTFixedPosOperand(SDValue N, SDValue &FixedPos, unsigned Width);

  bool SelectCMP_SWAP(SDNode *N);

  bool SelectSVE8BitLslImm(SDValue N, SDValue &Imm, SDValue &Shift);

  bool SelectSVEAddSubImm(SDValue N, MVT VT, SDValue &Imm, SDValue &Shift);

  bool SelectSVELogicalImm(SDValue N, MVT VT, SDValue &Imm);

  bool SelectSVESignedArithImm(SDValue N, SDValue &Imm);
  bool SelectSVEShiftImm(SDValue N, uint64_t Low, uint64_t High,
                         bool AllowSaturation, SDValue &Imm);

  bool SelectSVEArithImm(SDValue N, SDValue &Imm);
  bool SelectSVERegRegAddrMode(SDValue N, unsigned Scale, SDValue &Base,
                               SDValue &Offset);

  virtual const TargetLowering *getTargetLowering() const = 0;
};

//===--------------------------------------------------------------------===//
/// ISel - AArch64-specific code to select AArch64 machine instructions for
/// SelectionDAG operations.
///
class AArch64DAGToDAGISel final : public AArch64DAGMatcher,
                                  public SelectionDAGISel {
public:
  explicit AArch64DAGToDAGISel(AArch64TargetMachine &tm,
                               CodeGenOpt::Level OptLevel);

  StringRef getPassName() const override {
    return "AArch64 Instruction Selection";
  }

  bool runOnMachineFunction(MachineFunction &MF) override {
    Subtarget = &MF.getSubtarget<AArch64Subtarget>();
    return SelectionDAGISel::runOnMachineFunction(MF);
  }

private:
  void Select(SDNode *N) override { AArch64DAGMatcher::Select(N); }

  /// Return a reference to the TargetMachine, casted to the target-specific
  /// type.
  const TargetLowering *getTargetLowering() const override {
    return SelectionDAGISel::getTargetLowering();
  }
};

} // namespace llvm

#endif
