//===-- PPCISelDAGToDAG.h - PPC --pattern matching inst selector ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines a pattern matching instruction selector for PowerPC,
// converting from a legalized dag to a PPC dag.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_POWERPC_PPCCISELDAGTODAG_H
#define LLVM_LIB_TARGET_POWERPC_PPCCISELDAGTODAG_H

#include "PPCRegisterInfo.h"
#include "PPCSubtarget.h"
#include "PPCTargetMachine.h"
#include "PPCMachineFunctionInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

namespace llvm {

//===--------------------------------------------------------------------===//
/// PPCDAGToDAGISel - PPC specific code to select PPC machine
/// instructions for SelectionDAG operations.
///
class PPCDAGMatcher : public virtual DAGMatcher {
protected:
  const PPCTargetMachine &TM;
  const PPCSubtarget *PPCSubTarget = nullptr;
  const PPCSubtarget *Subtarget = nullptr;
  const PPCTargetLowering *PPCLowering = nullptr;
  unsigned GlobalBaseReg = 0;

public:
  explicit PPCDAGMatcher(PPCTargetMachine &tm, CodeGenOpt::Level OptLevel,
                         const PPCSubtarget *Subtarget = nullptr);

  void PreprocessISelDAG() override;
  void PostprocessISelDAG() override;

  /// getI16Imm - Return a target constant with the specified value, of type
  /// i16.
  inline SDValue getI16Imm(unsigned Imm, const SDLoc &dl) {
    return CurDAG->getTargetConstant(Imm, dl, MVT::i16);
  }

  /// getI32Imm - Return a target constant with the specified value, of type
  /// i32.
  inline SDValue getI32Imm(unsigned Imm, const SDLoc &dl) {
    return CurDAG->getTargetConstant(Imm, dl, MVT::i32);
  }

  /// getI64Imm - Return a target constant with the specified value, of type
  /// i64.
  inline SDValue getI64Imm(uint64_t Imm, const SDLoc &dl) {
    return CurDAG->getTargetConstant(Imm, dl, MVT::i64);
  }

  /// getSmallIPtrImm - Return a target constant of pointer type.
  inline SDValue getSmallIPtrImm(unsigned Imm, const SDLoc &dl) {
    return CurDAG->getTargetConstant(
        Imm, dl, PPCLowering->getPointerTy(CurDAG->getDataLayout()));
  }

  /// isRotateAndMask - Returns true if Mask and Shift can be folded into a
  /// rotate and mask opcode and mask operation.
  static bool isRotateAndMask(SDNode *N, unsigned Mask, bool isShiftMask,
                              unsigned &SH, unsigned &MB, unsigned &ME);

  /// getGlobalBaseReg - insert code into the entry mbb to materialize the PIC
  /// base register.  Return the virtual register that holds this value.
  SDNode *getGlobalBaseReg();

  void selectFrameIndex(SDNode *SN, SDNode *N, unsigned Offset = 0);

  // Select - Convert the specified operand from a target-independent to a
  // target-specific node if it hasn't already been changed.
  void Select(SDNode *N);

  bool tryBitfieldInsert(SDNode *N);
  bool tryBitPermutation(SDNode *N);
  bool tryIntCompareInGPR(SDNode *N);

  // tryTLSXFormLoad - Convert an ISD::LOAD fed by a PPCISD::ADD_TLS into
  // an X-Form load instruction with the offset being a relocation coming from
  // the PPCISD::ADD_TLS.
  bool tryTLSXFormLoad(LoadSDNode *N);
  // tryTLSXFormStore - Convert an ISD::STORE fed by a PPCISD::ADD_TLS into
  // an X-Form store instruction with the offset being a relocation coming from
  // the PPCISD::ADD_TLS.
  bool tryTLSXFormStore(StoreSDNode *N);
  /// SelectCC - Select a comparison of the specified values with the
  /// specified condition code, returning the CR# of the expression.
  SDValue SelectCC(SDValue LHS, SDValue RHS, ISD::CondCode CC, const SDLoc &dl,
                   SDValue Chain = SDValue());

  /// SelectAddrImmOffs - Return true if the operand is valid for a preinc
  /// immediate field.  Note that the operand at this point is already the
  /// result of a prior SelectAddressRegImm call.
  bool SelectAddrImmOffs(SDValue N, SDValue &Out) const;

  /// SelectAddrIdx - Given the specified address, check to see if it can be
  /// represented as an indexed [r+r] operation.
  /// This is for xform instructions whose associated displacement form is D.
  /// The last parameter \p 0 means associated D form has no requirment for 16
  /// bit signed displacement.
  /// Returns false if it can be represented by [r+imm], which are preferred.
  bool SelectAddrIdx(SDValue N, SDValue &Base, SDValue &Index) {
    return PPCLowering->SelectAddressRegReg(N, Base, Index, *CurDAG, None);
  }

