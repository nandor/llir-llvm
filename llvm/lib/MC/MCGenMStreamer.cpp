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

void MCGenMStreamer::reset()
{
  llvm::errs() << "MCGenMStreamer::reset\n";
}

void MCGenMStreamer::EmitLabel(MCSymbol *Symbol, SMLoc Loc)
{
  llvm::errs() << "MCGenMStreamer::EmitLabel\n";
}

void MCGenMStreamer::ChangeSection(MCSection *Section, const MCExpr *Subsection)
{
  llvm::errs() << "MCGenMStreamer::ChangeSection\n";
}

void MCGenMStreamer::EmitInstruction(const MCInst &Inst, const MCSubtargetInfo &STI, bool)
{
  llvm::errs() << "MCGenMStreamer::EmitInstruction\n";
}

void MCGenMStreamer::EmitCodePaddingBasicBlockStart(const MCCodePaddingContext &Context)
{
  llvm::errs() << "MCGenMStreamer::EmitCodePaddingBasicBlockStart\n";
}

void MCGenMStreamer::EmitCodePaddingBasicBlockEnd(const MCCodePaddingContext &Context)
{
  llvm::errs() << "MCGenMStreamer::EmitCodePaddingBasicBlockEnd\n";
}

void MCGenMStreamer::FinishImpl()
{
  llvm::errs() << "MCGenMStreamer::FinishImpl\n";
}



bool MCGenMStreamer::EmitSymbolAttribute(MCSymbol *Symbol, MCSymbolAttr Attribute)
{
  llvm::errs() << "MCGenMStreamer::EmitSymbolAttribute\n";
  return false;
}

void MCGenMStreamer::EmitCFISections(bool EH, bool Debug)
{
  llvm::errs() << "MCGenMStreamer::EmitCFISections\n";
  abort();
}

void MCGenMStreamer::visitUsedSymbol(const MCSymbol &Sym)
{
  llvm::errs() << "MCGenMStreamer::visitUsedSymbol\n";
  abort();
}

void MCGenMStreamer::EmitAssignment(MCSymbol *Symbol, const MCExpr *Value)
{
  llvm::errs() << "MCGenMStreamer::EmitAssignment\n";
  abort();
}

void MCGenMStreamer::EmitValueImpl(const MCExpr *Value, unsigned Size, SMLoc Loc)
{
  llvm::errs() << "MCGenMStreamer::EmitValueImpl\n";
  abort();
}

void MCGenMStreamer::EmitULEB128Value(const MCExpr *Value)
{
  llvm::errs() << "MCGenMStreamer::EmitULEB128Value\n";
  abort();
}

void MCGenMStreamer::EmitSLEB128Value(const MCExpr *Value)
{
  llvm::errs() << "MCGenMStreamer::EmitSLEB128Value\n";
  abort();
}

void MCGenMStreamer::EmitWeakReference(MCSymbol *Alias, const MCSymbol *Symbol)
{
  llvm::errs() << "MCGenMStreamer::EmitWeakReference\n";
  abort();
}

void MCGenMStreamer::EmitBundleAlignMode(unsigned AlignPow2)
{
  llvm::errs() << "MCGenMStreamer::EmitBundleAlignMode\n";
  abort();
}

void MCGenMStreamer::EmitBundleLock(bool AlignToEnd)
{
  llvm::errs() << "MCGenMStreamer::EmitBundleLock\n";
  abort();
}

void MCGenMStreamer::EmitBundleUnlock()
{
  llvm::errs() << "MCGenMStreamer::EmitBundleUnlock\n";
  abort();
}

void MCGenMStreamer::EmitBytes(StringRef Data)
{
  llvm::errs() << "MCGenMStreamer::EmitBytes\n";
  abort();
}

void MCGenMStreamer::EmitValueToAlignment(unsigned ByteAlignment, int64_t Value, unsigned ValueSize, unsigned MaxBytesToEmit)
{
  llvm::errs() << "MCGenMStreamer::EmitValueToAlignment\n";
  abort();
}

void MCGenMStreamer::EmitCodeAlignment(unsigned ByteAlignment, unsigned MaxBytesToEmit)
{
  llvm::errs() << "MCGenMStreamer::EmitCodeAlignment\n";
  abort();
}

void MCGenMStreamer::emitValueToOffset(const MCExpr *Offset, unsigned char Value, SMLoc Loc)
{
  llvm::errs() << "MCGenMStreamer::emitValueToOffset\n";
  abort();
}

void MCGenMStreamer::EmitDwarfLocDirective(unsigned FileNo, unsigned Line, unsigned Column, unsigned Flags, unsigned Isa, unsigned Discriminator, StringRef FileName)
{
  llvm::errs() << "MCGenMStreamer::EmitDwarfLocDirective\n";
  abort();
}

void MCGenMStreamer::EmitCVLocDirective(unsigned FunctionId, unsigned FileNo, unsigned Line, unsigned Column, bool PrologueEnd, bool IsStmt, StringRef FileName, SMLoc Loc)
{
  llvm::errs() << "MCGenMStreamer::EmitCVLocDirective\n";
  abort();
}

void MCGenMStreamer::EmitCVLinetableDirective(unsigned FunctionId, const MCSymbol *Begin, const MCSymbol *End)
{
  llvm::errs() << "MCGenMStreamer::EmitCVLinetableDirective\n";
  abort();
}

void MCGenMStreamer::EmitCVInlineLinetableDirective(unsigned PrimaryFunctionId, unsigned SourceFileId, unsigned SourceLineNum, const MCSymbol *FnStartSym, const MCSymbol *FnEndSym)
{
  llvm::errs() << "MCGenMStreamer::EmitCVInlineLinetableDirective\n";
  abort();
}

