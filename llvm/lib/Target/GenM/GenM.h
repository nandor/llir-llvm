//===-- GenM.h - Top-level interface for GenM representation ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// GenM back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_GENM_H
#define LLVM_LIB_TARGET_GENM_GENM_H

#include "llvm/PassRegistry.h"
#include "llvm/Support/CodeGen.h"

namespace llvm {
class FunctionPass;
class GenMTargetMachine;
class MachineInstr;

// GenM-Specific passes.
FunctionPass *createGenMArgumentMove();
FunctionPass *createGenMISelDag(GenMTargetMachine &TM);
FunctionPass *createGenMRegisterNumbering();

// PassRegistry initialization declarations.
void initializeGenMArgumentMovePass(PassRegistry &);
void initializeGenMRegisterNumberingPass(PassRegistry &);

namespace GenM {
bool isArgument(MachineInstr &MI);
}

} // end namespace llvm;

#endif
