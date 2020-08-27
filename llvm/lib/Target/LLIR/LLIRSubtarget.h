//===-- LLIRSubtarget.h - Define Subtarget for the LLIR ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the LLIR specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIRSUBTARGET_H
#define LLVM_LIB_TARGET_LLIR_LLIRSUBTARGET_H

#include <string>

#include "LLIRFrameLowering.h"
#include "LLIRISelLowering.h"
#include "LLIRInstrInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"

#define GET_SUBTARGETINFO_HEADER
#include "LLIRGenSubtargetInfo.inc"

namespace llvm {

class LLIRSubtarget : public LLIRGenSubtargetInfo {
 public:
  LLIRSubtarget(const Triple &TT, const std::string &CPU, const std::string &FS,
                const TargetMachine &TM);

  const LLIRRegisterInfo *getRegisterInfo() const override {
    return &RegisterInfo;
  }

  const LLIRInstrInfo *getInstrInfo() const override { return &InstrInfo; }

  const TargetFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }

  const LLIRTargetLowering *getTargetLowering() const override {
    return &TargetLowering;
  }

  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &DAGTargetInfo;
  }

  bool enableMachineScheduler() const override;

 private:
  void ParseSubtargetFeatures(StringRef CPU, StringRef TrueCPU, StringRef FS);

 private:
  Triple TargetTriple;

  LLIRRegisterInfo RegisterInfo;
  LLIRInstrInfo InstrInfo;
  LLIRTargetLowering TargetLowering;
  LLIRFrameLowering FrameLowering;
  SelectionDAGTargetInfo DAGTargetInfo;
};

}  // end namespace llvm

#endif
