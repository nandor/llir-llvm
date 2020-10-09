//===-- LLIRTargetMachine.cpp - Define TargetMachine for LLIR -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "LLIRTargetMachine.h"
#include "LLIR.h"
#include "LLIRTargetObjectFile.h"
#include "MCTargetDesc/LLIRMCTargetDesc.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

namespace {

/// LLIR Code Generator Pass Configuration Options.
class LLIRPassConfig : public TargetPassConfig {
public:
  LLIRPassConfig(LLIRTargetMachine &TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

  LLIRTargetMachine &getLLIRTargetMachine() const
  {
    return getTM<LLIRTargetMachine>();
  }

  FunctionPass *createTargetRegisterAllocator(bool) override
  {
    return nullptr;
  }

  void addIRPasses() override {
    TargetPassConfig::addIRPasses();
  }

  bool addInstSelector() override
  {
    TargetPassConfig::addInstSelector();
    addPass(createLLIRISelDag(getLLIRTargetMachine()));
    addPass(createLLIRArgumentMove());
    return false;
  }

  void addPostRegAlloc() override {
    disablePass(&MachineCopyPropagationID);
    disablePass(&PostRAMachineSinkingID);
    disablePass(&PostRASchedulerID);
    disablePass(&FuncletLayoutID);
    disablePass(&StackMapLivenessID);
    disablePass(&LiveDebugValuesID);
    disablePass(&PatchableFunctionID);
    disablePass(&ShrinkWrapID);

    TargetPassConfig::addPostRegAlloc();
  }

  bool addGCPasses() override { return false; }

  void addPreEmitPass() override {
    TargetPassConfig::addPreEmitPass();
    addPass(createLLIRRegisterNumbering());
  }

  // No reg alloc
  bool addRegAssignmentFast() override { return false; }

  // No reg alloc
  bool addRegAssignmentOptimized() override { return false; }
};

} // namespace

extern "C" void LLVMInitializeLLIRTarget()
{
  // Register the target.
  RegisterTargetMachine<LLIRTargetMachine> X(getTheLLIR_X86_64Target());
  RegisterTargetMachine<LLIRTargetMachine> Y(getTheLLIR_AArch64Target());

  // Register backend passes.
  auto &PR = *PassRegistry::getPassRegistry();
  initializeLLIRRegisterNumberingPass(PR);
}

static std::string computeDataLayoutX86_64(const Triple &TT) {
  // X86 is little endian
  std::string Ret = "e";

  Ret += DataLayout::getManglingComponent(TT);
  // X86 and x32 have 32 bit pointers.
  if ((TT.isArch64Bit() &&
       (TT.getEnvironment() == Triple::GNUX32 || TT.isOSNaCl())) ||
      !TT.isArch64Bit())
    Ret += "-p:32:32";

  // Address spaces for 32 bit signed, 32 bit unsigned, and 64 bit pointers.
  Ret += "-p270:32:32-p271:32:32-p272:64:64";

  // Some ABIs align 64 bit integers and doubles to 64 bits, others to 32.
  if (TT.isArch64Bit() || TT.isOSWindows() || TT.isOSNaCl())
    Ret += "-i64:64";
  else if (TT.isOSIAMCU())
    Ret += "-i64:32-f64:32";
  else
    Ret += "-f64:32:64";

  // Some ABIs align long double to 128 bits, others to 32.
  if (TT.isOSNaCl() || TT.isOSIAMCU())
    ; // No f80
  else if (TT.isArch64Bit() || TT.isOSDarwin())
    Ret += "-f80:128";
  else
    Ret += "-f80:32";

  if (TT.isOSIAMCU())
    Ret += "-f128:32";

  // The registers can hold 8, 16, 32 or, in x86-64, 64 bits.
  if (TT.isArch64Bit())
    Ret += "-n8:16:32:64";
  else
    Ret += "-n8:16:32";

  // The stack is aligned to 32 bits on some ABIs and 128 bits on others.
  if ((!TT.isArch64Bit() && TT.isOSWindows()) || TT.isOSIAMCU())
    Ret += "-a:0:32-S32";
  else
    Ret += "-S128";

  return Ret;
}

static std::string computeDataLayout(
    const Triple &TT,
    StringRef CPU,
    StringRef FS)
{
  switch (TT.getArch()) {
    case Triple::llir_x86_64: {
      return computeDataLayoutX86_64(TT);
    }
    default: {
      llvm_unreachable("invalid LLIR target");
    }
  }
}

static std::unique_ptr<TargetLoweringObjectFile> createTLOF(const Triple &TT) {
  return std::make_unique<LLIRELFTargetObjectFile>();
}

LLIRTargetMachine::LLIRTargetMachine(
    const Target &T,
    const Triple &TT,
    StringRef CPU,
    StringRef FS,
    const TargetOptions &Options,
    Optional<Reloc::Model> RM,
    Optional<CodeModel::Model> CM,
    CodeGenOpt::Level OL,
    bool JIT)
  : LLVMTargetMachine(
        T,
        computeDataLayout(TT, CPU, FS),
        TT,
        CPU,
        FS,
        Options,
        Reloc::Static,
        CodeModel::Large,
        OL
    ),
    TLOF(createTLOF(TT))
{
  this->Options.TrapUnreachable = true;
  this->Options.NoTrapAfterNoreturn = false;
  initAsmInfo();
}

LLIRTargetMachine::~LLIRTargetMachine()
{
}

const LLIRSubtarget *
LLIRTargetMachine::getSubtargetImpl(const Function &F) const
{
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");

  std::string CPU = !CPUAttr.hasAttribute(Attribute::None)
      ? CPUAttr.getValueAsString().str()
      : TargetCPU;
  std::string FS = !FSAttr.hasAttribute(Attribute::None)
      ? FSAttr.getValueAsString().str()
      : TargetFS;

  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    // This needs to be done before we create a new subtarget since any
    // creation will depend on the TM and the code generation flags on the
    // function that reside in TargetOptions.
    resetTargetOptions(F);
    I = std::make_unique<LLIRSubtarget>(
        TargetTriple,
        CPU,
        FS,
      *this
    );
  }
  return I.get();
}

TargetPassConfig *LLIRTargetMachine::createPassConfig(PassManagerBase &PM)
{
  return new LLIRPassConfig(*this, PM);
}

TargetLoweringObjectFile *LLIRTargetMachine::getObjFileLowering() const
{
  return TLOF.get();
}

bool LLIRTargetMachine::isMachineVerifierClean() const
{
  llvm_unreachable("isMachineVerifierClean");
}
