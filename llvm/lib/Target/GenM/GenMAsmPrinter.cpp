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
#include "MCTargetDesc/GenMMCTargetStreamer.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfoImpls.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
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

  switch (MI->getOpcode()) {
    case GenM::ARG_I32:
    case GenM::ARG_I64:
      break;
    default: {
      GenMMCInstLower MCInstLowering(OutContext, *this);
      MCInst TmpInst;
      MCInstLowering.Lower(MI, TmpInst);
      EmitToStreamer(*OutStreamer, TmpInst);
    }
  }
}

void GenMAsmPrinter::EmitFunctionBodyStart()
{
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
