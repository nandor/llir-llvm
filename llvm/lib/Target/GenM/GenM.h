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

#include "MCTargetDesc/GenMMCTargetDesc.h"

namespace llvm {
class FunctionPass;
class GenMTargetMachine;

FunctionPass *createGenMISelDag(GenMTargetMachine &TM);

} // end namespace llvm;

#endif
