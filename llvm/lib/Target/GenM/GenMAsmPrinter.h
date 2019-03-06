// GenMAsmPrinter.h - GenM implementation of AsmPrinter -------------*- C++ -*-
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_GENMASMPRINTER_H
#define LLVM_LIB_TARGET_GENM_GENMASMPRINTER_H

#include "GenMMachineFunctionInfo.h"
#include "GenMSubtarget.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class GenMMCTargetStreamer;

class LLVM_LIBRARY_VISIBILITY GenMAsmPrinter final : public AsmPrinter {
public:
  explicit GenMAsmPrinter(
      TargetMachine &TM,
      std::unique_ptr<MCStreamer> Streamer)
    : AsmPrinter(TM, std::move(Streamer))
  {
  }

  StringRef getPassName() const override { return "GenM Assembly Printer"; }

  void EmitGlobalVariable(const GlobalVariable *GV) override;
  void EmitFunctionBodyStart() override;
  void EmitInstruction(const MachineInstr *MI) override;

  bool PrintAsmOperand(
      const MachineInstr *MI,
      unsigned OpNo,
      unsigned AsmVariant,
      const char *ExtraCode,
      raw_ostream &O
  ) override;

  bool PrintAsmMemoryOperand(
      const MachineInstr *MI,
      unsigned OpNo,
      unsigned AsmVariant,
      const char *ExtraCode,
      raw_ostream &O
  ) override;

private:
  GenMMCTargetStreamer &getTargetStreamer();
};


} // end namespace llvm

#endif
