//===-- LLIRTargetMachine.h - Define TargetMachine for LLIR -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the LLIR specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIRTARGETMACHINE_H
#define LLVM_LIB_TARGET_LLIR_LLIRTARGETMACHINE_H

#include "LLIRRegisterInfo.h"
#include "LLIRSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class LLIRTargetMachine : public LLVMTargetMachine {
public:
  LLIRTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, const TargetOptions &Options,
                     Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                     CodeGenOpt::Level OL, bool JIT);
  ~LLIRTargetMachine() override;

  const LLIRSubtarget *getSubtargetImpl(const Function &) const override;

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override;

  bool isMachineVerifierClean() const override;

  bool usesPhysRegsForPEI() const override;

private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  mutable StringMap<std::unique_ptr<LLIRSubtarget>> SubtargetMap;
};

} // end namespace llvm

#endif
