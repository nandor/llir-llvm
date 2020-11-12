//===--- LLIR.cpp - LLIR Tool and ToolChain Implementations -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "LLIR.h"

#include <system_error>

#include "CommonArgs.h"
#include "Linux.h"
#include "clang/Config/config.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "clang/Driver/Options.h"
#include "clang/Driver/Tool.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/TargetParser.h"
#include "llvm/Support/VirtualFileSystem.h"

using namespace clang::driver;
using namespace clang::driver::tools;
using namespace clang::driver::toolchains;
using namespace clang;
using namespace llvm::opt;

void tools::LLIRTool::anchor() {}

void tools::llir::Assembler::ConstructJob(Compilation &C, const JobAction &JA,
                                          const InputInfo &Output,
                                          const InputInfoList &Inputs,
                                          const llvm::opt::ArgList &Args,
                                          const char *LinkingOutput) const {
  auto &ToolChain = static_cast<const toolchains::LLIR &>(getToolChain());
  const llvm::Triple &Triple = ToolChain.getTriple();
  ArgStringList CmdArgs;

  CmdArgs.push_back("-o");
  CmdArgs.push_back(Output.getFilename());

  for (const auto &II : Inputs) {
    if (II.isFilename()) {
      CmdArgs.push_back(II.getFilename());
      continue;
    }
    llvm_unreachable("Assembler::ConstructJob");
  }

  const std::string as = Triple.str() + "-as";
  const char *Exec = Args.MakeArgString(ToolChain.GetProgramPath(as.c_str()));
  C.addCommand(std::make_unique<Command>(
      JA, *this, ResponseFileSupport::AtFileCurCP(), Exec, CmdArgs, Inputs));
}

void tools::llir::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                                       const InputInfo &Output,
                                       const InputInfoList &Inputs,
                                       const llvm::opt::ArgList &Args,
                                       const char *LinkingOutput) const {
  auto &ToolChain = static_cast<const toolchains::LLIR &>(getToolChain());
  const llvm::Triple &Triple = ToolChain.getTriple();
  ArgStringList CmdArgs;

  // Forward the optimisation level.
  if (Arg *A = Args.getLastArg(options::OPT_O_Group)) {
    if (A->getOption().matches(options::OPT_O0)) {
      CmdArgs.push_back("-O0");
    } else if (A->getOption().matches(options::OPT_O)) {
      CmdArgs.push_back(Args.MakeArgString("-O" + StringRef(A->getValue())));
    } else {
      llvm_unreachable("not implemented");
    }
  }

  if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nostartfiles)) {
    if (!Args.hasArg(options::OPT_shared))
      CmdArgs.push_back(Args.MakeArgString(ToolChain.GetFilePath("crt1.o")));
  }

  Args.AddAllArgs(CmdArgs, options::OPT_L);
  Args.AddAllArgs(CmdArgs, options::OPT_u);
  ToolChain.AddFilePathLibArgs(Args, CmdArgs);

  AddLinkerInputs(ToolChain, Inputs, Args, CmdArgs, JA);

  if (!Args.hasArg(options::OPT_nostdlib)) {
    if (!Args.hasArg(options::OPT_nodefaultlibs)) {
      bool WantPthread = Args.hasArg(options::OPT_pthread) ||
                         Args.hasArg(options::OPT_pthreads);

      if (WantPthread)
        CmdArgs.push_back("-lpthread");

      if (!Args.hasArg(options::OPT_nolibc))
        CmdArgs.push_back("-lc");
    }
  }

  if (Args.hasArg(options::OPT_static)) {
    CmdArgs.push_back("-static");
  } else if (Args.hasArg(options::OPT_shared)) {
    CmdArgs.push_back("-shared");
  } else {
    SmallString<128> SysRoot(ToolChain.computeSysRoot());

    SmallString<128> LibDir(SysRoot);
    llvm::sys::path::append(LibDir, "lib");

    SmallString<128> DynamicLinkerPath(LibDir);
    std::string Linker = "ld-musl-" + Triple.getArchName().str() + ".so.1";
    llvm::sys::path::append(DynamicLinkerPath, Linker);

    CmdArgs.push_back("-rpath");
    CmdArgs.push_back(Args.MakeArgString(LibDir));

    CmdArgs.push_back("-dynamic-linker");
    CmdArgs.push_back(Args.MakeArgString(DynamicLinkerPath));
  }

  CmdArgs.push_back("-o");
  CmdArgs.push_back(Output.getFilename());

  const std::string ld = Triple.str() + "-ld";
  const char *Exec = Args.MakeArgString(ToolChain.GetProgramPath(ld.c_str()));
  C.addCommand(std::make_unique<Command>(
      JA, *this, ResponseFileSupport::AtFileCurCP(), Exec, CmdArgs, Inputs));
}

LLIR::LLIR(const Driver &D, const llvm::Triple &Triple, const ArgList &Args)
    : ToolChain(D, Triple, Args) {
  getProgramPaths().push_back(getDriver().getInstalledDir());
  if (getDriver().getInstalledDir() != getDriver().Dir)
    getProgramPaths().push_back(getDriver().Dir);

  std::string SysRoot = computeSysRoot();
  path_list &FilePaths = getFilePaths();
  addPathIfExists(D, SysRoot + "/lib", FilePaths);
}

LLIR::~LLIR() {}

void LLIR::printVerboseInfo(raw_ostream &OS) const {}

bool LLIR::IsUnwindTablesDefault(const ArgList &Args) const { return false; }

Tool *LLIR::buildAssembler() const { return new tools::llir::Assembler(*this); }

Tool *LLIR::buildLinker() const { return new tools::llir::Linker(*this); }

void LLIR::AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                                     llvm::opt::ArgStringList &CC1Args) const {
  SmallString<128> SysRootInclude(computeSysRoot());
  llvm::sys::path::append(SysRootInclude, "include");
  addExternCSystemInclude(DriverArgs, CC1Args, SysRootInclude);

  SmallString<128> ResourceInclude(getDriver().ResourceDir);
  llvm::sys::path::append(ResourceInclude, "include");
  addSystemInclude(DriverArgs, CC1Args, ResourceInclude);
}

void LLIR::AddClangCXXStdlibIncludeArgs(const ArgList &DriverArgs,
                                        ArgStringList &CC1Args) const {
}

std::string LLIR::computeSysRoot() const {
  std::string SysRoot = getDriver().SysRoot;
  if (!SysRoot.empty()) {
    return SysRoot;
  }
  return std::string();
}
