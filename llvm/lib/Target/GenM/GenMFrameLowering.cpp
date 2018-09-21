//===-- GenMFrameLowering.cpp - GenM Frame Information --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the GenM implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "GenMFrameLowering.h"
#include "GenMInstrInfo.h"
#include "GenMSubtarget.h"
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


GenMFrameLowering::GenMFrameLowering(const GenMSubtarget &ST)
    : TargetFrameLowering(
          TargetFrameLowering::StackGrowsDown,
          16,
          0,
          16
      )
{
}

void GenMFrameLowering::emitPrologue(
    MachineFunction &MF,
    MachineBasicBlock &MBB) const
{
  assert(!"not implemented");
}

MachineBasicBlock::iterator
GenMFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF,
    MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const
{
  assert(!"not implemented");
}


void GenMFrameLowering::emitEpilogue(
    MachineFunction &MF,
    MachineBasicBlock &MBB) const
{
  assert(!"not implemented");
}

bool GenMFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const
{
  assert(!"not implemented");
}

bool GenMFrameLowering::hasFP(const MachineFunction &MF) const
{
  assert(!"not implemented");
}

void GenMFrameLowering::determineCalleeSaves(
    MachineFunction &MF,
    BitVector &SavedRegs,
    RegScavenger *RS) const
{
  assert(!"not implemented");
}

int GenMFrameLowering::getFrameIndexReference(
    const MachineFunction &MF,
    int FI,
    unsigned &FrameReg) const
{
  assert(!"not implemented");
}
