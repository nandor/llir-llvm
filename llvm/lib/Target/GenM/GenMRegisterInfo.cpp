//===-- GenMRegisterInfo.cpp - SPARC Register Information -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the SPARC implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "GenMRegisterInfo.h"
#include "GenM.h"
#include "GenMSubtarget.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "GenMGenRegisterInfo.inc"

GenMRegisterInfo::GenMRegisterInfo()
  : GenMGenRegisterInfo(0)
{
}

const MCPhysReg *
GenMRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const
{
  static const MCPhysReg CalleeSavedRegs[] = {0};
  return CalleeSavedRegs;
}

const uint32_t *GenMRegisterInfo::getCallPreservedMask(
    const MachineFunction &MF,
    CallingConv::ID CC) const
{
  assert(!"not implemented");
}

BitVector GenMRegisterInfo::getReservedRegs(const MachineFunction &MF) const
{
  BitVector Reserved(getNumRegs());
  for (auto Reg : { GenM::PC, GenM::SP }) {
    Reserved.set(Reg);
  }
  return Reserved;
}

const TargetRegisterClass *GenMRegisterInfo::getPointerRegClass(
    const MachineFunction &MF,
    unsigned Kind) const
{
  assert(!"not implemented");
}

bool GenMRegisterInfo::enableMultipleCopyHints() const
{
  assert(!"not implemented");
}

void GenMRegisterInfo::eliminateFrameIndex(
    MachineBasicBlock::iterator II,
    int SPAdj,
    unsigned FIOperandNum,
    RegScavenger *RS) const
{
  assert(!"not implemented");
}

unsigned GenMRegisterInfo::getFrameRegister(const MachineFunction &MF) const
{
  assert(!"not implemented");
}

bool GenMRegisterInfo::canRealignStack(const MachineFunction &MF) const
{
  assert(!"not implemented");
}
