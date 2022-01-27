//===--- Musl.h - Musl Tool and ToolChain Implementations -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_MUSL_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_MUSL_H

#include <set>

#include "Cuda.h"
#include "clang/Driver/Tool.h"
#include "clang/Driver/ToolChain.h"

namespace clang {
namespace driver {

namespace tools {

/// Base class for tools.
class LLVM_LIBRARY_VISIBILITY MuslTool : public Tool {
  virtual void anchor();

 public:
  MuslTool(const char *Name, const char *ShortName, const ToolChain &TC)
      : Tool(Name, ShortName, TC) {}
};

/// Directly call Musl Binutils' assembler and linker.
namespace musl {

class LLVM_LIBRARY_VISIBILITY Linker : public MuslTool {
 public:
  Linker(const ToolChain &TC) : MuslTool("Musl::Linker", "linker", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  bool isLinkJob() const override { return true; }

  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

}  // end namespace musl
}  // end namespace tools

namespace toolchains {

/// Musl - A tool chain built around the musl standard library implementation.
class LLVM_LIBRARY_VISIBILITY Musl : public ToolChain {
 public:
  Musl(const Driver &D, const llvm::Triple &Triple,
       const llvm::opt::ArgList &Args);
  ~Musl() override;

  void printVerboseInfo(raw_ostream &OS) const override;

  bool useIntegratedAs() const override { return true; }
  bool IsUnwindTablesDefault(const llvm::opt::ArgList &Args) const override;
  bool isPICDefault() const override { return false; }
  bool isPIEDefault() const override { return false; }
  bool isPICDefaultForced() const override { return false; }
  bool IsIntegratedAssemblerDefault() const override { return true; }

  std::string computeSysRoot() const override;

 protected:
  Tool *buildLinker() const override;

 public:
  /// \name ToolChain Implementation Helper Functions
  /// @{

  /// Check whether the target triple's architecture is 64-bits.
  bool isTarget64Bit() const { return getTriple().isArch64Bit(); }

  /// Check whether the target triple's architecture is 32-bits.
  bool isTarget32Bit() const { return getTriple().isArch32Bit(); }

  void AddClangSystemIncludeArgs(
      const llvm::opt::ArgList &DriverArgs,
      llvm::opt::ArgStringList &CC1Args) const override;

  void AddClangCXXStdlibIncludeArgs(
      const llvm::opt::ArgList &DriverArgs,
      llvm::opt::ArgStringList &CC1Args) const override;

  void AddCXXStdlibLibArgs(const llvm::opt::ArgList &Args,
                           llvm::opt::ArgStringList &CmdArgs) const override;

  RuntimeLibType GetRuntimeLibType(
      const llvm::opt::ArgList &Args) const override;

  /// @}
};

}  // end namespace toolchains
}  // end namespace driver
}  // end namespace clang

#endif  // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_MUSL_H
