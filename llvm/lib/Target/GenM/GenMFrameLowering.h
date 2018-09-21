//===-- GenMFrameLowering.h - Define frame lowering for GenM ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_GENMFRAMELOWERING_H
#define LLVM_LIB_TARGET_GENM_GENMFRAMELOWERING_H

#include "GenM.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class GenMSubtarget;

class GenMFrameLowering : public TargetFrameLowering {
public:
  explicit GenMFrameLowering(const GenMSubtarget &ST);

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  MachineBasicBlock::iterator eliminateCallFramePseudoInstr(
      MachineFunction &MF,
      MachineBasicBlock &MBB,
      MachineBasicBlock::iterator I
  ) const override;

  bool hasReservedCallFrame(const MachineFunction &MF) const override;

  bool hasFP(const MachineFunction &MF) const override;

  void determineCalleeSaves(
      MachineFunction &MF,
      BitVector &SavedRegs,
      RegScavenger *RS = nullptr
  ) const override;

  int getFrameIndexReference(
      const MachineFunction &MF,
      int FI,
      unsigned &FrameReg
  ) const override;

  bool targetHandlesStackFrameRounding() const override { return true; }
};

} // End llvm namespace

#endif
