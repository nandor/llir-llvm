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
  for (auto Reg : { GenM::FP, GenM::SP }) {
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

  const TargetFrameLowering &TFL = *getFrameLowering(MF);
  const int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  const GenMInstrInfo *TII = MF.getSubtarget<GenMSubtarget>().getInstrInfo();
  DebugLoc DL = MI.getDebugLoc();

  unsigned FrameReg, StackReg;
  if (int Offset = TFL.getFrameIndexReference(MF, FrameIndex, FrameReg)) {
    unsigned TempReg = MRI.createVirtualRegister(&GenM::I64RegClass);
    BuildMI(MBB, II, DL, TII->get(GenM::COPY_I64), TempReg)
        .addReg(FrameReg);

    unsigned ConstReg = MRI.createVirtualRegister(&GenM::I64RegClass);
    BuildMI(MBB, II, DL, TII->get(GenM::CONST_I64), ConstReg)
        .addImm(Offset);

    unsigned AddrReg = MRI.createVirtualRegister(&GenM::I64RegClass);
    BuildMI(MBB, II, DL, TII->get(GenM::ADD_I64), AddrReg)
        .addReg(TempReg)
        .addReg(ConstReg);

    StackReg = AddrReg;
  } else {
    unsigned TempReg = MRI.createVirtualRegister(&GenM::I64RegClass);
    BuildMI(MBB, II, DL, TII->get(GenM::COPY_I64), TempReg)
        .addReg(FrameReg);

    StackReg = TempReg;
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(StackReg, false);
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
  llvm_unreachable("getPointerRegClass");
}
