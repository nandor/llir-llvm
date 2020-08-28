//===--- LLIR.h - LLIR Tool and ToolChain Implementations --------*- C++
//-*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_LLIR_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_LLIR_H

#include "Cuda.h"
#include "clang/Driver/Tool.h"
#include "clang/Driver/ToolChain.h"
#include <set>

namespace clang {
namespace driver {

namespace tools {

/// Base class for all LLIR tools that provide the same behavior when
/// it comes to response files support
class LLVM_LIBRARY_VISIBILITY LLIRTool : public Tool {
  virtual void anchor();

public:
  LLIRTool(const char *Name, const char *ShortName, const ToolChain &TC)
      : Tool(Name, ShortName, TC) {}
};

/// Directly call LLIR Binutils' assembler and linker.
namespace llir {
class LLVM_LIBRARY_VISIBILITY Assembler : public LLIRTool {
public:
  Assembler(const ToolChain &TC)
      : LLIRTool("LLIR::Assembler", "assembler", TC) {}

  bool hasIntegratedCPP() const override { return false; }

  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

class LLVM_LIBRARY_VISIBILITY Linker : public LLIRTool {
public:
  Linker(const ToolChain &TC) : LLIRTool("LLIR::Linker", "linker", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  bool isLinkJob() const override { return true; }

  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

} // end namespace llir
} // end namespace tools

namespace toolchains {

/// LLIR - A tool chain using the 'llir' command to perform
/// all subcommands; this relies on llir translating the majority of
/// command line options.
class LLVM_LIBRARY_VISIBILITY LLIR : public ToolChain {
public:
  LLIR(const Driver &D, const llvm::Triple &Triple,
       const llvm::opt::ArgList &Args);
  ~LLIR() override;

  void printVerboseInfo(raw_ostream &OS) const override;

  bool useIntegratedAs() const override { return false; }
  bool IsUnwindTablesDefault(const llvm::opt::ArgList &Args) const override;
  bool isPICDefault() const override { return false; }
  bool isPIEDefault() const override { return false; }
  bool isPICDefaultForced() const override { return false; }
  bool IsIntegratedAssemblerDefault() const override { return true; }

  std::string computeSysRoot() const override;

protected:
  Tool *buildAssembler() const override;
  Tool *buildLinker() const override;

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
  /// @}
};

} // end namespace toolchains
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_LLIR_H
