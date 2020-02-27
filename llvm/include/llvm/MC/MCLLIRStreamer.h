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
  MCLLIRStreamer(
      MCContext &Context,
      std::unique_ptr<MCAsmBackend> TAB,
      std::unique_ptr<MCObjectWriter> OW,
      std::unique_ptr<MCCodeEmitter> Emitter)
    : MCObjectStreamer(
        Context,
        std::move(TAB),
        std::move(OW),
        std::move(Emitter)
      )
  {}

  ~MCLLIRStreamer() override;

public:
  bool isIntegratedAssemblerRequired() const override { return true; };

public:
  bool EmitRelocDirective(const MCExpr &Offset, StringRef Name, const MCExpr *Expr, SMLoc Loc, const MCSubtargetInfo &STI) override;
  bool EmitSymbolAttribute(MCSymbol *Symbol, MCSymbolAttr Attribute) override;
  bool mayHaveInstructions(MCSection &Sec) const override;
  void ChangeSection(MCSection *Section, const MCExpr *Subsection) override;
  void emitAbsoluteSymbolDiff(const MCSymbol *Hi, const MCSymbol *Lo, unsigned Size) override;
  void emitAbsoluteSymbolDiffAsULEB128(const MCSymbol *Hi, const MCSymbol *Lo) override;
  void EmitAddrsig() override;
  void EmitAddrsigSym(const MCSymbol *Sym) override;
  void EmitAssignment(MCSymbol *Symbol, const MCExpr *Value) override;
  void EmitBundleAlignMode(unsigned AlignPow2) override;
  void EmitBundleLock(bool AlignToEnd) override;
  void EmitBundleUnlock() override;
  void EmitBytes(StringRef Data) override;
  void EmitCFISections(bool EH, bool Debug) override;
  void EmitCodeAlignment(unsigned ByteAlignment, unsigned MaxBytesToEmit = 0) override;
  void EmitCodePaddingBasicBlockEnd(const MCCodePaddingContext &Context) override;
  void EmitCodePaddingBasicBlockStart(const MCCodePaddingContext &Context) override;
  void EmitCommonSymbol(MCSymbol *Symbol, uint64_t Size, unsigned ByteAlignment) override;
  void EmitCVDefRangeDirective(ArrayRef<std::pair<const MCSymbol *, const MCSymbol *>> Ranges, StringRef FixedSizePortion) override;
  void EmitCVFileChecksumOffsetDirective(unsigned FileNo) override;
  void EmitCVFileChecksumsDirective() override;
  void EmitCVInlineLinetableDirective(unsigned PrimaryFunctionId, unsigned SourceFileId, unsigned SourceLineNum, const MCSymbol *FnStartSym, const MCSymbol *FnEndSym) override;
  void EmitCVLinetableDirective(unsigned FunctionId, const MCSymbol *Begin, const MCSymbol *End) override;
  void EmitCVLocDirective(unsigned FunctionId, unsigned FileNo, unsigned Line, unsigned Column, bool PrologueEnd, bool IsStmt, StringRef FileName, SMLoc Loc) override;
  void EmitCVStringTableDirective() override;
  void EmitDTPRel32Value(const MCExpr *Value) override;
  void EmitDTPRel64Value(const MCExpr *Value) override;
  void EmitDwarfLocDirective(unsigned FileNo, unsigned Line, unsigned Column, unsigned Flags, unsigned Isa, unsigned Discriminator, StringRef FileName) override;
  void EmitFileDirective(StringRef Filename) override;
  void emitFill(const MCExpr &NumBytes, uint64_t FillValue, SMLoc Loc = SMLoc()) override;
  void emitFill(const MCExpr &NumValues, int64_t Size, int64_t Expr, SMLoc Loc = SMLoc()) override;
  void EmitGPRel32Value(const MCExpr *Value) override;
  void EmitGPRel64Value(const MCExpr *Value) override;
  void EmitInstruction(const MCInst &Inst, const MCSubtargetInfo &STI, bool = false) override;
  void EmitLabel(MCSymbol *Symbol, SMLoc Loc = SMLoc()) override;
  void EmitSLEB128Value(const MCExpr *Value) override;
  void EmitTPRel32Value(const MCExpr *Value) override;
  void EmitTPRel64Value(const MCExpr *Value) override;
  void EmitULEB128Value(const MCExpr *Value) override;
  void EmitValueImpl(const MCExpr *Value, unsigned Size, SMLoc Loc = SMLoc()) override;
  void EmitValueToAlignment(unsigned ByteAlignment, int64_t Value = 0, unsigned ValueSize = 1, unsigned MaxBytesToEmit = 0) override;
  void emitValueToOffset(const MCExpr *Offset, unsigned char Value, SMLoc Loc) override;
  void EmitWeakReference(MCSymbol *Alias, const MCSymbol *Symbol) override;
  void EmitZerofill(MCSection *Section, MCSymbol *Symbol = nullptr, uint64_t Size = 0, unsigned ByteAlignment = 0, SMLoc Loc = SMLoc()) override;
  void FinishImpl() override;
  void reset() override;
  void visitUsedSymbol(const MCSymbol &Sym) override;

private:
  void EmitInstToFragment(const MCInst &Inst, const MCSubtargetInfo &) override;
  void EmitInstToData(const MCInst &Inst, const MCSubtargetInfo &) override;
};

} // end namespace llvm

#endif
