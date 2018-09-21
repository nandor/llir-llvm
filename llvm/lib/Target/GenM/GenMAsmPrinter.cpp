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

#include "GenM.h"
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

namespace {

class GenMAsmPrinter : public AsmPrinter {
public:
  explicit GenMAsmPrinter(
      TargetMachine &TM,
      std::unique_ptr<MCStreamer> Streamer)
    : AsmPrinter(TM, std::move(Streamer))
  {
  }

  StringRef getPassName() const override { return "GenM Assembly Printer"; }

  void EmitFunctionBodyStart() override;
  void EmitInstruction(const MachineInstr *MI) override;

  static const char *getRegisterName(unsigned RegNo)
  {
    assert(!"not implemented");
  }

  bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                       unsigned AsmVariant, const char *ExtraCode,
                       raw_ostream &O) override;
  bool PrintAsmMemoryOperand(const MachineInstr *MI, unsigned OpNo,
                             unsigned AsmVariant, const char *ExtraCode,
                             raw_ostream &O) override;

private:
  GenMMCTargetStreamer &getTargetStreamer()
  {
    return static_cast<GenMMCTargetStreamer &>(
        *OutStreamer->getTargetStreamer()
    );
  }
};

} // end of anonymous namespace

void GenMAsmPrinter::EmitInstruction(const MachineInstr *MI)
{
  assert(!"not implemented");
}

void GenMAsmPrinter::EmitFunctionBodyStart()
{
  assert(!"not implemented");
}

/// PrintAsmOperand - Print out an operand for an inline asm expression.
///
bool GenMAsmPrinter::PrintAsmOperand(
    const MachineInstr *MI,
    unsigned OpNo,
    unsigned AsmVariant,
    const char *ExtraCode,
    raw_ostream &O)
{
  assert(!"not implemented");
}

bool GenMAsmPrinter::PrintAsmMemoryOperand(
    const MachineInstr *MI,
    unsigned OpNo,
    unsigned AsmVariant,
    const char *ExtraCode,
    raw_ostream &O)
{
  assert(!"not implemented");
}

// Force static initialization.
extern "C" void LLVMInitializeGenMAsmPrinter()
{
  RegisterAsmPrinter<GenMAsmPrinter> X(getTheGenMTarget());
}
