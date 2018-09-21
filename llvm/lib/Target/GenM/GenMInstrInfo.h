//===-- GenMInstrInfo.h - GenM Instruction Information ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the GenM implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_GENMINSTRINFO_H
#define LLVM_LIB_TARGET_GENM_GENMINSTRINFO_H

#include "GenMRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "GenMGenInstrInfo.inc"

namespace llvm {

class GenMSubtarget;

class GenMInstrInfo : public GenMGenInstrInfo {
public:
  explicit GenMInstrInfo(GenMSubtarget &ST);

  unsigned isLoadFromStackSlot(
      const MachineInstr &MI,
      int &FrameIndex
  ) const override;

  unsigned isStoreToStackSlot(
      const MachineInstr &MI,
      int &FrameIndex
  ) const override;

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

  // Lower pseudo instructions after register allocation.
  bool expandPostRAPseudo(MachineInstr &MI) const override;
};

}

#endif
