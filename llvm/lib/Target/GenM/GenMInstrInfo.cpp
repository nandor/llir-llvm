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
  for (MachineInstr &MI : MBB.terminators()) {
    switch (MI.getOpcode()) {
      default: {
        return true;
      }
    }
  }
  return false;
}

unsigned GenMInstrInfo::removeBranch(
    MachineBasicBlock &MBB,
    int *BytesRemoved) const
{
  assert(!"not implemented");
}

unsigned GenMInstrInfo::insertBranch(
    MachineBasicBlock &MBB,
    MachineBasicBlock *TBB,
    MachineBasicBlock *FBB,
    ArrayRef<MachineOperand> Cond,
    const DebugLoc &DL,
    int *BytesAdded) const
{
  assert(!"not implemented");
}

bool GenMInstrInfo::reverseBranchCondition(
    SmallVectorImpl<MachineOperand> &Cond) const
{
  assert(!"not implemented");
}

void GenMInstrInfo::copyPhysReg(
    MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I,
    const DebugLoc &DL,
    unsigned DestReg,
    unsigned SrcReg,
    bool KillSrc) const
{
  assert(!"not implemented");
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
  assert(!"not implemented");
}

void GenMInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB,
    MachineBasicBlock::iterator MBBI,
    unsigned DestReg,
    int FrameIndex,
    const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI) const
{
  assert(!"not implemented");
}

// Lower pseudo instructions after register allocation.
bool GenMInstrInfo::expandPostRAPseudo(MachineInstr &MI) const
{
  assert(!"not implemented");
}
