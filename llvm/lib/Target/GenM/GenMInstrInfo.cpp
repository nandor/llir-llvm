//===-- GenMInstrInfo.cpp - GenM Instruction Information ----------------===//
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

#include "GenMInstrInfo.h"
#include "GenM.h"
#include "GenMSubtarget.h"
#include "MCTargetDesc/GenMMCTargetDesc.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "GenMGenInstrInfo.inc"

GenMInstrInfo::GenMInstrInfo(GenMSubtarget &ST)
  : GenMGenInstrInfo(-1, -1, -1, -1)
{
}

bool GenMInstrInfo::analyzeBranch(
    MachineBasicBlock &MBB,
    MachineBasicBlock *&TBB,
    MachineBasicBlock *&FBB,
    SmallVectorImpl<MachineOperand> &Cond,
    bool AllowModify) const
{
  return true;
}

unsigned GenMInstrInfo::removeBranch(
    MachineBasicBlock &MBB,
    int *BytesRemoved) const
{
  MachineBasicBlock::instr_iterator I = MBB.instr_end();
  unsigned Count = 0;

  while (I != MBB.instr_begin()) {
    --I;
    if (I->isDebugInstr())
      continue;
    if (!I->isTerminator())
      break;
    // Remove the branch.
    I->eraseFromParent();
    I = MBB.instr_end();
    ++Count;
  }

  return Count;
}

unsigned GenMInstrInfo::insertBranch(
    MachineBasicBlock &MBB,
    MachineBasicBlock *TBB,
    MachineBasicBlock *FBB,
    ArrayRef<MachineOperand> Cond,
    const DebugLoc &DL,
    int *BytesAdded) const
{
  switch (Cond.size()) {
    case 0: {
      if (TBB) {
        BuildMI(&MBB, DL, get(GenM::JMP)).addMBB(TBB);
        return 1;
      } else {
        return 0;
      }
    }
    case 2: {
      llvm_unreachable("not implemented");
    }
    default: {
      llvm_unreachable("invalid condition");
    }
  }
}

bool GenMInstrInfo::reverseBranchCondition(
    SmallVectorImpl<MachineOperand> &Cond) const
{
  llvm_unreachable("not implemented");
}

void GenMInstrInfo::copyPhysReg(
    MachineBasicBlock &MBB,
    MachineBasicBlock::iterator MBBI,
    const DebugLoc &DL,
    unsigned DstReg,
    unsigned SrcReg,
    bool KillSrc) const
{
  auto &MRI = MBB.getParent()->getRegInfo();
  const TargetRegisterClass *DstCls = MRI.getRegClass(DstReg);
  const TargetRegisterClass *SrcCls = MRI.getRegClass(SrcReg);
  if (SrcCls != DstCls) {
    llvm_unreachable("cannot copy registers");
  }

  unsigned Op;
  if (&GenM::I32RegClass == DstCls) {
    Op = GenM::COPY_I32;
  } else if (&GenM::I64RegClass == SrcCls) {
    Op = GenM::COPY_I64;
  } else {
    llvm_unreachable("cannot copy physical registers");
  }

  BuildMI(MBB, MBBI, DL, get(Op), DstReg)
      .addReg(SrcReg, getKillRegState(KillSrc));
}

void GenMInstrInfo::storeRegToStackSlot(
    MachineBasicBlock &MBB,
    MachineBasicBlock::iterator MBBI,
    unsigned SrcReg,
    bool isKill,
    int FrameIndex,
    const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI) const
{
  llvm_unreachable("not implemented");
}

void GenMInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB,
    MachineBasicBlock::iterator MBBI,
    unsigned DestReg,
    int FrameIndex,
    const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI) const
{
  llvm_unreachable("not implemented");
}
