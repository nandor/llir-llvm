//===-- GenMSubtarget.cpp - SPARC Subtarget Information -------------------===//
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

#include "GenMSubtarget.h"
#include "GenM.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "genm-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "GenMGenSubtargetInfo.inc"


GenMSubtarget::GenMSubtarget(
    const Triple &TT,
    const std::string &CPU,
    const std::string &FS,
    const TargetMachine &TM)
  : GenMGenSubtargetInfo(TT, CPU, FS)
  , TargetTriple(TT)
  , InstrInfo(*this)
  , TargetLowering(TM, *this)
  , FrameLowering(*this)
{
  ParseSubtargetFeatures(CPU, FS);
}

bool GenMSubtarget::enableMachineScheduler() const
{
  assert(!"not implemented");
}
