//===-- LLIRRegisterInfo.cpp - SPARC Register Information -----------------===//
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

#include "LLIRRegisterInfo.h"
#include "LLIR.h"
#include "LLIRSubtarget.h"
#include "MCTargetDesc/LLIRMCTargetDesc.h"
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
#include "LLIRGenRegisterInfo.inc"

LLIRRegisterInfo::LLIRRegisterInfo()
  : LLIRGenRegisterInfo(0)
{
}

const MCPhysReg *
LLIRRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const
{
  static const MCPhysReg CalleeSavedRegs[] = {0};
  return CalleeSavedRegs;
}

BitVector LLIRRegisterInfo::getReservedRegs(const MachineFunction &MF) const
{
  BitVector Reserved(getNumRegs());
  Reserved.set(LLIR::RSP);
  Reserved.set(LLIR::FRAME_ADDR);
  return Reserved;
}

void LLIRRegisterInfo::eliminateFrameIndex(
    MachineBasicBlock::iterator II,
    int SPAdj,
    unsigned FIOperandNum,
    RegScavenger *RS) const
{
  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();

  const int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  const LLIRInstrInfo *TII = MF.getSubtarget<LLIRSubtarget>().getInstrInfo();
  DebugLoc DL = MI.getDebugLoc();

  unsigned TempReg = MRI.createVirtualRegister(&LLIR::I64RegClass);
  BuildMI(MBB, II, DL, TII->get(LLIR::FRAME_I64), TempReg)
      .addImm(FrameIndex)
      .addImm(0ull);
  MI.getOperand(FIOperandNum).ChangeToRegister(TempReg, false);
}

Register LLIRRegisterInfo::getFrameRegister(const MachineFunction &MF) const
{
  return LLIR::FRAME_ADDR;
}

const TargetRegisterClass *LLIRRegisterInfo::getPointerRegClass(
    const MachineFunction &MF,
    unsigned Kind) const
{
  llvm_unreachable("getPointerRegClass");
}
