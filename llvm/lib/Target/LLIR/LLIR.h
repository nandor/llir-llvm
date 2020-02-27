//===-- LLIR.h - Top-level interface for LLIR representation ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// LLIR back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIR_H
#define LLVM_LIB_TARGET_LLIR_LLIR_H

#include "llvm/PassRegistry.h"
#include "llvm/Support/CodeGen.h"

namespace llvm {
class FunctionPass;
class LLIRTargetMachine;
class MachineInstr;

// LLIR-Specific passes.
FunctionPass *createLLIRISelDag(LLIRTargetMachine &TM);
FunctionPass *createLLIRRegisterNumbering();
FunctionPass *createLLIRArgumentMove();

// PassRegistry initialization declarations.
void initializeLLIRRegisterNumberingPass(PassRegistry &);
void initializeLLIRArgumentMovePass(PassRegistry &);

namespace LLIR {
}

} // end namespace llvm;

#endif
