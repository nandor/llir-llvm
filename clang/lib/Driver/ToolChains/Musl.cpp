//===--- Musl.cpp - Musl Tool and ToolChain Implementations -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Musl.h"

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

void tools::MuslTool::anchor() {}

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

void tools::musl::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                                       const InputInfo &Output,
                                       const InputInfoList &Inputs,
                                       const llvm::opt::ArgList &Args,
                                       const char *LinkingOutput) const {
  auto &TC = static_cast<const toolchains::Musl &>(getToolChain());
  const Driver &D = TC.getDriver();
  const llvm::Triple &Triple = TC.getTriple();
  ArgStringList CmdArgs;

  SmallString<128> SysRoot(TC.computeSysRoot());
  SmallString<128> LibDir(SysRoot);
  llvm::sys::path::append(LibDir, "lib");

  // TODO: forward the pie flag.
  bool pie = getPIE(Args, TC);
  (void) pie;

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
        if (D.CCCIsCXX()) {
          if (TC.ShouldLinkCXXStdlib(Args)) {
            bool OnlyLibstdcxxStatic = Args.hasArg(options::OPT_static_libstdcxx) &&
                                       !Args.hasArg(options::OPT_static);
            if (OnlyLibstdcxxStatic)
              CmdArgs.push_back("-Bstatic");
            TC.AddCXXStdlibLibArgs(Args, CmdArgs);
            if (OnlyLibstdcxxStatic)
              CmdArgs.push_back("-Bdynamic");
          }
          CmdArgs.push_back("-lm");
        }
        // Silence warnings when linking C code with a C++ '-stdlib' argument.
        Args.ClaimAllArgs(options::OPT_stdlib_EQ);

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

  // Forward flags to locate libraries.
  SmallString<128> DynamicLinkerPath(LibDir);
  std::string Linker = "ld-musl-" + Triple.getArchName().str() + ".so.1";
  llvm::sys::path::append(DynamicLinkerPath, Linker);

  CmdArgs.push_back("-rpath");
  CmdArgs.push_back(Args.MakeArgString(LibDir));

  CmdArgs.push_back("-rpath-link");
  CmdArgs.push_back(Args.MakeArgString(LibDir));

  CmdArgs.push_back("-dynamic-linker");
  CmdArgs.push_back(Args.MakeArgString(DynamicLinkerPath));

  // Generate .eh_frame_hdr section.
  CmdArgs.push_back("--eh-frame-hdr");

  // Prepare flags for shared/static libs.
  if (Args.hasArg(options::OPT_static)) {
    CmdArgs.push_back("-static");
  } else if (Args.hasArg(options::OPT_shared)) {
    CmdArgs.push_back("-shared");
  }

  // Add libunwind.
  {
    SmallString<128> Path(LibDir);
    llvm::sys::path::append(Path, "libunwind");
    auto LibPath = (Path + ".a").str();
    auto SoPath = (Path + ".so").str();
    if (Args.hasArg(options::OPT_static)) {
      if (TC.getVFS().exists(LibPath)) {
        CmdArgs.push_back(Args.MakeArgString(LibPath));
      }
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

Musl::Musl(const Driver &D, const llvm::Triple &Triple, const ArgList &Args)
    : ToolChain(D, Triple, Args) {
  getProgramPaths().push_back(getDriver().getInstalledDir());
  if (getDriver().getInstalledDir() != getDriver().Dir)
    getProgramPaths().push_back(getDriver().Dir);

  std::string SysRoot = computeSysRoot();
  path_list &FilePaths = getFilePaths();
  addPathIfExists(D, SysRoot + "/lib", FilePaths);
}

Musl::~Musl() {}

void Musl::printVerboseInfo(raw_ostream &OS) const {}

bool Musl::IsUnwindTablesDefault(const ArgList &Args) const { return false; }

Tool *Musl::buildLinker() const { return new tools::musl::Linker(*this); }

void Musl::AddClangSystemIncludeArgs(const ArgList &DriverArgs,
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

static std::string DetectLibcxxIncludePath(llvm::vfs::FileSystem &vfs,
                                           StringRef base) {
  std::error_code EC;
  int MaxVersion = 0;
  std::string MaxVersionString;
  for (llvm::vfs::directory_iterator LI = vfs.dir_begin(base, EC), LE;
       !EC && LI != LE; LI = LI.increment(EC)) {
    StringRef VersionText = llvm::sys::path::filename(LI->path());
    int Version;
    if (VersionText[0] == 'v' &&
        !VersionText.slice(1, StringRef::npos).getAsInteger(10, Version)) {
      if (Version > MaxVersion) {
        MaxVersion = Version;
        MaxVersionString = std::string(VersionText);
      }
    }
  }
  return MaxVersion ? (base + "/" + MaxVersionString).str() : "";
}

void Musl::AddClangCXXStdlibIncludeArgs(const ArgList &DriverArgs,
                                        ArgStringList &CC1Args) const {
  if (DriverArgs.hasArg(options::OPT_nostdlibinc) ||
      DriverArgs.hasArg(options::OPT_nostdincxx))
    return;

  switch (GetCXXStdlibType(DriverArgs)) {
  case ToolChain::CST_Libcxx: {
    std::string Path = computeSysRoot() + "/include/c++";
    std::string IncludePath = DetectLibcxxIncludePath(getVFS(), Path);
    if (IncludePath.empty() || !getVFS().exists(IncludePath))
      return;
    addSystemInclude(DriverArgs, CC1Args, IncludePath);
    break;
  }

  case ToolChain::CST_Libstdcxx:
    llvm_unreachable("not implemented");
  }
}

void Musl::AddCXXStdlibLibArgs(const ArgList &Args,
                               ArgStringList &CmdArgs) const {
  CXXStdlibType Type = GetCXXStdlibType(Args);
  switch (Type) {
  case ToolChain::CST_Libcxx:
    if (getVFS().exists(computeSysRoot() + "/lib/libc++.so")) {
      CmdArgs.push_back("-lc++");
    }
    if (getVFS().exists(computeSysRoot() + "/lib/libc++abi.so")) {
      CmdArgs.push_back("-lc++abi");
    }
    break;

  case ToolChain::CST_Libstdcxx:
    llvm_unreachable("not implemented");
  }
}

std::string Musl::computeSysRoot() const {
  std::string SysRoot = getDriver().SysRoot;
  if (!SysRoot.empty()) {
    return SysRoot;
  }
  return std::string();
}

ToolChain::RuntimeLibType Musl::GetRuntimeLibType(
    const ArgList &Args) const {
  if (Arg *A = Args.getLastArg(clang::driver::options::OPT_rtlib_EQ)) {
    StringRef Value = A->getValue();
    if (Value != "compiler-rt")
      getDriver().Diag(clang::diag::err_drv_invalid_rtlib_name)
          << A->getAsString(Args);
  }

  return ToolChain::RLT_CompilerRT;
}
