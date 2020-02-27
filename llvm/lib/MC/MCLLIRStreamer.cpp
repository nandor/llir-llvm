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

void MCLLIRStreamer::reset()
{
  MCObjectStreamer::reset();
}

void MCLLIRStreamer::EmitFileDirective(StringRef Filename)
{
}

void MCLLIRStreamer::EmitLabel(MCSymbol *Symbol, SMLoc Loc)
{
  llvm::errs() << "MCLLIRStreamer::EmitLabel " << Symbol->getName() << "\n";
}

void MCLLIRStreamer::ChangeSection(MCSection *Section, const MCExpr *Subsection)
{
  MCAssembler &Asm = getAssembler();
  this->MCObjectStreamer::ChangeSection(Section, Subsection);
  if (auto *Sym = Section->getBeginSymbol()) {
    Asm.registerSymbol(*Sym);
  }
}

void MCLLIRStreamer::EmitInstruction(const MCInst &Inst, const MCSubtargetInfo &STI, bool)
{
  llvm::errs() << "MCLLIRStreamer::EmitInstruction\n";
}

void MCLLIRStreamer::EmitCodePaddingBasicBlockStart(const MCCodePaddingContext &Context)
{
  llvm::errs() << "MCLLIRStreamer::EmitCodePaddingBasicBlockStart\n";
}

void MCLLIRStreamer::EmitCodePaddingBasicBlockEnd(const MCCodePaddingContext &Context)
{
  llvm::errs() << "MCLLIRStreamer::EmitCodePaddingBasicBlockEnd\n";
}

void MCLLIRStreamer::FinishImpl()
{
  llvm::errs() << "MCLLIRStreamer::FinishImpl\n";
}



bool MCLLIRStreamer::EmitSymbolAttribute(MCSymbol *Symbol, MCSymbolAttr Attribute)
{
  llvm::errs() << "MCLLIRStreamer::EmitSymbolAttribute\n";
  return false;
}

void MCLLIRStreamer::EmitCFISections(bool EH, bool Debug)
{
  llvm::errs() << "MCLLIRStreamer::EmitCFISections\n";
  abort();
}

void MCLLIRStreamer::visitUsedSymbol(const MCSymbol &Sym)
{
  llvm::errs() << "MCLLIRStreamer::visitUsedSymbol\n";
  abort();
}

void MCLLIRStreamer::EmitAssignment(MCSymbol *Symbol, const MCExpr *Value)
{
  llvm::errs() << "MCLLIRStreamer::EmitAssignment\n";
  abort();
}

void MCLLIRStreamer::EmitValueImpl(const MCExpr *Value, unsigned Size, SMLoc Loc)
{
  llvm::errs() << "MCLLIRStreamer::EmitValueImpl\n";
  abort();
}

void MCLLIRStreamer::EmitULEB128Value(const MCExpr *Value)
{
  llvm::errs() << "MCLLIRStreamer::EmitULEB128Value\n";
  abort();
}

void MCLLIRStreamer::EmitSLEB128Value(const MCExpr *Value)
{
  llvm::errs() << "MCLLIRStreamer::EmitSLEB128Value\n";
  abort();
}

void MCLLIRStreamer::EmitWeakReference(MCSymbol *Alias, const MCSymbol *Symbol)
{
  llvm::errs() << "MCLLIRStreamer::EmitWeakReference\n";
  abort();
}

void MCLLIRStreamer::EmitBundleAlignMode(unsigned AlignPow2)
{
  llvm::errs() << "MCLLIRStreamer::EmitBundleAlignMode\n";
  abort();
}

void MCLLIRStreamer::EmitBundleLock(bool AlignToEnd)
{
  llvm::errs() << "MCLLIRStreamer::EmitBundleLock\n";
  abort();
}

void MCLLIRStreamer::EmitBundleUnlock()
{
  llvm::errs() << "MCLLIRStreamer::EmitBundleUnlock\n";
  abort();
}

void MCLLIRStreamer::EmitBytes(StringRef Data)
{
  llvm::errs() << "MCLLIRStreamer::EmitBytes\n";
  abort();
}

void MCLLIRStreamer::EmitValueToAlignment(unsigned ByteAlignment, int64_t Value, unsigned ValueSize, unsigned MaxBytesToEmit)
{
  llvm::errs() << "MCLLIRStreamer::EmitValueToAlignment\n";
  abort();
}

void MCLLIRStreamer::EmitCodeAlignment(unsigned ByteAlignment, unsigned MaxBytesToEmit)
{
  llvm::errs() << "MCLLIRStreamer::EmitCodeAlignment\n";
  abort();
}

void MCLLIRStreamer::emitValueToOffset(const MCExpr *Offset, unsigned char Value, SMLoc Loc)
{
  llvm::errs() << "MCLLIRStreamer::emitValueToOffset\n";
  abort();
}

void MCLLIRStreamer::EmitDwarfLocDirective(unsigned FileNo, unsigned Line, unsigned Column, unsigned Flags, unsigned Isa, unsigned Discriminator, StringRef FileName)
{
  llvm::errs() << "MCLLIRStreamer::EmitDwarfLocDirective\n";
  abort();
}

void MCLLIRStreamer::EmitCVLocDirective(unsigned FunctionId, unsigned FileNo, unsigned Line, unsigned Column, bool PrologueEnd, bool IsStmt, StringRef FileName, SMLoc Loc)
{
  llvm::errs() << "MCLLIRStreamer::EmitCVLocDirective\n";
  abort();
}

void MCLLIRStreamer::EmitCVLinetableDirective(unsigned FunctionId, const MCSymbol *Begin, const MCSymbol *End)
{
  llvm::errs() << "MCLLIRStreamer::EmitCVLinetableDirective\n";
  abort();
}

