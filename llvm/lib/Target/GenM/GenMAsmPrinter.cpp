//===-- GenMAsmPrinter.cpp - GenM LLVM assembly writer --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to GAS-format SPARC assembly language.
//
//===----------------------------------------------------------------------===//

#include "GenMAsmPrinter.h"
#include "GenM.h"
#include "GenMMCInstLower.h"
#include "GenMInstrInfo.h"
#include "GenMTargetMachine.h"
#include "MCTargetDesc/GenMMCTargetDesc.h"
#include "MCTargetDesc/GenMMCTargetStreamer.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfoImpls.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

void GenMAsmPrinter::EmitInstruction(const MachineInstr *MI)
{
  LLVM_DEBUG(dbgs() << "EmitInstruction: " << *MI << '\n');

  auto &MF = *MI->getParent()->getParent();

  GenMMCInstLower MCInstLowering(OutContext, MF, *this);
  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

void GenMAsmPrinter::EmitGlobalVariable(const GlobalVariable *GV)
{
  AsmPrinter::EmitGlobalVariable(GV);
  getTargetStreamer().emitEnd();
}

void GenMAsmPrinter::EmitFunctionBodyStart()
{
  MachineFrameInfo &MFI = MF->getFrameInfo();
  auto *FuncInfo = MF->getInfo<GenMMachineFunctionInfo>();
  auto &F = MF->getFunction();
  auto &Streamer = getTargetStreamer();
  auto &STI = MF->getSubtarget();

  if (auto StackSize = MFI.getStackSize()) {
    if (F.hasFnAttribute(Attribute::StackAlignment)) {
      Streamer.emitStackSize(StackSize, F.getFnStackAlignment());
    } else {
      Streamer.emitStackSize(StackSize, STI.getFrameLowering()->getStackAlignment());
    }
  }

  if (F.hasFnAttribute(Attribute::NoInline)) {
    Streamer.emitNoInline();
  }

  auto &Params = FuncInfo->getParams();
  bool IsVA = MF->getFunction().isVarArg();
  if (!Params.empty() || IsVA) {
    Streamer.emitParams(Params, IsVA);
  }
  Streamer.emitCallingConv(MF->getFunction().getCallingConv());

  AsmPrinter::EmitFunctionBodyStart();
}

bool GenMAsmPrinter::PrintAsmOperand(
    const MachineInstr *MI,
    unsigned OpNo,
    unsigned AsmVariant,
    const char *ExtraCode,
    raw_ostream &O)
{
  return AsmPrinter::PrintAsmMemoryOperand(MI, OpNo, AsmVariant, ExtraCode, O);
}

bool GenMAsmPrinter::PrintAsmMemoryOperand(
    const MachineInstr *MI,
    unsigned OpNo,
    unsigned AsmVariant,
    const char *ExtraCode,
    raw_ostream &O)
{
  return AsmPrinter::PrintAsmMemoryOperand(MI, OpNo, AsmVariant, ExtraCode, O);
}

GenMMCTargetStreamer &GenMAsmPrinter::getTargetStreamer()
{
  return static_cast<GenMMCTargetStreamer &>(*OutStreamer->getTargetStreamer());
}


// Force static initialization.
extern "C" void LLVMInitializeGenMAsmPrinter()
{
  RegisterAsmPrinter<GenMAsmPrinter> X(getTheGenMTarget());
}
