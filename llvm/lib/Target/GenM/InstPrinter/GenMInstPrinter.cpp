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
#include "llvm/IR/CallingConv.h"
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
    case GenM::VA: OS << "$va"; break;
    default: {
      assert(RegNo >= GenM::NUM_TARGET_REGS);
      OS << "$" << (RegNo - GenM::NUM_TARGET_REGS);
      break;
    }
  }
}

static void printCallingConv(llvm::raw_ostream &OS, unsigned CallConv)
{
  switch (static_cast<CallingConv::ID>(CallConv)) {
    case CallingConv::C:    OS << "c"; break;
    case CallingConv::Fast: OS << "fast"; break;
    default: llvm_unreachable("not implemented");
  }
}

void GenMInstPrinter::printCall(
    const char *Op,
    llvm::raw_ostream &OS,
    const MCInst *MI,
    const MCSubtargetInfo &STI,
    bool isVoid,
    bool isVA)
{
  unsigned start = isVoid ? 0 : 1;
  if (isVA) {
    OS << '\t' << Op << '.' << MI->getOperand(start++).getImm() << '\t';
  } else {
    OS << '\t' << Op << '\t';
  }

  if (!isVoid) {
    printOperand(MI, 0, STI, OS);
    OS << ", ";
  }
  printCallingConv(OS, MI->getOperand(start++).getImm());

  for (unsigned i = start; i < MI->getNumOperands(); ++i) {
    OS << ", ";
    printOperand(MI, i, STI, OS);
  }
}

void GenMInstPrinter::printInst(
    const MCInst *MI,
    raw_ostream &OS,
    StringRef Annot,
    const MCSubtargetInfo &STI)
{
  switch (auto Op = MI->getOpcode()) {
    case GenM::CALL_I32:     printCall("call.i32", OS, MI, STI, 0, 0); break;
    case GenM::CALL_I32_VA:  printCall("call.i32", OS, MI, STI, 0, 1); break;
    case GenM::CALL_F32:     printCall("call.f32", OS, MI, STI, 0, 0); break;
    case GenM::CALL_F32_VA:  printCall("call.f32", OS, MI, STI, 0, 1); break;
    case GenM::CALL_I64:     printCall("call.i64", OS, MI, STI, 0, 0); break;
    case GenM::CALL_I64_VA:  printCall("call.i64", OS, MI, STI, 0, 1); break;
    case GenM::CALL_F64:     printCall("call.f64", OS, MI, STI, 0, 0); break;
    case GenM::CALL_F64_VA:  printCall("call.f64", OS, MI, STI, 0, 1); break;
    case GenM::TCALL:        printCall("tcall",    OS, MI, STI, 1, 0); break;
    case GenM::TCALL_VA:     printCall("tcall",    OS, MI, STI, 1, 1); break;
    case GenM::CALL_VOID:    printCall("call",     OS, MI, STI, 1, 0); break;
    case GenM::CALL_VOID_VA: printCall("call",     OS, MI, STI, 1, 1); break;

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