void MCGenMStreamer::EmitCVDefRangeDirective(ArrayRef<std::pair<const MCSymbol *, const MCSymbol *>> Ranges, StringRef FixedSizePortion)
{
  llvm::errs() << "MCGenMStreamer::EmitCVDefRangeDirective\n";
  abort();
}

void MCGenMStreamer::EmitCVStringTableDirective()
{
  llvm::errs() << "MCGenMStreamer::EmitCVStringTableDirective\n";
  abort();
}

void MCGenMStreamer::EmitCVFileChecksumsDirective()
{
  llvm::errs() << "MCGenMStreamer::EmitCVFileChecksumsDirective\n";
  abort();
}

void MCGenMStreamer::EmitCVFileChecksumOffsetDirective(unsigned FileNo)
{
  llvm::errs() << "MCGenMStreamer::EmitCVFileChecksumOffsetDirective\n";
  abort();
}

void MCGenMStreamer::EmitDTPRel32Value(const MCExpr *Value)
{
  llvm::errs() << "MCGenMStreamer::EmitDTPRel32Value\n";
  abort();
}

void MCGenMStreamer::EmitDTPRel64Value(const MCExpr *Value)
{
  llvm::errs() << "MCGenMStreamer::EmitDTPRel64Value\n";
  abort();
}

void MCGenMStreamer::EmitTPRel32Value(const MCExpr *Value)
{
  llvm::errs() << "MCGenMStreamer::EmitTPRel32Value\n";
  abort();
}

void MCGenMStreamer::EmitTPRel64Value(const MCExpr *Value)
{
  llvm::errs() << "MCGenMStreamer::EmitTPRel64Value\n";
  abort();
}

void MCGenMStreamer::EmitGPRel32Value(const MCExpr *Value)
{
  llvm::errs() << "MCGenMStreamer::EmitGPRel32Value\n";
  abort();
}

void MCGenMStreamer::EmitGPRel64Value(const MCExpr *Value)
{
  llvm::errs() << "MCGenMStreamer::EmitGPRel64Value\n";
  abort();
}

bool MCGenMStreamer::EmitRelocDirective(const MCExpr &Offset, StringRef Name, const MCExpr *Expr, SMLoc Loc, const MCSubtargetInfo &STI)
{
  llvm::errs() << "MCGenMStreamer::EmitRelocDirective\n";
  abort();
  return true;
}

void MCGenMStreamer::emitFill(const MCExpr &NumBytes, uint64_t FillValue, SMLoc Loc)
{
  llvm::errs() << "MCGenMStreamer::emitFill\n";
  abort();
}

void MCGenMStreamer::emitFill(const MCExpr &NumValues, int64_t Size, int64_t Expr, SMLoc Loc)
{
  llvm::errs() << "MCGenMStreamer::emitFill\n";
  abort();
}

void MCGenMStreamer::EmitFileDirective(StringRef Filename)
{
  llvm::errs() << "MCGenMStreamer::EmitFileDirective\n";
}

void MCGenMStreamer::EmitAddrsig()
{
  llvm::errs() << "MCGenMStreamer::EmitAddrsig\n";
  abort();
}

void MCGenMStreamer::EmitAddrsigSym(const MCSymbol *Sym)
{
  llvm::errs() << "MCGenMStreamer::EmitAddrsigSym\n";
  abort();
}

void MCGenMStreamer::emitAbsoluteSymbolDiff(const MCSymbol *Hi, const MCSymbol *Lo, unsigned Size)
{
  llvm::errs() << "MCGenMStreamer::emitAbsoluteSymbolDiff\n";
  abort();
}

void MCGenMStreamer::emitAbsoluteSymbolDiffAsULEB128(const MCSymbol *Hi, const MCSymbol *Lo)
{
  llvm::errs() << "MCGenMStreamer::emitAbsoluteSymbolDiffAsULEB128\n";
  abort();
}

bool MCGenMStreamer::mayHaveInstructions(MCSection &Sec) const
{
  llvm::errs() << "MCGenMStreamer::mayHaveInstructions\n";
  abort();
}

void MCGenMStreamer::EmitCommonSymbol(MCSymbol *Symbol, uint64_t Size, unsigned ByteAlignment)
{
  llvm::errs() << "MCGenMStreamer::EmitCommonSymbol\n";
  abort();
}

void MCGenMStreamer::EmitZerofill(MCSection *Section, MCSymbol *Symbol, uint64_t Size, unsigned ByteAlignment, SMLoc Loc)
{
  llvm::errs() << "MCGenMStreamer::EmitZerofill\n";
  abort();
}

void MCGenMStreamer::EmitInstToFragment(const MCInst &Inst, const MCSubtargetInfo &)
{
  llvm::errs() << "MCGenMStreamer::EmitInstToFragment\n";
  abort();
}

void MCGenMStreamer::EmitInstToData(const MCInst &Inst, const MCSubtargetInfo &)
{
  llvm::errs() << "MCGenMStreamer::EmitInstToData\n";
  abort();
}


MCStreamer *llvm::createGenMStreamer(
    MCContext &Context,
    std::unique_ptr<MCAsmBackend> &&MAB,
    std::unique_ptr<MCObjectWriter> &&OW,
    std::unique_ptr<MCCodeEmitter> &&CE,
    bool RelaxAll)
{
  MCGenMStreamer *S = new MCGenMStreamer(
      Context,
      std::move(MAB),
      std::move(OW),
      std::move(CE)
  );
  if (RelaxAll) {
    S->getAssembler().setRelaxAll(true);
  }
  return S;
}
