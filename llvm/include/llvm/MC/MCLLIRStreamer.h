//===- MCLLIRStreamer.h - MCStreamer LLIR Object File Interface -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_MC_MCLLIRSTREAMER_H
#define LLVM_MC_MCLLIRSTREAMER_H

#include "MCAsmBackend.h"
#include "MCCodeEmitter.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/MC/MCDirectives.h"
#include "llvm/MC/MCObjectStreamer.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/SectionKind.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {
class MCAssembler;
class MCExpr;
class MCInst;
class raw_ostream;

class MCLLIRStreamer : public MCObjectStreamer {
 public:
  MCLLIRStreamer(MCContext &Context, std::unique_ptr<MCAsmBackend> TAB,
                 std::unique_ptr<MCObjectWriter> OW,
                 std::unique_ptr<MCCodeEmitter> Emitter)
      : MCObjectStreamer(Context, std::move(TAB), std::move(OW),
                         std::move(Emitter)) {}

  ~MCLLIRStreamer() override;

 public:
  bool isIntegratedAssemblerRequired() const override { return true; };

 public:
  bool emitSymbolAttribute(MCSymbol *Symbol, MCSymbolAttr Attribute) override;
  void emitAbsoluteSymbolDiff(const MCSymbol *Hi, const MCSymbol *Lo,
                              unsigned Size) override;
  void emitAbsoluteSymbolDiffAsULEB128(const MCSymbol *Hi,
                                       const MCSymbol *Lo) override;
  void emitAddrsig() override;
  void emitAddrsigSym(const MCSymbol *Sym) override;
  void emitAssignment(MCSymbol *Symbol, const MCExpr *Value) override;
  void emitBundleAlignMode(unsigned AlignPow2) override;
  void emitBundleLock(bool AlignToEnd) override;
  void emitBundleUnlock() override;
  void emitBytes(StringRef Data) override;
  void emitCFISections(bool EH, bool Debug) override;
  void emitCodeAlignment(unsigned ByteAlignment,
                         unsigned MaxBytesToEmit = 0) override;
  void emitCommonSymbol(MCSymbol *Symbol, uint64_t Size,
                        unsigned ByteAlignment) override;
  void emitCVDefRangeDirective(
      ArrayRef<std::pair<const MCSymbol *, const MCSymbol *>> Ranges,
      StringRef FixedSizePortion) override;
  void emitCVFileChecksumOffsetDirective(unsigned FileNo) override;
  void emitCVFileChecksumsDirective() override;
  void emitCVInlineLinetableDirective(unsigned PrimaryFunctionId,
                                      unsigned SourceFileId,
                                      unsigned SourceLineNum,
                                      const MCSymbol *FnStartSym,
                                      const MCSymbol *FnEndSym) override;
  void emitCVLinetableDirective(unsigned FunctionId, const MCSymbol *Begin,
                                const MCSymbol *End) override;
  void emitCVLocDirective(unsigned FunctionId, unsigned FileNo, unsigned Line,
                          unsigned Column, bool PrologueEnd, bool IsStmt,
                          StringRef FileName, SMLoc Loc) override;
  void emitCVStringTableDirective() override;
  void emitDTPRel32Value(const MCExpr *Value) override;
  void emitDTPRel64Value(const MCExpr *Value) override;
  void emitDwarfLocDirective(unsigned FileNo, unsigned Line, unsigned Column,
                             unsigned Flags, unsigned Isa,
                             unsigned Discriminator,
                             StringRef FileName) override;
  void emitFileDirective(StringRef Filename) override;
  void emitFill(const MCExpr &NumBytes, uint64_t FillValue,
                SMLoc Loc = SMLoc()) override;
  void emitFill(const MCExpr &NumValues, int64_t Size, int64_t Expr,
                SMLoc Loc = SMLoc()) override;
  void emitGPRel32Value(const MCExpr *Value) override;
  void emitGPRel64Value(const MCExpr *Value) override;
  void emitInstruction(const MCInst &Inst, const MCSubtargetInfo &STI) override;
  void emitLabel(MCSymbol *Symbol, SMLoc Loc = SMLoc()) override;
  void emitSLEB128Value(const MCExpr *Value) override;
  void emitTPRel32Value(const MCExpr *Value) override;
  void emitTPRel64Value(const MCExpr *Value) override;
  void emitULEB128Value(const MCExpr *Value) override;
  void emitValueImpl(const MCExpr *Value, unsigned Size,
                     SMLoc Loc = SMLoc()) override;
  void emitValueToAlignment(unsigned ByteAlignment, int64_t Value = 0,
                            unsigned ValueSize = 1,
                            unsigned MaxBytesToEmit = 0) override;
  void emitValueToOffset(const MCExpr *Offset, unsigned char Value,
                         SMLoc Loc) override;
  void emitWeakReference(MCSymbol *Alias, const MCSymbol *Symbol) override;
  void emitZerofill(MCSection *Section, MCSymbol *Symbol = nullptr,
                    uint64_t Size = 0, unsigned ByteAlignment = 0,
                    SMLoc Loc = SMLoc()) override;

  bool mayHaveInstructions(MCSection &Sec) const override;
  void changeSection(MCSection *Section, const MCExpr *Subsection) override;
  void finishImpl() override;
  void reset() override;
  void visitUsedSymbol(const MCSymbol &Sym) override;

 private:
  void emitInstToFragment(const MCInst &Inst, const MCSubtargetInfo &) override;
  void emitInstToData(const MCInst &Inst, const MCSubtargetInfo &) override;
};

}  // end namespace llvm

#endif
