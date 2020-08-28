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
  RegisterTargetMachine<LLIRTargetMachine> X(getTheLLIRTarget());

  // Register backend passes.
  auto &PR = *PassRegistry::getPassRegistry();
  initializeLLIRRegisterNumberingPass(PR);
}

static std::string computeDataLayout(
    const Triple &TT,
    StringRef CPU,
    StringRef FS)
{
  // TODO(nand):
  //  This should be computed based on the CPU, FS and triple.
  //  This value is hardcoded for x86_64.
  return "e-m:g-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128";
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
    TLOF(std::make_unique<LLIRTargetObjectFile>())
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
