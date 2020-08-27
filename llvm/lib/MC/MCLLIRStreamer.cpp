//===- lib/MC/MCLLIRStreamer.cpp - LLIR Object Output ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file assembles .s files and emits LLIR .o object files.
//
//===----------------------------------------------------------------------===//

#include "llvm/MC/MCLLIRStreamer.h"

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
#include "llvm/MC/MCSectionLLIR.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MCSymbolLLIR.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

MCLLIRStreamer::~MCLLIRStreamer() {}

void MCLLIRStreamer::reset() { MCObjectStreamer::reset(); }

void MCLLIRStreamer::emitFileDirective(StringRef Filename) {}

void MCLLIRStreamer::emitLabel(MCSymbol *Symbol, SMLoc Loc) {
  llvm_unreachable("MCLLIRStreamer::emitLabel");
}

void MCLLIRStreamer::changeSection(MCSection *Section,
                                   const MCExpr *Subsection) {
  MCAssembler &Asm = getAssembler();
  this->MCObjectStreamer::changeSection(Section, Subsection);
  if (auto *Sym = Section->getBeginSymbol()) {
    Asm.registerSymbol(*Sym);
  }
}

void MCLLIRStreamer::emitInstruction(const MCInst &Inst,
                                     const MCSubtargetInfo &STI) {
  llvm_unreachable("MCLLIRStreamer::emitInstruction");
}

void MCLLIRStreamer::finishImpl() {
  llvm_unreachable("MCLLIRStreamer::finishImpl");
}

bool MCLLIRStreamer::emitSymbolAttribute(MCSymbol *Symbol,
                                         MCSymbolAttr Attribute) {
  llvm_unreachable("MCLLIRStreamer::emitSymbolAttribute");
}

void MCLLIRStreamer::emitCFISections(bool EH, bool Debug) {
  llvm_unreachable("MCLLIRStreamer::emitCFISections");
}

void MCLLIRStreamer::visitUsedSymbol(const MCSymbol &Sym) {
  llvm_unreachable("MCLLIRStreamer::visitUsedSymbol");
}

void MCLLIRStreamer::emitAssignment(MCSymbol *Symbol, const MCExpr *Value) {
  llvm_unreachable("MCLLIRStreamer::emitAssignment");
}

void MCLLIRStreamer::emitValueImpl(const MCExpr *Value, unsigned Size,
                                   SMLoc Loc) {
  llvm_unreachable("MCLLIRStreamer::emitValueImpl");
}

void MCLLIRStreamer::emitULEB128Value(const MCExpr *Value) {
  llvm_unreachable("MCLLIRStreamer::emitULEB128Value");
}

void MCLLIRStreamer::emitSLEB128Value(const MCExpr *Value) {
  llvm_unreachable("MCLLIRStreamer::emitSLEB128Value");
}

void MCLLIRStreamer::emitWeakReference(MCSymbol *Alias,
                                       const MCSymbol *Symbol) {
  llvm_unreachable("MCLLIRStreamer::emitWeakReference");
}

void MCLLIRStreamer::emitBundleAlignMode(unsigned AlignPow2) {
  llvm_unreachable("MCLLIRStreamer::emitBundleAlignMode");
}

void MCLLIRStreamer::emitBundleLock(bool AlignToEnd) {
  llvm_unreachable("MCLLIRStreamer::emitBundleLock");
}

void MCLLIRStreamer::emitBundleUnlock() {
  llvm_unreachable("MCLLIRStreamer::emitBundleUnlock");
}

void MCLLIRStreamer::emitBytes(StringRef Data) {
  llvm_unreachable("MCLLIRStreamer::emitBytes");
}

void MCLLIRStreamer::emitValueToAlignment(unsigned ByteAlignment, int64_t Value,
                                          unsigned ValueSize,
                                          unsigned MaxBytesToEmit) {
  llvm_unreachable("MCLLIRStreamer::emitValueToAlignment");
}

void MCLLIRStreamer::emitCodeAlignment(unsigned ByteAlignment,
                                       unsigned MaxBytesToEmit) {
  llvm_unreachable("MCLLIRStreamer::emitCodeAlignment");
}

void MCLLIRStreamer::emitValueToOffset(const MCExpr *Offset,
                                       unsigned char Value, SMLoc Loc) {
  llvm_unreachable("MCLLIRStreamer::emitValueToOffset");
}

void MCLLIRStreamer::emitDwarfLocDirective(unsigned FileNo, unsigned Line,
                                           unsigned Column, unsigned Flags,
                                           unsigned Isa, unsigned Discriminator,
                                           StringRef FileName) {
  llvm_unreachable("MCLLIRStreamer::emitDwarfLocDirective");
}

void MCLLIRStreamer::emitCVLocDirective(unsigned FunctionId, unsigned FileNo,
                                        unsigned Line, unsigned Column,
                                        bool PrologueEnd, bool IsStmt,
                                        StringRef FileName, SMLoc Loc) {
  llvm_unreachable("MCLLIRStreamer::emitCVLocDirective");
}

void MCLLIRStreamer::emitCVLinetableDirective(unsigned FunctionId,
                                              const MCSymbol *Begin,
                                              const MCSymbol *End) {
  llvm_unreachable("MCLLIRStreamer::emitCVLinetableDirective");
}

