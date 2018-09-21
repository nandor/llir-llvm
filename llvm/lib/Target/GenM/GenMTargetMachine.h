//===-- GenMTargetMachine.h - Define TargetMachine for GenM -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the GenM specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_GENMTARGETMACHINE_H
#define LLVM_LIB_TARGET_GENM_GENMTARGETMACHINE_H

#include "GenMRegisterInfo.h"
#include "GenMSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class GenMTargetMachine : public LLVMTargetMachine {
public:
  GenMTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, const TargetOptions &Options,
                     Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                     CodeGenOpt::Level OL, bool JIT);
  ~GenMTargetMachine() override;

  const GenMSubtarget *getSubtargetImpl(const Function &) const override;

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override;

  bool isMachineVerifierClean() const override;

private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  mutable StringMap<std::unique_ptr<GenMSubtarget>> SubtargetMap;
};

} // end namespace llvm

#endif
