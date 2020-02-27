//===-- LLIRRegisterInfo.h - LLIR Register Information Impl -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LLIR implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIRREGISTERINFO_H
#define LLVM_LIB_TARGET_LLIR_LLIRREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "LLIRGenRegisterInfo.inc"

namespace llvm {
struct LLIRRegisterInfo : public LLIRGenRegisterInfo {
  LLIRRegisterInfo();

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  void eliminateFrameIndex(
      MachineBasicBlock::iterator II,
      int SPAdj,
      unsigned FIOperandNum,
      RegScavenger *RS = nullptr
  ) const override;

  unsigned getFrameRegister(const MachineFunction &MF) const override;

  const TargetRegisterClass *getPointerRegClass(
      const MachineFunction &MF,
      unsigned Kind = 0
  ) const override;

  const uint32_t *getNoPreservedMask() const override { return nullptr; }
};

} // end namespace llvm

#endif
