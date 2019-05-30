//===- lib/MC/MCGenMStreamer.cpp - GenM Object Output ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file assembles .s files and emits GenM .o object files.
//
//===----------------------------------------------------------------------===//

#include "llvm/MC/MCGenMStreamer.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCObjectStreamer.h"
#include "llvm/MC/MCSection.h"
#include "llvm/MC/MCSectionGenM.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MCSymbolGenM.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

MCGenMStreamer::~MCGenMStreamer() {}

void MCGenMStreamer::EmitAssemblerFlag(MCAssemblerFlag Flag)
{
  abort();
}

void MCGenMStreamer::ChangeSection(
    MCSection *Section,
    const MCExpr *Subsection)
{
  abort();
}

void MCGenMStreamer::EmitWeakReference(
    MCSymbol *Alias,
    const MCSymbol *Symbol)
{
  abort();
}

bool MCGenMStreamer::EmitSymbolAttribute(MCSymbol *S, MCSymbolAttr Attribute) {
  abort();
}

void MCGenMStreamer::EmitCommonSymbol(
    MCSymbol *S,
    uint64_t Size,
    unsigned ByteAlignment)
{
  abort();
}

void MCGenMStreamer::emitELFSize(MCSymbol *Symbol, const MCExpr *Value)
{
  abort();
}

void MCGenMStreamer::EmitLocalCommonSymbol(
    MCSymbol *S,
    uint64_t Size,
    unsigned ByteAlignment)
{
  abort();
}

void MCGenMStreamer::EmitValueImpl(
    const MCExpr *Value,
    unsigned Size,
    SMLoc Loc)
{
  abort();
}

void MCGenMStreamer::EmitValueToAlignment(
    unsigned ByteAlignment,
    int64_t Value,
    unsigned ValueSize,
    unsigned MaxBytesToEmit)
{
  abort();
}

void MCGenMStreamer::EmitIdent(StringRef IdentString)
{
  abort();
}

void MCGenMStreamer::EmitInstToFragment(
    const MCInst &Inst,
    const MCSubtargetInfo &STI)
{
  abort();
}

void MCGenMStreamer::EmitInstToData(
    const MCInst &Inst,
    const MCSubtargetInfo &STI)
{
  abort();
}

void MCGenMStreamer::FinishImpl()
{
  abort();
}

MCStreamer *llvm::createGenMStreamer(MCContext &Context,
                                     std::unique_ptr<MCAsmBackend> &&MAB,
                                     std::unique_ptr<MCObjectWriter> &&OW,
                                     std::unique_ptr<MCCodeEmitter> &&CE,
                                     bool RelaxAll) {
  MCGenMStreamer *S = new MCGenMStreamer(
      Context,
      std::move(MAB),
      std::move(OW),
      std::move(CE)
  );
  if (RelaxAll)
    S->getAssembler().setRelaxAll(true);
  return S;
}

void MCGenMStreamer::EmitThumbFunc(MCSymbol *Func) {
  abort();
}

void MCGenMStreamer::EmitSymbolDesc(MCSymbol *Symbol, unsigned DescValue) {
  abort();
}

void MCGenMStreamer::EmitZerofill(
    MCSection *Section,
    MCSymbol *Symbol,
    uint64_t Size,
    unsigned ByteAlignment,
    SMLoc Loc)
{
  abort();
}

void MCGenMStreamer::EmitTBSSSymbol(
    MCSection *Section,
    MCSymbol *Symbol,
    uint64_t Size,
    unsigned ByteAlignment)
{
  abort();
}
