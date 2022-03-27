//=--- LLIRMCExpr.h - LLIR specific MC expression classes ---------*- C++ -*-=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes LLIR-specific MCExprs, i.e, registers used for
// extended variable assignments.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_MCTARGETDESC_LLIRMCEXPR_H
#define LLVM_LIB_TARGET_LLIR_MCTARGETDESC_LLIRMCEXPR_H

#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {

class LLIRMCExpr : public MCTargetExpr {
  std::string Annot;

private:

  explicit LLIRMCExpr(llvm::StringRef Str) : Annot(Str.str()) {}

public:
  static const LLIRMCExpr *create(llvm::StringRef Str, MCContext &Ctx) {
    return new (Ctx) LLIRMCExpr(Str);
  }

  StringRef getAnnot() const { return Annot; }

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override {
    OS << Annot;
  }

  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override {
    return false;
  }

  bool inlineAssignedExpr() const override { return true; }
  void visitUsedExpr(MCStreamer &Streamer) const override {};
  MCFragment *findAssociatedFragment() const override { return nullptr; }

  // There are no TLS LLIRMCExprs at the moment.
  void fixELFSymbolsInTLSFixups(MCAssembler &Asm) const override {}

  bool isEqualTo(const MCExpr *X) const override {
    if (auto *E = dyn_cast<LLIRMCExpr>(X))
      return getAnnot() == E->getAnnot();
    return false;
  }

  static bool classof(const MCExpr *E) {
    return E->getKind() == MCExpr::Target;
  }
};

} // end namespace llvm

#endif
