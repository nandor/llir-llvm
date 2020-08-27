//===-- LLIRFrameLowering.cpp - LLIR Frame Information --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LLIR implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "LLIRFrameLowering.h"

#include "LLIRInstrInfo.h"
#include "LLIRSubtarget.h"
#include "LLIRTargetMachine.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

LLIRFrameLowering::LLIRFrameLowering(const LLIRSubtarget &ST)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown,
                          /*StackAlignment=*/Align(16), /*LocalAreaOffset=*/0,
                          /*TransientStackAlignment=*/Align(16),
                          /*StackRealignable=*/true) {}

void LLIRFrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {}

void LLIRFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {}

MachineBasicBlock::iterator LLIRFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  llvm_unreachable("not implemented");
}

bool LLIRFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  llvm_unreachable("not implemented");
}

bool LLIRFrameLowering::hasFP(const MachineFunction &MF) const {
  // If frame pointer elimination is disabled, emit frame pointer.
  if (MF.getTarget().Options.DisableFramePointerElim(MF)) {
    return true;
  }

  const MachineFrameInfo &MFI = MF.getFrameInfo();
  return MFI.isFrameAddressTaken() || MFI.hasVarSizedObjects();
}

int LLIRFrameLowering::getFrameIndexReference(const MachineFunction &MF, int FI,
                                              Register &FrameReg) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  const TargetRegisterInfo *RI = MF.getSubtarget().getRegisterInfo();

  int Offset = MFI.getObjectOffset(FI) + MF.getFrameInfo().getStackSize();
  FrameReg = RI->getFrameRegister(MF);
  return Offset;
}
