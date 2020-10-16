//===-- LLIRInstPrinter.cpp - Convert LLIR MCInst to assembly syntax -----==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an LLIR MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "LLIRInstPrinter.h"

#include "LLIR.h"
#include "MCTargetDesc/LLIRMCTargetDesc.h"
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
#include "LLIRGenAsmWriter.inc"

LLIRInstPrinter::LLIRInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                                 const MCRegisterInfo &MRI)
    : MCInstPrinter(MAI, MII, MRI) {}

void LLIRInstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const {
  switch (RegNo) {
    case LLIR::RSP: {
      OS << "$sp";
      break;
    }
    case LLIR::FRAME_ADDR: {
      OS << "$frame_address";
      break;
    }
    default: {
      assert(RegNo >= LLIR::NUM_TARGET_REGS);
      OS << "$" << (RegNo - LLIR::NUM_TARGET_REGS);
      break;
    }
  }
}

static void printCallingConv(llvm::raw_ostream &OS, unsigned CallConv) {
  switch (static_cast<CallingConv::ID>(CallConv)) {
    case CallingConv::C:
      OS << "c";
      break;
    case CallingConv::Fast:
      OS << "c";
      break;
    case CallingConv::LLIR_SETJMP:
      OS << "setjmp";
      break;
    default:
      llvm_unreachable("not implemented");
  }
}

void LLIRInstPrinter::printCall(const char *Op, llvm::raw_ostream &OS,
                                const MCInst *MI, const MCSubtargetInfo &STI,
                                bool isVoid, bool isVA, bool isTail) {
  unsigned start = isVoid ? 0 : 1;
  if (isVA) {
    OS << '\t' << Op << '.' << MI->getOperand(start++).getImm() << '.';
  } else {
    OS << '\t' << Op << '.';
  }
  printCallingConv(OS, MI->getOperand(start++).getImm());
  OS << '\t';

  if (!isVoid && !isTail) {
    printOperand(MI, 0, STI, OS);
  }

  for (unsigned i = start; i < MI->getNumOperands(); ++i) {
    if ((!isVoid && !isTail) || i != start) {
      OS << ", ";
    }
    printOperand(MI, i, STI, OS);
  }
}

void LLIRInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                StringRef Annot, const MCSubtargetInfo &STI,
                                raw_ostream &OS) {
  switch (MI->getOpcode()) {
    case LLIR::CALL_I8:
      printCall("call.i8", OS, MI, STI, 0, 0, 0);
      break;
    case LLIR::CALL_I8_VA:
      printCall("call.i8", OS, MI, STI, 0, 1, 0);
      break;
    case LLIR::CALL_I16:
      printCall("call.i16", OS, MI, STI, 0, 0, 0);
      break;
    case LLIR::CALL_I16_VA:
      printCall("call.i16", OS, MI, STI, 0, 1, 0);
      break;
    case LLIR::CALL_I32:
      printCall("call.i32", OS, MI, STI, 0, 0, 0);
      break;
    case LLIR::CALL_I32_VA:
      printCall("call.i32", OS, MI, STI, 0, 1, 0);
      break;
    case LLIR::CALL_F32:
      printCall("call.f32", OS, MI, STI, 0, 0, 0);
      break;
    case LLIR::CALL_F32_VA:
      printCall("call.f32", OS, MI, STI, 0, 1, 0);
      break;
    case LLIR::CALL_I64:
      printCall("call.i64", OS, MI, STI, 0, 0, 0);
      break;
    case LLIR::CALL_I64_VA:
      printCall("call.i64", OS, MI, STI, 0, 1, 0);
      break;
    case LLIR::CALL_F64:
      printCall("call.f64", OS, MI, STI, 0, 0, 0);
      break;
    case LLIR::CALL_F64_VA:
      printCall("call.f64", OS, MI, STI, 0, 1, 0);
      break;
    case LLIR::CALL_F80:
      printCall("call.f80", OS, MI, STI, 0, 0, 0);
      break;
    case LLIR::CALL_F80_VA:
      printCall("call.f80", OS, MI, STI, 0, 1, 0);
      break;
    case LLIR::CALL_VOID:
      printCall("call", OS, MI, STI, 1, 0, 0);
      break;
    case LLIR::CALL_VOID_VA:
      printCall("call", OS, MI, STI, 1, 1, 0);
      break;

    case LLIR::TCALL_I8:
      printCall("tcall.i8", OS, MI, STI, 0, 0, 1);
      break;
    case LLIR::TCALL_I8_VA:
      printCall("tcall.i8", OS, MI, STI, 0, 1, 1);
      break;
    case LLIR::TCALL_I16:
      printCall("tcall.i16", OS, MI, STI, 0, 0, 1);
      break;
    case LLIR::TCALL_I16_VA:
      printCall("tcall.i16", OS, MI, STI, 0, 1, 1);
      break;
    case LLIR::TCALL_I32:
      printCall("tcall.i32", OS, MI, STI, 0, 0, 1);
      break;
    case LLIR::TCALL_I32_VA:
      printCall("tcall.i32", OS, MI, STI, 0, 1, 1);
      break;
    case LLIR::TCALL_F32:
      printCall("tcall.f32", OS, MI, STI, 0, 0, 1);
      break;
    case LLIR::TCALL_F32_VA:
      printCall("tcall.f32", OS, MI, STI, 0, 1, 1);
      break;
    case LLIR::TCALL_I64:
      printCall("tcall.i64", OS, MI, STI, 0, 0, 1);
      break;
    case LLIR::TCALL_I64_VA:
      printCall("tcall.i64", OS, MI, STI, 0, 1, 1);
      break;
    case LLIR::TCALL_F64:
      printCall("tcall.f64", OS, MI, STI, 0, 0, 1);
      break;
    case LLIR::TCALL_F64_VA:
      printCall("tcall.f64", OS, MI, STI, 0, 1, 1);
      break;
    case LLIR::TCALL_F80:
      printCall("tcall.f80", OS, MI, STI, 0, 0, 1);
      break;
    case LLIR::TCALL_F80_VA:
      printCall("tcall.f80", OS, MI, STI, 0, 1, 1);
      break;
    case LLIR::TCALL_VOID:
      printCall("tcall", OS, MI, STI, 1, 0, 1);
      break;
    case LLIR::TCALL_VOID_VA:
      printCall("tcall", OS, MI, STI, 1, 1, 1);
      break;

    case LLIR::SWITCH_I64: {
      OS << "\tswitch\t";
      printOperand(MI, 0, STI, OS);
      for (unsigned i = 1; i < MI->getNumOperands(); ++i) {
        OS << ", ";
        printOperand(MI, i, STI, OS);
      }
      break;
    }

    default: {
      printInstruction(MI, Address, STI, OS);
      break;
    }
  }
}

void LLIRInstPrinter::printOperand(const MCInst *MI, int opNum,
                                   const MCSubtargetInfo &STI,
                                   raw_ostream &OS) {
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
    union {
      double dv;
      uint64_t iv;
    };
    dv = MO.getFPImm();
    OS << iv;
    return;
  }

  llvm_unreachable("unknown operand kind");
}
