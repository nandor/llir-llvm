//===-- LLIRAsmBackend.cpp - LLIR Assembler Backend -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/LLIRMCTargetDesc.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

namespace {

class LLIRAsmBackend : public MCAsmBackend {
protected:
  const Target &TheTarget;

public:
  LLIRAsmBackend(const Target &T)
      : MCAsmBackend(support::little)
      , TheTarget(T)
  {
  }

  unsigned getNumFixupKinds() const override
  {
    return 0;
  }

  void applyFixup(
      const MCAssembler &Asm,
      const MCFixup &Fixup,
      const MCValue &Target,
      MutableArrayRef<char> Data,
      uint64_t Value,
      bool IsResolved,
      const MCSubtargetInfo *STI) const override
  {
    llvm_unreachable("applyFixup() unimplemented");
  }

  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override
  {
    llvm_unreachable("getFixupKindInfo() unimplemented");
  }

  bool shouldForceRelocation(
      const MCAssembler &Asm,
      const MCFixup &Fixup,
      const MCValue &Target) override
  {
    llvm_unreachable("shouldForceRelocation() unimplemented");
  }

  bool mayNeedRelaxation(
      const MCInst &Inst,
      const MCSubtargetInfo &STI) const override
  {
    llvm_unreachable("mayNeedRelaxation() unimplemented");
  }

  bool fixupNeedsRelaxation(
      const MCFixup &Fixup,
      uint64_t Value,
      const MCRelaxableFragment *DF,
      const MCAsmLayout &Layout) const override
  {
    llvm_unreachable("fixupNeedsRelaxation() unimplemented");
  }

  void relaxInstruction(
      const MCInst &Inst,
      const MCSubtargetInfo &STI,
      MCInst &Res) const override
  {
    llvm_unreachable("relaxInstruction() unimplemented");
  }

  bool writeNopData(raw_ostream &OS, uint64_t Count) const override
  {
    llvm_unreachable("writeNopData() unimplemented");
  }

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override
  {
    return createLLIRObjectWriter();
  }
};

} // end anonymous namespace

MCAsmBackend *llvm::createLLIRAsmBackend(
    const Target &T,
    const MCSubtargetInfo &STI,
    const MCRegisterInfo &MRI,
    const MCTargetOptions &Options)
{
  return new LLIRAsmBackend(T);
}
