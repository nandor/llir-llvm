//===-- LLIRException.h - LLIR Exception Framework -----------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains support for writing dwarf exception info into asm files.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_CODEGEN_ASMPRINTER_LLIREXCEPTION_H
#define LLVM_LIB_CODEGEN_ASMPRINTER_LLIREXCEPTION_H

#include "EHStreamer.h"
#include "llvm/CodeGen/AsmPrinter.h"

namespace llvm {
class MachineFunction;
class ARMTargetStreamer;

class LLVM_LIBRARY_VISIBILITY LLIRException : public EHStreamer {
public:
  LLIRException(AsmPrinter *A);
  ~LLIRException() override;

  void markFunctionEnd() override;
  void endFragment() override;

  /// Emit all exception information that should come after the content.
  void endModule() override;

  /// Gather pre-function exception information.  Assumes being emitted
  /// immediately after the function entry point.
  void beginFunction(const MachineFunction *MF) override;

  /// Gather and emit post-function exception information.
  void endFunction(const MachineFunction *) override;

  void beginFragment(const MachineBasicBlock *MBB,
                     ExceptionSymbolProvider ESP) override;

  void beginBasicBlock(const MachineBasicBlock &MBB) override;
  void endBasicBlock(const MachineBasicBlock &MBB) override;
};

} // End of namespace llvm

#endif // LLVM_LIB_CODEGEN_ASMPRINTER_LLIREXCEPTION_H
