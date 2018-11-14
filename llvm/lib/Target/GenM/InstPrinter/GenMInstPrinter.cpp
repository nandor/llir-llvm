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

void GenMInstPrinter::printCallOps(
    llvm::raw_ostream &OS,
    const MCInst *MI,
    const MCSubtargetInfo &STI)
{
  printCallingConv(OS, MI->getOperand(1).getImm());
  OS << ", ";
  printOperand(MI, 0, STI, OS);
  for (unsigned i = 2; i < MI->getNumOperands(); i += 1) {
    OS << ", ";
    printOperand(MI, i, STI, OS);
  }
  return;
}

void GenMInstPrinter::printVoidOps(
    llvm::raw_ostream &OS,
    const MCInst *MI,
    const MCSubtargetInfo &STI)
{
  printCallingConv(OS, MI->getOperand(0).getImm());
  for (unsigned i = 1; i < MI->getNumOperands(); i += 1) {
    OS << ", ";
    printOperand(MI, i, STI, OS);
  }
  return;
}

void GenMInstPrinter::printInst(
    const MCInst *MI,
    raw_ostream &OS,
    StringRef Annot,
    const MCSubtargetInfo &STI)
{
  switch (auto Op = MI->getOpcode()) {
    case GenM::CALL_I32:     OS << "\tcall.i32   \t"; printCallOps(OS, MI, STI); break;
    case GenM::CALL_I32_VA:  OS << "\tcall_va.i32\t"; printCallOps(OS, MI, STI); break;
    case GenM::CALL_F32:     OS << "\tcall.f32   \t"; printCallOps(OS, MI, STI); break;
    case GenM::CALL_F32_VA:  OS << "\tcall_va.f32\t"; printCallOps(OS, MI, STI); break;
    case GenM::CALL_I64:     OS << "\tcall.i64   \t"; printCallOps(OS, MI, STI); break;
    case GenM::CALL_I64_VA:  OS << "\tcall_va.i64\t"; printCallOps(OS, MI, STI); break;
    case GenM::CALL_F64:     OS << "\tcall.f64   \t"; printCallOps(OS, MI, STI); break;
    case GenM::CALL_F64_VA:  OS << "\tcall_va.f64\t"; printCallOps(OS, MI, STI); break;
    case GenM::TCALL:        OS << "\ttcall      \t"; printVoidOps(OS, MI, STI); break;
    case GenM::TCALL_VA:     OS << "\ttcall_va   \t"; printVoidOps(OS, MI, STI); break;
    case GenM::CALL_VOID:    OS << "\tcall       \t"; printVoidOps(OS, MI, STI); break;
    case GenM::CALL_VOID_VA: OS << "\tcall_va    \t"; printVoidOps(OS, MI, STI); break;

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
