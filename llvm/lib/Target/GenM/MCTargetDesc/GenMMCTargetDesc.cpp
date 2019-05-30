//===-- GenMMCTargetDesc.cpp - GenM Target Descriptions -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides GenM specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "GenMMCTargetDesc.h"
#include "GenMMCAsmInfo.h"
#include "InstPrinter/GenMInstPrinter.h"
#include "MCTargetDesc/GenMMCTargetStreamer.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "GenMGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "GenMGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "GenMGenRegisterInfo.inc"

static MCAsmInfo *createGenMMCAsmInfo(
    const MCRegisterInfo &MRI,
    const Triple &TT)
{
  return new GenMELFMCAsmInfo(TT);
}


static MCInstPrinter *createGenMMCInstPrinter(
    const Triple &T,
    unsigned SyntaxVariant,
    const MCAsmInfo &MAI,
    const MCInstrInfo &MII,
    const MCRegisterInfo &MRI)
{
  return new GenMInstPrinter(MAI, MII, MRI);
}

static MCTargetStreamer *createObjectTargetStreamer(
    MCStreamer &S,
    const MCSubtargetInfo &STI)
{
  return new GenMMCTargetGenMStreamer(S);
}

static MCTargetStreamer *createAsmTargetStreamer(
    MCStreamer &S,
    formatted_raw_ostream &OS,
    MCInstPrinter *InstPrinter,
    bool isVerboseAsm)
{
  return new GenMMCTargetAsmStreamer(S, OS);
}

extern "C" void LLVMInitializeGenMTargetMC()
{
  // Register the MC asm info.
  RegisterMCAsmInfoFn X(getTheGenMTarget(), createGenMMCAsmInfo);

  // Register stuff for all targets and subtargets.
  for (Target *T : {&getTheGenMTarget()}) {
    TargetRegistry::RegisterMCInstPrinter(*T, createGenMMCInstPrinter);
    TargetRegistry::RegisterMCCodeEmitter(*T, createGenMMCCodeEmitter);
    TargetRegistry::RegisterMCAsmBackend(*T, createGenMAsmBackend);
    TargetRegistry::RegisterObjectTargetStreamer(*T, createObjectTargetStreamer);
    TargetRegistry::RegisterAsmTargetStreamer(*T, createAsmTargetStreamer);
  }
}
