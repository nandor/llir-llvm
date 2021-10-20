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

#include "Arch/RISCV.h"
#include "Arch/PPC.h"
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
  auto &TC = static_cast<const toolchains::LLIR &>(getToolChain());
  const llvm::Triple &Triple = TC.getTriple();
  ArgStringList CmdArgs;

  // Ignore a bunch of arguments.
  claimNoWarnArgs(Args);
  ParsePICArgs(TC, Args);

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
  const char *Exec = Args.MakeArgString(TC.GetProgramPath(as.c_str()));
  C.addCommand(std::make_unique<Command>(
      JA, *this, ResponseFileSupport::AtFileCurCP(), Exec, CmdArgs, Inputs));
}

static bool getPIE(const ArgList &Args, const ToolChain &TC) {
  if (Args.hasArg(options::OPT_shared) || Args.hasArg(options::OPT_static) ||
      Args.hasArg(options::OPT_r) || Args.hasArg(options::OPT_static_pie))
    return false;

  Arg *A = Args.getLastArg(options::OPT_pie, options::OPT_no_pie,
                           options::OPT_nopie);
  if (!A)
    return TC.isPIEDefault();
  return A->getOption().matches(options::OPT_pie);
}

void tools::llir::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                                       const InputInfo &Output,
                                       const InputInfoList &Inputs,
                                       const llvm::opt::ArgList &Args,
                                       const char *LinkingOutput) const {
  auto &TC = static_cast<const toolchains::LLIR &>(getToolChain());
  const Driver &D = TC.getDriver();
  const llvm::Triple &Triple = TC.getTriple();
  ArgStringList CmdArgs;

  SmallString<128> SysRoot(TC.computeSysRoot());
  SmallString<128> LibDir(SysRoot);
  llvm::sys::path::append(LibDir, "lib");

  // TODO: forward the pie flag.
  bool pie = getPIE(Args, TC);
  (void) pie;

  // Forward architecture flags.
  std::vector<StringRef> Features;
  switch (Triple.getArch()) {
  default:
    llvm_unreachable("invalid LLIR architecture");
  case llvm::Triple::llir_x86:
  case llvm::Triple::llir_x86_64:
    break;
  case llvm::Triple::llir_aarch64:
    break;
  case llvm::Triple::llir_ppc64le: {
    llvm::Triple BaseTriple(Triple);
    BaseTriple.setArch(llvm::Triple::ppc64le);
    ppc::getPPCTargetFeatures(TC.getDriver(), BaseTriple, Args, Features);
    break;
  }
  case llvm::Triple::llir_riscv64: {
    llvm::Triple BaseTriple(Triple);
    BaseTriple.setArch(llvm::Triple::riscv64);
    riscv::getRISCVTargetFeatures(TC.getDriver(), BaseTriple, Args, Features);

    StringRef ABIName = riscv::getRISCVABI(Args, BaseTriple);
    CmdArgs.push_back("-mabi");
    CmdArgs.push_back(ABIName.data());

    CmdArgs.push_back("-mcpu");
    if (const Arg *A = Args.getLastArg(options::OPT_mcpu_EQ)) {
      CmdArgs.push_back(A->getValue());
    } else {
      CmdArgs.push_back("generic-rv64");
    }
    break;
  }
  }

  // Forward all feature flags.
  if (!Features.empty()) {
    std::string FS;
    for (StringRef Feature : Features) {
      if (!FS.empty()) {
        FS += ",";
      }
      FS += Feature;
    }
    CmdArgs.push_back("-mfs");
    CmdArgs.push_back(Args.MakeArgString(FS));
  }

  // Forward the rdynamic flag.
  if (Args.hasArg(options::OPT_rdynamic))
    CmdArgs.push_back("-E");

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

  // Set up the CRT, based on the environment.
  switch (Triple.getEnvironment()) {
  default: break;
  case llvm::Triple::Musl:
  case llvm::Triple::MuslEABI:
  case llvm::Triple::MuslEABIHF: {
    if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nostartfiles)) {
      if (!Args.hasArg(options::OPT_shared))
        CmdArgs.push_back(Args.MakeArgString(TC.GetFilePath("crt1.o")));
    }
    if (!Args.hasArg(options::OPT_nostartfiles) && D.CCCIsCXX()) {
      auto CrtBegin = TC.getCompilerRT(Args, "crtbegin", ToolChain::FT_Object);
      if (TC.getVFS().exists(CrtBegin)) {
        CmdArgs.push_back(Args.MakeArgString(CrtBegin));
      }
    }
    break;
  }
  }

  // Forward linker flags.
  Args.AddAllArgs(CmdArgs, options::OPT_L);
  Args.AddAllArgs(CmdArgs, options::OPT_u);
  TC.AddFilePathLibArgs(Args, CmdArgs);

  AddLinkerInputs(TC, Inputs, Args, CmdArgs, JA);

  // Add the implicit C library.
  switch (Triple.getEnvironment()) {
  default: break;
  case llvm::Triple::Musl:
  case llvm::Triple::MuslEABI:
  case llvm::Triple::MuslEABIHF: {
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
    break;
  }
  }

  // Prepare flags for shared/static libs.
  if (Args.hasArg(options::OPT_static)) {
    CmdArgs.push_back("-static");
  } else if (Args.hasArg(options::OPT_shared)) {
    CmdArgs.push_back("-shared");
  } else {
    switch (Triple.getEnvironment()) {
    case llvm::Triple::Musl:
    case llvm::Triple::MuslEABI:
    case llvm::Triple::MuslEABIHF: {
      SmallString<128> DynamicLinkerPath(LibDir);
      std::string Linker = "ld-musl-" + Triple.getArchName().str() + ".so.1";
      llvm::sys::path::append(DynamicLinkerPath, Linker);

      CmdArgs.push_back("-rpath");
      CmdArgs.push_back(Args.MakeArgString(LibDir));

      CmdArgs.push_back("-dynamic-linker");
      CmdArgs.push_back(Args.MakeArgString(DynamicLinkerPath));
      break;
    }
    default: {
      break;
    }
    }
  }

  // Add libunwind.
  {
    SmallString<128> Path(LibDir);
    llvm::sys::path::append(Path, "libunwind");
    auto LibPath = (Path + ".a").str();
    auto SoPath = (Path + ".so").str();
    if (Args.hasArg(options::OPT_static) || TC.getVFS().exists(LibPath)) {
      CmdArgs.push_back(Args.MakeArgString(LibPath));
    } else if (TC.getVFS().exists(SoPath)) {
      CmdArgs.push_back(Args.MakeArgString(SoPath));
    }
  }

  // Add compiler-rt.
  auto RT = TC.getCompilerRT(Args, "builtins", ToolChain::FT_Static);
  if (TC.getVFS().exists(RT)) {
    CmdArgs.push_back(Args.MakeArgString(RT));
    if (!Args.hasArg(options::OPT_nostdlib)) {
      if (!Args.hasArg(options::OPT_nolibc)) {
        CmdArgs.push_back(Args.MakeArgString("-lc"));
      }
    }
  }

  // CRT finalisation.
  switch (Triple.getEnvironment()) {
  default: break;
  case llvm::Triple::Musl:
  case llvm::Triple::MuslEABI:
  case llvm::Triple::MuslEABIHF: {
    if (!Args.hasArg(options::OPT_nostartfiles) && D.CCCIsCXX()) {
      auto CrtEnd = TC.getCompilerRT(Args, "crtend", ToolChain::FT_Object);
      if (TC.getVFS().exists(CrtEnd)) {
        CmdArgs.push_back(Args.MakeArgString(CrtEnd));
      }
    }
    break;
  }
  }

  CmdArgs.push_back("-o");
  CmdArgs.push_back(Output.getFilename());

  const std::string LD = Triple.str() + "-ld";
  const char *Exec = Args.MakeArgString(TC.GetProgramPath(LD.c_str()));
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

void LLIR::AddClangSystemIncludeArgs(const ArgList &DriverArgs,
                                     ArgStringList &CC1Args) const {
  if (!DriverArgs.hasArg(options::OPT_nobuiltininc)) {
    SmallString<128> ResourceInclude(getDriver().ResourceDir);
    llvm::sys::path::append(ResourceInclude, "include");
    addSystemInclude(DriverArgs, CC1Args, ResourceInclude);
  }
  if (!DriverArgs.hasArg(options::OPT_nostdinc)) {
    SmallString<128> SysRootInclude(computeSysRoot());
    llvm::sys::path::append(SysRootInclude, "include");
    addExternCSystemInclude(DriverArgs, CC1Args, SysRootInclude);
  }
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

ToolChain::RuntimeLibType LLIR::GetRuntimeLibType(
    const ArgList &Args) const {
  if (Arg *A = Args.getLastArg(clang::driver::options::OPT_rtlib_EQ)) {
    StringRef Value = A->getValue();
    if (Value != "compiler-rt")
      getDriver().Diag(clang::diag::err_drv_invalid_rtlib_name)
          << A->getAsString(Args);
  }

  return ToolChain::RLT_CompilerRT;
}