void MCLLIRStreamer::emitCVInlineLinetableDirective(unsigned PrimaryFunctionId,
                                                    unsigned SourceFileId,
                                                    unsigned SourceLineNum,
                                                    const MCSymbol *FnStartSym,
                                                    const MCSymbol *FnEndSym) {
  llvm_unreachable("MCLLIRStreamer::emitCVInlineLinetableDirective");
}

void MCLLIRStreamer::emitCVDefRangeDirective(
    ArrayRef<std::pair<const MCSymbol *, const MCSymbol *>> Ranges,
    StringRef FixedSizePortion) {
  llvm_unreachable("MCLLIRStreamer::emitCVDefRangeDirective");
}

void MCLLIRStreamer::emitCVStringTableDirective() {
  llvm_unreachable("MCLLIRStreamer::emitCVStringTableDirective");
}

void MCLLIRStreamer::emitCVFileChecksumsDirective() {
  llvm_unreachable("MCLLIRStreamer::emitCVFileChecksumsDirective");
}

void MCLLIRStreamer::emitCVFileChecksumOffsetDirective(unsigned FileNo) {
  llvm_unreachable("MCLLIRStreamer::emitCVFileChecksumOffsetDirective");
}

void MCLLIRStreamer::emitDTPRel32Value(const MCExpr *Value) {
  llvm_unreachable("MCLLIRStreamer::emitDTPRel32Value");
}

void MCLLIRStreamer::emitDTPRel64Value(const MCExpr *Value) {
  llvm_unreachable("MCLLIRStreamer::emitDTPRel64Value");
}

void MCLLIRStreamer::emitTPRel32Value(const MCExpr *Value) {
  llvm_unreachable("MCLLIRStreamer::emitTPRel32Value");
}

void MCLLIRStreamer::emitTPRel64Value(const MCExpr *Value) {
  llvm_unreachable("MCLLIRStreamer::emitTPRel64Value");
}

void MCLLIRStreamer::emitGPRel32Value(const MCExpr *Value) {
  llvm_unreachable("MCLLIRStreamer::emitGPRel32Value");
}

void MCLLIRStreamer::emitGPRel64Value(const MCExpr *Value) {
  llvm_unreachable("MCLLIRStreamer::emitGPRel64Value");
}

void MCLLIRStreamer::emitFill(const MCExpr &NumBytes, uint64_t FillValue,
                              SMLoc Loc) {
  llvm_unreachable("MCLLIRStreamer::emitFill");
}

void MCLLIRStreamer::emitFill(const MCExpr &NumValues, int64_t Size,
                              int64_t Expr, SMLoc Loc) {
  llvm_unreachable("MCLLIRStreamer::emitFill");
}

void MCLLIRStreamer::emitAddrsig() {
  llvm_unreachable("MCLLIRStreamer::emitAddrsig");
}

void MCLLIRStreamer::emitAddrsigSym(const MCSymbol *Sym) {
  llvm_unreachable("MCLLIRStreamer::emitAddrsigSym");
}

void MCLLIRStreamer::emitAbsoluteSymbolDiff(const MCSymbol *Hi,
                                            const MCSymbol *Lo, unsigned Size) {
  llvm_unreachable("MCLLIRStreamer::emitAbsoluteSymbolDiff");
}

void MCLLIRStreamer::emitAbsoluteSymbolDiffAsULEB128(const MCSymbol *Hi,
                                                     const MCSymbol *Lo) {
  llvm_unreachable("MCLLIRStreamer::emitAbsoluteSymbolDiffAsULEB128");
}

bool MCLLIRStreamer::mayHaveInstructions(MCSection &Sec) const {
  llvm_unreachable("MCLLIRStreamer::mayHaveInstructions");
}

void MCLLIRStreamer::emitCommonSymbol(MCSymbol *Symbol, uint64_t Size,
                                      unsigned ByteAlignment) {
  llvm_unreachable("MCLLIRStreamer::emitCommonSymbol");
}

void MCLLIRStreamer::emitZerofill(MCSection *Section, MCSymbol *Symbol,
                                  uint64_t Size, unsigned ByteAlignment,
                                  SMLoc Loc) {
  llvm_unreachable("MCLLIRStreamer::emitZerofill");
}

void MCLLIRStreamer::emitInstToFragment(const MCInst &Inst,
                                        const MCSubtargetInfo &) {
  llvm_unreachable("MCLLIRStreamer::emitInstToFragment");
}

void MCLLIRStreamer::emitInstToData(const MCInst &Inst,
                                    const MCSubtargetInfo &) {
  llvm_unreachable("MCLLIRStreamer::emitInstToData");
}

MCStreamer *llvm::createLLIRStreamer(MCContext &Context,
                                     std::unique_ptr<MCAsmBackend> &&MAB,
                                     std::unique_ptr<MCObjectWriter> &&OW,
                                     std::unique_ptr<MCCodeEmitter> &&CE,
                                     bool RelaxAll) {
  MCLLIRStreamer *S =
      new MCLLIRStreamer(Context, std::move(MAB), std::move(OW), std::move(CE));
  if (RelaxAll) {
    S->getAssembler().setRelaxAll(true);
  }
  return S;
}
