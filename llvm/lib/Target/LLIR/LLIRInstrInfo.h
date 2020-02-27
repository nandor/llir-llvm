//===-- LLIRInstrInfo.h - LLIR Instruction Information ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LLIR implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIRINSTRINFO_H
#define LLVM_LIB_TARGET_LLIR_LLIRINSTRINFO_H

#include "LLIRRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "LLIRGenInstrInfo.inc"

namespace llvm {

class LLIRSubtarget;

class LLIRInstrInfo : public LLIRGenInstrInfo {
public:
  explicit LLIRInstrInfo(LLIRSubtarget &ST);

  bool analyzeBranch(
      MachineBasicBlock &MBB,
      MachineBasicBlock *&TBB,
      MachineBasicBlock *&FBB,
      SmallVectorImpl<MachineOperand> &Cond,
      bool AllowModify = false
  ) const override;

  unsigned removeBranch(
      MachineBasicBlock &MBB,
      int *BytesRemoved = nullptr
  ) const override;

  unsigned insertBranch(
      MachineBasicBlock &MBB,
      MachineBasicBlock *TBB,
      MachineBasicBlock *FBB,
      ArrayRef<MachineOperand> Cond,
      const DebugLoc &DL,
      int *BytesAdded = nullptr
  ) const override;

  bool reverseBranchCondition(
      SmallVectorImpl<MachineOperand> &Cond
  ) const override;

  void copyPhysReg(
      MachineBasicBlock &MBB,
      MachineBasicBlock::iterator I,
      const DebugLoc &DL,
      unsigned DestReg,
      unsigned SrcReg,
      bool KillSrc
  ) const override;

  void storeRegToStackSlot(
      MachineBasicBlock &MBB,
      MachineBasicBlock::iterator MBBI,
      unsigned SrcReg,
      bool isKill,
      int FrameIndex,
      const TargetRegisterClass *RC,
      const TargetRegisterInfo *TRI
  ) const override;

  void loadRegFromStackSlot(
      MachineBasicBlock &MBB,
      MachineBasicBlock::iterator MBBI,
      unsigned DestReg,
      int FrameIndex,
      const TargetRegisterClass *RC,
      const TargetRegisterInfo *TRI
  ) const override;
};

}

#endif
