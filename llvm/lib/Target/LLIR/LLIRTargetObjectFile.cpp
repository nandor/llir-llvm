//===------- LLIRTargetObjectFile.cpp - LLIR Object Info Impl -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "LLIRTargetObjectFile.h"
#include "llvm/Target/TargetMachine.h"
using namespace llvm;

void LLIRELFTargetObjectFile::Initialize(
    MCContext &Ctx,
    const TargetMachine &TM)
{
  TargetLoweringObjectFileLLIR::Initialize(Ctx, TM);
}

const MCExpr *LLIRELFTargetObjectFile::getTTypeGlobalReference(
    const GlobalValue *GV,
    unsigned Encoding,
    const TargetMachine &TM,
    MachineModuleInfo *MMI,
    MCStreamer &Streamer) const
{
  llvm_unreachable("getTTypeGlobalReference");
}
