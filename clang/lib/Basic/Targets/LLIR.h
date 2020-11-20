//===--- LLIR.h - Declare LLIR target feature support -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares LLIR TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_LLIR_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_LLIR_H

#include "OSTargets.h"
#include "X86.h"
#include "AArch64.h"
#include "PPC.h"
#include "RISCV.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/X86TargetParser.h"

namespace clang {
namespace targets {

// llir x86-64 generic target
class LLVM_LIBRARY_VISIBILITY LLIR_X86_64TargetInfo : public X86_64TargetInfo {
 public:
  LLIR_X86_64TargetInfo(const llvm::Triple &Triple, const TargetOptions &Opts)
      : X86_64TargetInfo(Triple, Opts) {
    // Pointers are 32-bit in x32.
    resetDataLayout(
        "e-m:e-p270:32:32-p271:32:32-p272:64:"
        "64-i64:64-f80:128-n8:16:32:64-S128");
  }

  const char *getClobbers() const override { return ""; }

  bool handleTargetFeatures(std::vector<std::string> &Features,
                            DiagnosticsEngine &Diags) override {
    if (X86_64TargetInfo::handleTargetFeatures(Features, Diags)) {
      HasFMA = false;
      SSELevel = std::min(SSE1, SSELevel);
      XOPLevel = std::min(NoXOP, XOPLevel);
      return true;
    }
    return false;
  }

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override {
    X86_64TargetInfo::getTargetDefines(Opts, Builder);
    Builder.defineMacro("__llir__");
    Builder.defineMacro("__llir_x86_64__");
  }
};

// llir aarch64 generic target
class LLVM_LIBRARY_VISIBILITY LLIR_AArch64leTargetInfo
    : public AArch64leTargetInfo {
public:
  LLIR_AArch64leTargetInfo(const llvm::Triple &Triple, const TargetOptions &Opts)
      : AArch64leTargetInfo(Triple, Opts) {}

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override {
    AArch64leTargetInfo::getTargetDefines(Opts, Builder);
    Builder.defineMacro("__llir__");
    Builder.defineMacro("__llir_aarch64__");
  }
};

// llir ppc64 generic target
class LLVM_LIBRARY_VISIBILITY LLIR_PPC64TargetInfo
    : public PPC64TargetInfo {
public:
  LLIR_PPC64TargetInfo(const llvm::Triple &Triple, const TargetOptions &Opts)
      : PPC64TargetInfo(Triple, Opts) {}

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override {
    PPC64TargetInfo::getTargetDefines(Opts, Builder);
    Builder.defineMacro("__llir__");
    Builder.defineMacro("__llir_powerpc64__");
  }
};

// llir ppc64 generic target
class LLVM_LIBRARY_VISIBILITY LLIR_RISCV64TargetInfo
    : public RISCV64TargetInfo {
public:
  LLIR_RISCV64TargetInfo(const llvm::Triple &Triple, const TargetOptions &Opts)
      : RISCV64TargetInfo(Triple, Opts) {}

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override {
    RISCV64TargetInfo::getTargetDefines(Opts, Builder);
    Builder.defineMacro("__llir__");
    Builder.defineMacro("__llir_riscv64__");
  }
};

}  // namespace targets
}  // namespace clang
#endif  // LLVM_CLANG_LIB_BASIC_TARGETS_LLIR_H
