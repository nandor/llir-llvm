//===-- LLIRMCTargetDesc.cpp - LLIR Target Descriptions -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides LLIR specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "LLIRMCTargetDesc.h"

#include "LLIRMCAsmInfo.h"
#include "MCTargetDesc/LLIRInstPrinter.h"
#include "MCTargetDesc/LLIRMCTargetStreamer.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "LLIRGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "LLIRGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "LLIRGenRegisterInfo.inc"

static MCAsmInfo *createLLIRMCAsmInfo(const MCRegisterInfo &MRI,
                                      const Triple &TT,
                                      const MCTargetOptions &TO) {
  return new LLIRMCAsmInfo(TT);
}

static MCInstPrinter *createLLIRMCInstPrinter(const Triple &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new LLIRInstPrinter(MAI, MII, MRI);
}

static MCTargetStreamer *createObjectTargetStreamer(
    MCStreamer &S, const MCSubtargetInfo &STI) {
  return new LLIRMCTargetLLIRStreamer(S);
}

static MCTargetStreamer *createAsmTargetStreamer(MCStreamer &S,
                                                 formatted_raw_ostream &OS,
                                                 MCInstPrinter *InstPrinter,
                                                 bool isVerboseAsm) {
  return new LLIRMCTargetAsmStreamer(S, OS);
}

extern "C" void LLVMInitializeLLIRTargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfoFn X(getTheLLIRTarget(), createLLIRMCAsmInfo);

  // Register stuff for all targets and subtargets.
  for (Target *T : {&getTheLLIRTarget()}) {
    TargetRegistry::RegisterMCInstPrinter(*T, createLLIRMCInstPrinter);
    TargetRegistry::RegisterMCCodeEmitter(*T, createLLIRMCCodeEmitter);
    TargetRegistry::RegisterMCAsmBackend(*T, createLLIRAsmBackend);
    TargetRegistry::RegisterObjectTargetStreamer(*T,
                                                 createObjectTargetStreamer);
    TargetRegistry::RegisterAsmTargetStreamer(*T, createAsmTargetStreamer);
  }
}
