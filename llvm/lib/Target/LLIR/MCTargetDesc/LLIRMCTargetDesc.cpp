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

static MCInstPrinter *createMCInstPrinter(const Triple &T,
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

static MCSubtargetInfo *createMCSubtargetInfo(const Triple &TT, StringRef CPU,
                                              StringRef FS) {
  return createLLIRMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

extern "C" void LLVMInitializeLLIRTargetMC() {
  // Register stuff for all targets and subtargets.
  for (Target *T : {&getTheLLIR_X86_64Target(), &getTheLLIR_AArch64Target()}) {
    // Register the MC asm info.
    RegisterMCAsmInfoFn X(*T, createLLIRMCAsmInfo);

    // Register the MCInstPrinter.
    TargetRegistry::RegisterMCInstPrinter(*T, createMCInstPrinter);

    // Register the ASM Backend.
    TargetRegistry::RegisterMCAsmBackend(*T, createLLIRAsmBackend);

    // Register the MC subtarget info.
    TargetRegistry::RegisterMCSubtargetInfo(*T, createMCSubtargetInfo);

    // Register the object target streamer.
    TargetRegistry::RegisterObjectTargetStreamer(*T,
                                                 createObjectTargetStreamer);

    // Register the asm target streamer.
    TargetRegistry::RegisterAsmTargetStreamer(*T, createAsmTargetStreamer);
  }
}
