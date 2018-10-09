//===-- GenMTargetMachine.cpp - Define TargetMachine for GenM -----------===//
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

#include "GenMTargetMachine.h"
#include "GenM.h"
#include "GenMTargetObjectFile.h"
#include "MCTargetDesc/GenMMCTargetDesc.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

namespace {

/// GenM Code Generator Pass Configuration Options.
class GenMPassConfig : public TargetPassConfig {
public:
  GenMPassConfig(GenMTargetMachine &TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

  GenMTargetMachine &getGenMTargetMachine() const
  {
    return getTM<GenMTargetMachine>();
  }

  FunctionPass *createTargetRegisterAllocator(bool) override
  {
    return nullptr;
  }

  bool addInstSelector() override
  {
    TargetPassConfig::addInstSelector();
    addPass(createGenMISelDag(getGenMTargetMachine()));
    return false;
  }

  void addIRPasses() override {
    TargetPassConfig::addIRPasses();
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

  void addPreEmitPass() override {
    TargetPassConfig::addPreEmitPass();
    addPass(createGenMRegisterNumbering());
  }
};

} // namespace

extern "C" void LLVMInitializeGenMTarget()
{
  // Register the target.
  RegisterTargetMachine<GenMTargetMachine> X(getTheGenMTarget());

  // Register backend passes.
  auto &PR = *PassRegistry::getPassRegistry();
  initializeGenMRegisterNumberingPass(PR);
}

static std::string computeDataLayout(
    const Triple &TT,
    StringRef CPU,
    StringRef FS)
{
  // TODO(nand):
  //  This should be computed based on the CPU, FS and triple.
  //  This value is hardcoded for x86_64.
  return "e-m:o-i64:64-f80:128-n8:16:32:64-S128";
}

GenMTargetMachine::GenMTargetMachine(
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
    TLOF(make_unique<GenMTargetObjectFile>())
{
  initAsmInfo();
}

GenMTargetMachine::~GenMTargetMachine()
{
}

const GenMSubtarget *
GenMTargetMachine::getSubtargetImpl(const Function &F) const
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
    I = llvm::make_unique<GenMSubtarget>(
        TargetTriple,
        CPU,
        FS,
        *this
    );
  }
  return I.get();
}

TargetPassConfig *GenMTargetMachine::createPassConfig(PassManagerBase &PM)
{
  return new GenMPassConfig(*this, PM);
}

TargetLoweringObjectFile *GenMTargetMachine::getObjFileLowering() const
{
  return TLOF.get();
}

bool GenMTargetMachine::isMachineVerifierClean() const
{
  llvm_unreachable("not implemented");
}

bool GenMTargetMachine::usesPhysRegsForPEI() const
{
  return false;
}
