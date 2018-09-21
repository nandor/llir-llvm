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

void GenMInstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const
{
  assert(!"not implemented");
}

void GenMInstPrinter::printInst(
    const MCInst *MI,
    raw_ostream &O,
    StringRef Annot,
    const MCSubtargetInfo &STI)
{
  assert(!"not implemented");
}


void GenMInstPrinter::printOperand(
    const MCInst *MI,
    int opNum,
    const MCSubtargetInfo &STI,
    raw_ostream &OS)
{
  assert(!"not implemented");
}
