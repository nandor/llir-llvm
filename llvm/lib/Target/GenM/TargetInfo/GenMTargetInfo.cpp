//===-- GenMTargetInfo.cpp - GenM Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "GenM.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheGenMTarget()
{
  static Target TheGenMTarget;
  return TheGenMTarget;
}

extern "C" void LLVMInitializeGenMTargetInfo()
{
  RegisterTarget<Triple::genm, false> X(
      getTheGenMTarget(),
      "genm",
      "GenM",
      "GenM"
  );
}
