//===-- LLIRTargetObjectFile.h - LLIR Object Info ---------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIRTARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_LLIR_LLIRTARGETOBJECTFILE_H

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {
class MCContext;
class TargetMachine;

class LLIRTargetObjectFile : public TargetLoweringObjectFileELF {
public:
  LLIRTargetObjectFile() : TargetLoweringObjectFileELF() {}

  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;

  const MCExpr *getTTypeGlobalReference(
      const GlobalValue *GV,
      unsigned Encoding,
      const TargetMachine &TM,
      MachineModuleInfo *MMI,
      MCStreamer &Streamer
  ) const override;
};

} // end namespace llvm

#endif
