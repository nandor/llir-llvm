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
  // Optimise only when there a single condition, followed by jumps.
  bool HaveCond = false;
  for (MachineInstr &MI : MBB.terminators()) {
    switch (MI.getOpcode()) {
    default:
      return true;
    case GenM::JT:
      if (HaveCond) {
        return true;
      }
      Cond.push_back(MachineOperand::CreateImm(true));
      Cond.push_back(MI.getOperand(0));
      TBB = MI.getOperand(1).getMBB();
      HaveCond = true;
      break;
    case GenM::JF:
      if (HaveCond) {
        return true;
      }
      Cond.push_back(MachineOperand::CreateImm(false));
      Cond.push_back(MI.getOperand(0));
      TBB = MI.getOperand(1).getMBB();
      HaveCond = true;
      break;
    case GenM::JMP:
      if (!HaveCond) {
        TBB = MI.getOperand(0).getMBB();
      } else {
        FBB = MI.getOperand(0).getMBB();
      }
      break;
    }
  }

  return false;
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
    llvm::errs() << *I;
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
      if (Cond[0].getImm()) {
        BuildMI(&MBB, DL, get(GenM::JT)).add(Cond[1]).addMBB(TBB);
      } else {
        BuildMI(&MBB, DL, get(GenM::JF)).add(Cond[1]).addMBB(TBB);
      }
      if (FBB) {
        BuildMI(&MBB, DL, get(GenM::JMP)).addMBB(FBB);
        return 2;
      } else {
        return 1;
      }
    }
    default: {
      llvm_unreachable("invalid condition");
    }
  }
}

bool GenMInstrInfo::reverseBranchCondition(
    SmallVectorImpl<MachineOperand> &Cond) const
{
  assert(Cond.size() == 2 && "Expected a flag and succesor.");
  Cond.front() = MachineOperand::CreateImm(!Cond.front().getImm());
  return false;
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

  unsigned Op;
  if (TargetRegisterInfo::isVirtualRegister(SrcReg)) {
    if (TargetRegisterInfo::isVirtualRegister(DstReg)) {
      const TargetRegisterClass *DstCls = MRI.getRegClass(DstReg);
      // virt -> virt
      if (&GenM::I32RegClass == DstCls) {
        Op = GenM::MOV_I32;
      } else if (&GenM::I64RegClass == DstCls) {
        Op = GenM::MOV_I64;
      } else if (&GenM::F32RegClass == DstCls) {
        Op = GenM::MOV_F32;
      } else if (&GenM::F64RegClass == DstCls) {
        Op = GenM::MOV_F64;
      } else if (&GenM::F80RegClass == DstCls) {
        Op = GenM::MOV_F80;
      } else {
        llvm_unreachable("copy kind not supported");
      }
    } else {
      // virt -> phys
      Op = GenM::SET_I64;
    }
  } else {
    if (TargetRegisterInfo::isVirtualRegister(DstReg)) {
      // phys -> virt
      Op = GenM::GET_I64;
    } else {
      // phys -> phys
      llvm_unreachable("phys-phys copy not supported");
    }
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
