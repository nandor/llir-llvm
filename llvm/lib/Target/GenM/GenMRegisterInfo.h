//===-- GenMRegisterInfo.h - GenM Register Information Impl -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the GenM implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_GENMREGISTERINFO_H
#define LLVM_LIB_TARGET_GENM_GENMREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "GenMGenRegisterInfo.inc"

namespace llvm {
struct GenMRegisterInfo : public GenMGenRegisterInfo {
  GenMRegisterInfo();

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;
  const uint32_t *getCallPreservedMask(
      const MachineFunction &MF,
      CallingConv::ID CC
  ) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  const TargetRegisterClass *getPointerRegClass(
      const MachineFunction &MF,
      unsigned Kind
  ) const override;

  bool enableMultipleCopyHints() const override;

  void eliminateFrameIndex(
      MachineBasicBlock::iterator II,
      int SPAdj,
      unsigned FIOperandNum,
      RegScavenger *RS = nullptr
  ) const override;

  unsigned getFrameRegister(const MachineFunction &MF) const override;

  bool canRealignStack(const MachineFunction &MF) const override;

};

} // end namespace llvm

#endif
