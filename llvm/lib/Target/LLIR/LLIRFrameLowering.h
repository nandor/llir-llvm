//===-- LLIRFrameLowering.h - Define frame lowering for LLIR ----*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_LLIR_LLIRFRAMELOWERING_H
#define LLVM_LIB_TARGET_LLIR_LLIRFRAMELOWERING_H

#include "LLIR.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class LLIRSubtarget;

class LLIRFrameLowering : public TargetFrameLowering {
 public:
  explicit LLIRFrameLowering(const LLIRSubtarget &ST);

  MachineBasicBlock::iterator eliminateCallFramePseudoInstr(
      MachineFunction &MF, MachineBasicBlock &MBB,
      MachineBasicBlock::iterator I) const override;

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasFP(const MachineFunction &MF) const override;
  bool hasReservedCallFrame(const MachineFunction &MF) const override;

  int getFrameIndexReference(const MachineFunction &MF, int FI,
                             Register &FrameReg) const override;

  bool targetHandlesStackFrameRounding() const override { return true; }
};

}  // namespace llvm

#endif
