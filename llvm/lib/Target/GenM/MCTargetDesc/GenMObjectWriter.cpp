//===-- GenMObjectWriter.cpp - GenM ELF Writer ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/GenMMCTargetDesc.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
  class GenMObjectWriter : public MCELFObjectTargetWriter {
  public:
    GenMObjectWriter()
      : MCELFObjectTargetWriter(true, ELF::ELFOSABI_NONE, ELF::EM_NONE, false)
    {
    }

    ~GenMObjectWriter() override {}

  protected:
    unsigned getRelocType(
        MCContext &Ctx,
        const MCValue &Target,
        const MCFixup &Fixup,
        bool IsPCRel) const override
    {
      llvm_unreachable("getRelocType() not implemented");
    }

    bool needsRelocateWithSymbol(
        const MCSymbol &Sym,
        unsigned Type) const override
    {
      llvm_unreachable("needsRelocateWithSymbol() not implemented");
    }

  };
}

std::unique_ptr<MCObjectTargetWriter> llvm::createGenMObjectWriter()
{
  return llvm::make_unique<GenMObjectWriter>();
}
