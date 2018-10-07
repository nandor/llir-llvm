//===-- GenMSubtarget.h - Define Subtarget for the GenM ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the GenM specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_GENMSUBTARGET_H
#define LLVM_LIB_TARGET_GENM_GENMSUBTARGET_H

#include "GenMInstrInfo.h"
#include "GenMFrameLowering.h"
#include "GenMISelLowering.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "GenMGenSubtargetInfo.inc"

namespace llvm {

class GenMSubtarget : public GenMGenSubtargetInfo {
public:
  GenMSubtarget(
      const Triple &TT,
      const std::string &CPU,
      const std::string &FS,
      const TargetMachine &TM
  );

  const GenMRegisterInfo *getRegisterInfo() const override
  {
    return &RegisterInfo;
  }

  const GenMInstrInfo *getInstrInfo() const override
  {
    return &InstrInfo;
  }

  const TargetFrameLowering *getFrameLowering() const override
  {
    return &FrameLowering;
  }

  const GenMTargetLowering *getTargetLowering() const override
  {
    return &TargetLowering;
  }

  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override
  {
    return &DAGTargetInfo;
  }

  bool enableMachineScheduler() const override;

private:
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

private:
  Triple TargetTriple;

  GenMRegisterInfo RegisterInfo;
  GenMInstrInfo InstrInfo;
  GenMTargetLowering TargetLowering;
  GenMFrameLowering FrameLowering;
  SelectionDAGTargetInfo DAGTargetInfo;
};

} // end namespace llvm

#endif
