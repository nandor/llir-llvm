//===-- LLIRSubtarget.cpp - SPARC Subtarget Information -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the SPARC specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "LLIRSubtarget.h"
#include "LLIR.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "llir-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "LLIRGenSubtargetInfo.inc"


LLIRSubtarget::LLIRSubtarget(
    const Triple &TT,
    const std::string &CPU,
    const std::string &FS,
    const TargetMachine &TM)
  : LLIRGenSubtargetInfo(TT, CPU, /*TrueCPU*/CPU, FS)
  , TargetTriple(TT)
  , InstrInfo(*this)
  , TargetLowering(TM, *this)
  , FrameLowering(*this)
{
  ParseSubtargetFeatures(CPU, /*TrueCPU*/CPU, FS);
}

bool LLIRSubtarget::enableMachineScheduler() const
{
  return false;
}