void MCLLIRStreamer::EmitCVInlineLinetableDirective(unsigned PrimaryFunctionId, unsigned SourceFileId, unsigned SourceLineNum, const MCSymbol *FnStartSym, const MCSymbol *FnEndSym)
{
  llvm::errs() << "MCLLIRStreamer::EmitCVInlineLinetableDirective\n";
  abort();
}

void MCLLIRStreamer::EmitCVDefRangeDirective(ArrayRef<std::pair<const MCSymbol *, const MCSymbol *>> Ranges, StringRef FixedSizePortion)
{
  llvm::errs() << "MCLLIRStreamer::EmitCVDefRangeDirective\n";
  abort();
}

void MCLLIRStreamer::EmitCVStringTableDirective()
{
  llvm::errs() << "MCLLIRStreamer::EmitCVStringTableDirective\n";
  abort();
}

void MCLLIRStreamer::EmitCVFileChecksumsDirective()
{
  llvm::errs() << "MCLLIRStreamer::EmitCVFileChecksumsDirective\n";
  abort();
}

void MCLLIRStreamer::EmitCVFileChecksumOffsetDirective(unsigned FileNo)
{
  llvm::errs() << "MCLLIRStreamer::EmitCVFileChecksumOffsetDirective\n";
  abort();
}

void MCLLIRStreamer::EmitDTPRel32Value(const MCExpr *Value)
{
  llvm::errs() << "MCLLIRStreamer::EmitDTPRel32Value\n";
  abort();
}

void MCLLIRStreamer::EmitDTPRel64Value(const MCExpr *Value)
{
  llvm::errs() << "MCLLIRStreamer::EmitDTPRel64Value\n";
  abort();
}

void MCLLIRStreamer::EmitTPRel32Value(const MCExpr *Value)
{
  llvm::errs() << "MCLLIRStreamer::EmitTPRel32Value\n";
  abort();
}

void MCLLIRStreamer::EmitTPRel64Value(const MCExpr *Value)
{
  llvm::errs() << "MCLLIRStreamer::EmitTPRel64Value\n";
  abort();
}

void MCLLIRStreamer::EmitGPRel32Value(const MCExpr *Value)
{
  llvm::errs() << "MCLLIRStreamer::EmitGPRel32Value\n";
  abort();
}

void MCLLIRStreamer::EmitGPRel64Value(const MCExpr *Value)
{
  llvm::errs() << "MCLLIRStreamer::EmitGPRel64Value\n";
  abort();
}

bool MCLLIRStreamer::EmitRelocDirective(const MCExpr &Offset, StringRef Name, const MCExpr *Expr, SMLoc Loc, const MCSubtargetInfo &STI)
{
  llvm::errs() << "MCLLIRStreamer::EmitRelocDirective\n";
  abort();
  return true;
}

void MCLLIRStreamer::emitFill(const MCExpr &NumBytes, uint64_t FillValue, SMLoc Loc)
{
  llvm::errs() << "MCLLIRStreamer::emitFill\n";
  abort();
}

void MCLLIRStreamer::emitFill(const MCExpr &NumValues, int64_t Size, int64_t Expr, SMLoc Loc)
{
  llvm::errs() << "MCLLIRStreamer::emitFill\n";
  abort();
}

void MCLLIRStreamer::EmitAddrsig()
{
  llvm::errs() << "MCLLIRStreamer::EmitAddrsig\n";
  abort();
}

void MCLLIRStreamer::EmitAddrsigSym(const MCSymbol *Sym)
{
  llvm::errs() << "MCLLIRStreamer::EmitAddrsigSym\n";
  abort();
}

void MCLLIRStreamer::emitAbsoluteSymbolDiff(const MCSymbol *Hi, const MCSymbol *Lo, unsigned Size)
{
  llvm::errs() << "MCLLIRStreamer::emitAbsoluteSymbolDiff\n";
  abort();
}

void MCLLIRStreamer::emitAbsoluteSymbolDiffAsULEB128(const MCSymbol *Hi, const MCSymbol *Lo)
{
  llvm::errs() << "MCLLIRStreamer::emitAbsoluteSymbolDiffAsULEB128\n";
  abort();
}

bool MCLLIRStreamer::mayHaveInstructions(MCSection &Sec) const
{
  llvm::errs() << "MCLLIRStreamer::mayHaveInstructions\n";
  abort();
}

void MCLLIRStreamer::EmitCommonSymbol(MCSymbol *Symbol, uint64_t Size, unsigned ByteAlignment)
{
  llvm::errs() << "MCLLIRStreamer::EmitCommonSymbol\n";
  abort();
}

void MCLLIRStreamer::EmitZerofill(MCSection *Section, MCSymbol *Symbol, uint64_t Size, unsigned ByteAlignment, SMLoc Loc)
{
  llvm::errs() << "MCLLIRStreamer::EmitZerofill\n";
  abort();
}

void MCLLIRStreamer::EmitInstToFragment(const MCInst &Inst, const MCSubtargetInfo &)
{
  llvm::errs() << "MCLLIRStreamer::EmitInstToFragment\n";
  abort();
}

void MCLLIRStreamer::EmitInstToData(const MCInst &Inst, const MCSubtargetInfo &)
{
  llvm::errs() << "MCLLIRStreamer::EmitInstToData\n";
  abort();
}


MCStreamer *llvm::createLLIRStreamer(
    MCContext &Context,
    std::unique_ptr<MCAsmBackend> &&MAB,
    std::unique_ptr<MCObjectWriter> &&OW,
    std::unique_ptr<MCCodeEmitter> &&CE,
    bool RelaxAll)
{
  MCLLIRStreamer *S = new MCLLIRStreamer(
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
