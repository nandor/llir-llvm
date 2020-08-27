//===-- LLIRMCCodeEmitter.cpp - Convert LLIR code to machine code ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the LLIRMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include <cassert>
#include <cstdint>

#include "LLIRMCTargetDesc.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/SubtargetFeature.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Endian.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

namespace {

class LLIRMCCodeEmitter : public MCCodeEmitter {
 public:
  LLIRMCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx)
      : MCII(MCII), Ctx(Ctx) {}

  ~LLIRMCCodeEmitter() override = default;

  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getCallTargetOpValue(const MCInst &MI, unsigned OpNo,
                                SmallVectorImpl<MCFixup> &Fixups,
                                const MCSubtargetInfo &STI) const;

  unsigned getBranchTargetOpValue(const MCInst &MI, unsigned OpNo,
                                  SmallVectorImpl<MCFixup> &Fixups,
                                  const MCSubtargetInfo &STI) const;

  unsigned getBranchPredTargetOpValue(const MCInst &MI, unsigned OpNo,
                                      SmallVectorImpl<MCFixup> &Fixups,
                                      const MCSubtargetInfo &STI) const;

  unsigned getBranchOnRegTargetOpValue(const MCInst &MI, unsigned OpNo,
                                       SmallVectorImpl<MCFixup> &Fixups,
                                       const MCSubtargetInfo &STI) const;

 private:
  FeatureBitset computeAvailableFeatures(const FeatureBitset &FB) const;

  void verifyInstructionPredicates(
      const MCInst &MI, const FeatureBitset &AvailableFeatures) const;

 private:
  const MCInstrInfo &MCII;
  MCContext &Ctx;
};

}  // end anonymous namespace

void LLIRMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {
  llvm_unreachable("encodeInstruction() unimplemented");
}

#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "LLIRLLIRCCodeEmitter.inc"

MCCodeEmitter *llvm::createLLIRMCCodeEmitter(const MCInstrInfo &MCII,
                                             const MCRegisterInfo &MRI,
                                             MCContext &Ctx) {
  return new LLIRMCCodeEmitter(MCII, Ctx);
}
