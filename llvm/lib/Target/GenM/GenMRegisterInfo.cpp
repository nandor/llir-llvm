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
#include "MCTargetDesc/GenMMCTargetDesc.h"
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

BitVector GenMRegisterInfo::getReservedRegs(const MachineFunction &MF) const
{
  BitVector Reserved(getNumRegs());
  for (auto Reg : { GenM::PC, GenM::SP }) {
    Reserved.set(Reg);
  }
  return Reserved;
}

void GenMRegisterInfo::eliminateFrameIndex(
    MachineBasicBlock::iterator II,
    int SPAdj,
    unsigned FIOperandNum,
    RegScavenger *RS) const
{
  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  const TargetFrameLowering &TFL = *getFrameLowering(MF);

  const int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  unsigned FrameReg;
  int Offset = TFL.getFrameIndexReference(MF, FrameIndex, FrameReg);

  if (!isInt<32>(Offset)) {
    report_fatal_error("Not a 32-bit offset");
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false, false, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
}

unsigned GenMRegisterInfo::getFrameRegister(const MachineFunction &MF) const
{
  if (getFrameLowering(MF)->hasFP(MF)) {
    return GenM::FP;
  } else {
    return GenM::SP;
  }
}

const TargetRegisterClass *GenMRegisterInfo::getPointerRegClass(
    const MachineFunction &MF,
    unsigned Kind) const
{
  llvm_unreachable("not implemented");
}
