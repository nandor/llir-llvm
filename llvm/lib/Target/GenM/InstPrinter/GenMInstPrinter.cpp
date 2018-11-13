//===-- GenMInstPrinter.cpp - Convert GenM MCInst to assembly syntax -----==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an GenM MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "GenMInstPrinter.h"
#include "GenM.h"
#include "MCTargetDesc/GenMMCTargetDesc.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define GET_INSTRUCTION_NAME
#define PRINT_ALIAS_INSTR
#include "GenMGenAsmWriter.inc"

GenMInstPrinter::GenMInstPrinter(
    const MCAsmInfo &MAI,
    const MCInstrInfo &MII,
    const MCRegisterInfo &MRI)
  : MCInstPrinter(MAI, MII, MRI)
{
}

void GenMInstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const
{
  switch (RegNo) {
    case GenM::NUM_TARGET_REGS: {
      OS << "$undef";
      break;
    }
    case GenM::SP: OS << "$sp"; break;
    case GenM::FP: OS << "$fp"; break;
    default: {
      assert(RegNo >= GenM::NUM_TARGET_REGS);
      OS << "$" << (RegNo - GenM::NUM_TARGET_REGS);
      break;
    }
  }
}

void GenMInstPrinter::printInst(
    const MCInst *MI,
    raw_ostream &OS,
    StringRef Annot,
    const MCSubtargetInfo &STI)
{
  switch (auto Op = MI->getOpcode()) {
    case GenM::CALL_I32:
    case GenM::CALL_I64: {
      OS << '\t' << "call." << (Op == GenM::CALL_I32 ? "i32" : "i64");
      OS << '\t';
      printOperand(MI, 0, STI, OS);
      OS << ", ";
      printOperand(MI, 1, STI, OS);
      for (unsigned i = 2; i < MI->getNumOperands(); i += 1) {
        OS << ", ";
        printOperand(MI, i, STI, OS);
      }
      break;
    }
    case GenM::TCALL: {
      OS << '\t' << "tcall";
      OS << '\t';
      printOperand(MI, 0, STI, OS);
      for (unsigned i = 1; i < MI->getNumOperands(); i += 1) {
        OS << ", ";
        printOperand(MI, i, STI, OS);
      }
      break;
    }
    case GenM::CALL_VOID: {
      OS << '\t' << "call";
      OS << '\t';
      printOperand(MI, 0, STI, OS);
      for (unsigned i = 1; i < MI->getNumOperands(); i += 1) {
        OS << ", ";
        printOperand(MI, i, STI, OS);
      }
      break;
    }
    default: {
      printInstruction(MI, STI, OS);
      break;
    }
  }
}


void GenMInstPrinter::printOperand(
    const MCInst *MI,
    int opNum,
    const MCSubtargetInfo &STI,
    raw_ostream &OS)
{
  const MCOperand &MO = MI->getOperand(opNum);

  if (MO.isReg()) {
    printRegName(OS, MO.getReg());
    return;
  }

  if (MO.isImm()) {
    OS << MO.getImm();
    return;
  }

  if (MO.isExpr()) {
    MO.getExpr()->print(OS, &MAI);
    return;
  }

  if (MO.isFPImm()) {
    OS << MO.getFPImm();
    return;
  }

  assert(false && "Unknown operand kind");
}
