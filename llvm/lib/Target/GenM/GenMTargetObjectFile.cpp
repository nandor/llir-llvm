//===------- GenMTargetObjectFile.cpp - GenM Object Info Impl -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "GenMTargetObjectFile.h"
#include "llvm/Target/TargetMachine.h"
using namespace llvm;

void GenMTargetObjectFile::Initialize(
    MCContext &Ctx,
    const TargetMachine &TM)
{
  TargetLoweringObjectFileELF::Initialize(Ctx, TM);
  InitializeELF(TM.Options.UseInitArray);
}

const MCExpr *GenMTargetObjectFile::getTTypeGlobalReference(
    const GlobalValue *GV,
    unsigned Encoding,
    const TargetMachine &TM,
    MachineModuleInfo *MMI,
    MCStreamer &Streamer) const
{
  assert(!"not implemented");
}
