//===-- LLIRTargetInfo.cpp - LLIR Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/LLIRMCTargetDesc.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheLLIR_X86_64Target() {
  static Target TheLLIR_X86_64_Target;
  return TheLLIR_X86_64_Target;
}

Target &llvm::getTheLLIR_AArch64Target() {
  static Target TheLLIR_AArch64_Target;
  return TheLLIR_AArch64_Target;
}

extern "C" void LLVMInitializeLLIRTargetInfo() {
  RegisterTarget<Triple::llir_x86_64, false> X(
      getTheLLIR_X86_64Target(), "llir-x86-64", "LLIR over x86-64", "LLIR");
  RegisterTarget<Triple::llir_aarch64_be, false> Y(getTheLLIR_AArch64Target(),
                                                   "llir-aarch64-be",
                                                   "LLIR over aarch64", "LLIR");
}
