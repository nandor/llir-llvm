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

Target &llvm::getTheLLIR_X86_32Target() {
  static Target TheLLIR_X86_32_Target;
  return TheLLIR_X86_32_Target;
}

Target &llvm::getTheLLIR_X86_64Target() {
  static Target TheLLIR_X86_64_Target;
  return TheLLIR_X86_64_Target;
}

Target &llvm::getTheLLIR_AArch64Target() {
  static Target TheLLIR_AArch64_Target;
  return TheLLIR_AArch64_Target;
}

Target &llvm::getTheLLIR_PPC64LETarget() {
  static Target TheLLIR_PPC64LE_Target;
  return TheLLIR_PPC64LE_Target;
}

Target &llvm::getTheLLIR_RISCV64Target() {
  static Target TheLLIR_RiscV64_Target;
  return TheLLIR_RiscV64_Target;
}

extern "C" void LLVMInitializeLLIRTargetInfo() {
  RegisterTarget<Triple::llir_x86, false> A(
      getTheLLIR_X86_32Target(), "llir-x86-32", "LLIR over x86-32", "LLIR");
  RegisterTarget<Triple::llir_x86_64, false> B(
      getTheLLIR_X86_64Target(), "llir-x86-64", "LLIR over x86-64", "LLIR");
  RegisterTarget<Triple::llir_aarch64, false> C(
      getTheLLIR_AArch64Target(), "llir-aarch64", "LLIR over aarch64", "LLIR");
  RegisterTarget<Triple::llir_ppc64le, false> D(
      getTheLLIR_PPC64LETarget(), "llir-ppc64le", "LLIR over ppc64le", "LLIR");
  RegisterTarget<Triple::llir_riscv64, false> E(
      getTheLLIR_RISCV64Target(), "llir-riscv64", "LLIR over riscv64", "LLIR");
}
