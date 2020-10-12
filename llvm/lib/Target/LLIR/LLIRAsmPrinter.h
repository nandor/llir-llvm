// LLIRAsmPrinter.h - LLIR implementation of AsmPrinter -------------*- C++ -*-
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIRASMPRINTER_H
#define LLVM_LIB_TARGET_LLIR_LLIRASMPRINTER_H

#include "LLIRMachineFunctionInfo.h"
#include "LLIRSubtarget.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class LLIRMCTargetStreamer;

class LLVM_LIBRARY_VISIBILITY LLIRAsmPrinter final : public AsmPrinter {
public:
  explicit LLIRAsmPrinter(
      TargetMachine &TM,
      std::unique_ptr<MCStreamer> Streamer)
    : AsmPrinter(TM, std::move(Streamer))
  {
  }

  StringRef getPassName() const override { return "LLIR Assembly Printer"; }

  void emitGlobalVariable(const GlobalVariable *GV) override;
  void emitFunctionBodyStart() override;
  void emitFunctionBodyEnd() override;
  void emitInstruction(const MachineInstr *MI) override;
  void emitJumpTableInfo() override;
  void emitXXStructorList(const DataLayout &DL, const Constant *List,
                          bool IsCtor) override;

  bool PrintAsmOperand(
      const MachineInstr *MI,
      unsigned OpNo,
      const char *ExtraCode,
      raw_ostream &O
  ) override;

  bool PrintAsmMemoryOperand(
      const MachineInstr *MI,
      unsigned OpNo,
      const char *ExtraCode,
      raw_ostream &O
  ) override;

private:
  LLIRMCTargetStreamer &getTargetStreamer();
};


} // end namespace llvm

#endif
