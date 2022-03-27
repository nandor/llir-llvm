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
#include "MCTargetDesc/LLIRMCExpr.h"
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
    case LLIR::SP: {
      OS << "$sp";
      break;
    }
    case LLIR::FRAME_ADDR: {
      OS << "$frame_addr";
      break;
    }
    case LLIR::RETURN_ADDR: {
      OS << "$ret_addr";
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

static void printFlags(llvm::raw_ostream &OS, uint64_t imm)
{
  switch (imm & 0xFF) {
  case 0: return;
  case 1: {
    unsigned size = (imm >> 32) & 0xFFFF;
    unsigned align = (imm >> 16) & 0xFFFF;
    OS << ":byval:" << size << ":" << align;
    return;
  }
  case 2:
    OS << ":zext";
    return;
  case 3:
    OS << ":sext";
    return;
  }
  llvm_unreachable("invalid flag kind");
}

void LLIRInstPrinter::printCall(const char *Op, llvm::raw_ostream &OS,
                                const MCInst *MI, const MCSubtargetInfo &STI,
                                bool isVoid, bool isVA, bool isTail,
                                bool isInvoke) {
  unsigned start = isVoid ? 0 : 1;
  if (isVA) {
    OS << '\t' << Op << '.' << MI->getOperand(start++).getImm() << '.';
  } else {
    OS << '\t' << Op << '.';
  }
  printCallingConv(OS, MI->getOperand(start++).getImm());
  unsigned throwOp = 0;
  if (isInvoke) {
    throwOp = start++;
  }
  OS << '\t';

  // Return value.
  if (!isVoid) {
    if (!isTail) {
      printOperand(MI, 0, STI, OS);
      printFlags(OS, MI->getOperand(start++).getImm());
      OS << ", ";
    } else {
      start++;
    }
  }
  // Callee.
  printOperand(MI, start++, STI, OS);
  // Arguments.
  for (unsigned i = start, n = MI->getNumOperands(); i < n; i += 2) {
    OS << ", ";
    printOperand(MI, i + 1, STI, OS);
    printFlags(OS, MI->getOperand(i).getImm());
  }
  // Throw block.
  if (isInvoke) {
    OS << ", ";
    printOperand(MI, throwOp, STI, OS);
  }
}

void LLIRInstPrinter::printReturn(llvm::raw_ostream &OS, const MCInst *MI,
                                  const MCSubtargetInfo &STI) {
  OS << "\tret\t";
  for (unsigned i = 0, n = MI->getNumOperands(); i < n; i += 2) {
    if (i != 0) {
      OS << ", ";
    }
    printOperand(MI, i + 1, STI, OS);
    printFlags(OS, MI->getOperand(i).getImm());
  }
}

void LLIRInstPrinter::printLandingPad(const char *type, raw_ostream &OS,
                                      const MCInst *MI,
                                      const MCSubtargetInfo &STI) {
  OS << "\tlanding_pad" << type << '\t';
  for (unsigned i = 0, n = MI->getNumOperands(); i < n; ++i) {
    auto MO = MI->getOperand(i);
    if (i != 0) {
      if (MO.isExpr() && isa<LLIRMCExpr>(MO.getExpr())) {
        OS << " ";
      } else {
        OS << ", ";
      }
    }
    printOperand(MI, i, STI, OS);
  }
}

void LLIRInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                StringRef Annot, const MCSubtargetInfo &STI,
                                raw_ostream &OS) {
  switch (MI->getOpcode()) {
#define MAKE_CALL(name, op, v, t, i)\
  case LLIR::name##_I8:   printCall(op ".i8",   OS, MI, STI, 0, v, t, i); break; \
  case LLIR::name##_I16:  printCall(op ".i16",  OS, MI, STI, 0, v, t, i); break; \
  case LLIR::name##_I32:  printCall(op ".i32",  OS, MI, STI, 0, v, t, i); break; \
  case LLIR::name##_F32:  printCall(op ".f32",  OS, MI, STI, 0, v, t, i); break; \
  case LLIR::name##_I64:  printCall(op ".i64",  OS, MI, STI, 0, v, t, i); break; \
  case LLIR::name##_I128: printCall(op ".i128", OS, MI, STI, 0, v, t, i); break; \
  case LLIR::name##_F64:  printCall(op ".f64",  OS, MI, STI, 0, v, t, i); break; \
  case LLIR::name##_F80:  printCall(op ".f80",  OS, MI, STI, 0, v, t, i); break; \
  case LLIR::name##_F128: printCall(op ".f128", OS, MI, STI, 0, v, t, i); break; \
  case LLIR::name##_VOID: printCall(op,         OS, MI, STI, 1, v, t, i); break;

    MAKE_CALL(CALL,      "call",   0, 0, 0)
    MAKE_CALL(CALL_VA,   "call",   1, 0, 0)
    MAKE_CALL(TCALL,     "tcall",  0, 1, 0)
    MAKE_CALL(TCALL_VA,  "tcall",  1, 1, 0)
    MAKE_CALL(INVOKE,    "invoke", 0, 0, 1)
    MAKE_CALL(INVOKE_VA, "invoke", 1, 0, 1)

#undef MAKE_CALL

    case LLIR::SWITCH_I32:
    case LLIR::SWITCH_I64: {
      OS << "\tswitch\t";
      printOperand(MI, 0, STI, OS);
      for (unsigned i = 1; i < MI->getNumOperands(); ++i) {
        OS << ", ";
        printOperand(MI, i, STI, OS);
      }
      break;
    }

    case LLIR::RETURN:
      printReturn(OS, MI, STI);
      break;

    case LLIR::LANDING_PAD_I64_I64:
      printLandingPad(".i64.i64", OS, MI, STI);
      break;

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
    int64_t imm = MO.getImm();
    if (imm < 256) {
      OS << imm;
    } else {
      OS << format("0x%016" PRIx64, MO.getImm());
    }
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