  /// SelectAddrIdx4 - Given the specified address, check to see if it can be
  /// represented as an indexed [r+r] operation.
  /// This is for xform instructions whose associated displacement form is DS.
  /// The last parameter \p 4 means associated DS form 16 bit signed
  /// displacement must be a multiple of 4.
  /// Returns false if it can be represented by [r+imm], which are preferred.
  bool SelectAddrIdxX4(SDValue N, SDValue &Base, SDValue &Index) {
    return PPCLowering->SelectAddressRegReg(N, Base, Index, *CurDAG, Align(4));
  }

  /// SelectAddrIdx16 - Given the specified address, check to see if it can be
  /// represented as an indexed [r+r] operation.
  /// This is for xform instructions whose associated displacement form is DQ.
  /// The last parameter \p 16 means associated DQ form 16 bit signed
  /// displacement must be a multiple of 16.
  /// Returns false if it can be represented by [r+imm], which are preferred.
  bool SelectAddrIdxX16(SDValue N, SDValue &Base, SDValue &Index) {
    return PPCLowering->SelectAddressRegReg(N, Base, Index, *CurDAG, Align(16));
  }

  /// SelectAddrIdxOnly - Given the specified address, force it to be
  /// represented as an indexed [r+r] operation.
  bool SelectAddrIdxOnly(SDValue N, SDValue &Base, SDValue &Index) {
    return PPCLowering->SelectAddressRegRegOnly(N, Base, Index, *CurDAG);
  }

  /// SelectAddrImm - Returns true if the address N can be represented by
  /// a base register plus a signed 16-bit displacement [r+imm].
  /// The last parameter \p 0 means D form has no requirment for 16 bit signed
  /// displacement.
  bool SelectAddrImm(SDValue N, SDValue &Disp, SDValue &Base) {
    return PPCLowering->SelectAddressRegImm(N, Disp, Base, *CurDAG, None);
  }

  /// SelectAddrImmX4 - Returns true if the address N can be represented by
  /// a base register plus a signed 16-bit displacement that is a multiple of
  /// 4 (last parameter). Suitable for use by STD and friends.
  bool SelectAddrImmX4(SDValue N, SDValue &Disp, SDValue &Base) {
    return PPCLowering->SelectAddressRegImm(N, Disp, Base, *CurDAG, Align(4));
  }

  /// SelectAddrImmX16 - Returns true if the address N can be represented by
  /// a base register plus a signed 16-bit displacement that is a multiple of
  /// 16(last parameter). Suitable for use by STXV and friends.
  bool SelectAddrImmX16(SDValue N, SDValue &Disp, SDValue &Base) {
    return PPCLowering->SelectAddressRegImm(N, Disp, Base, *CurDAG, Align(16));
  }

  // Select an address into a single register.
  bool SelectAddr(SDValue N, SDValue &Base) {
    Base = N;
    return true;
  }

  bool SelectAddrPCRel(SDValue N, SDValue &Base) {
    return PPCLowering->SelectAddressPCRel(N, Base);
  }

  /// SelectInlineAsmMemoryOperand - Implement addressing mode selection for
  /// inline asm expressions.  It is always correct to compute the value into
  /// a register.  The case of adding a (possibly relocatable) constant to a
  /// register can be improved, but it is wrong to substitute Reg+Reg for
  /// Reg in an asm, because the load or store opcode would have to change.
  bool SelectInlineAsmMemoryOperand(const SDValue &Op, unsigned ConstraintID,
                                    std::vector<SDValue> &OutOps) override;

// Include the pieces autogenerated from the target description.
#include "PPCGenDAGISel.inc"

protected:
  bool trySETCC(SDNode *N);
  bool tryAsSingleRLDICL(SDNode *N);
  bool tryAsSingleRLDICR(SDNode *N);
  bool tryAsSingleRLWINM(SDNode *N);
  bool tryAsSingleRLWINM8(SDNode *N);
  bool tryAsSingleRLWIMI(SDNode *N);
  bool tryAsPairOfRLDICL(SDNode *N);
  bool tryAsSingleRLDIMI(SDNode *N);

  void PeepholePPC64();
  void PeepholePPC64ZExt();
  void PeepholeCROps();

  SDValue combineToCMPB(SDNode *N);
  void foldBoolExts(SDValue &Res, SDNode *&N);

  bool AllUsersSelectZero(SDNode *N);
  void SwapAllSelectUsers(SDNode *N);

  bool isOffsetMultipleOf(SDNode *N, unsigned Val) const;
  void transferMemOperands(SDNode *N, SDNode *Result);

  virtual const TargetLowering *getTargetLowering() const = 0;
};


class PPCDAGToDAGISel : public SelectionDAGISel, public virtual PPCDAGMatcher {
public:
  explicit PPCDAGToDAGISel(PPCTargetMachine &tm, CodeGenOpt::Level OptLevel);

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override {
    return "PowerPC DAG->DAG Pattern Instruction Selection";
  }

private:
  void Select(SDNode *N) override { PPCDAGMatcher::Select(N); }

  /// Return a reference to the TargetMachine, casted to the target-specific
  /// type.
  const TargetLowering *getTargetLowering() const override {
    return SelectionDAGISel::getTargetLowering();
  }
};

} // namespace llvm

#endif
