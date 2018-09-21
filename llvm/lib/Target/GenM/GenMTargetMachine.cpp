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

  bool addInstSelector() override
  {
    addPass(createGenMISelDag(getGenMTargetMachine()));
    return false;
  }
};

} // namespace

extern "C" void LLVMInitializeGenMTarget() {
  // Register the target.
  RegisterTargetMachine<GenMTargetMachine> X(getTheGenMTarget());
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
        "e-p:64:64",
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
  assert(!"not implemented");
}
