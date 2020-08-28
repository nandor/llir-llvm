//===- LLIRMCAsmInfo.h - LLIR asm properties -----------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the LLIRMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_MCTARGETDESC_LLIRMCASMINFO_H
#define LLVM_LIB_TARGET_LLIR_MCTARGETDESC_LLIRMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class LLIRMCAsmInfo : public MCAsmInfo {
 public:
  explicit LLIRMCAsmInfo(const Triple &TheTriple);

  const MCExpr *getExprForPersonalitySymbol(
      const MCSymbol *Sym, unsigned Encoding,
      MCStreamer &Streamer) const override;

  const MCExpr *getExprForFDESymbol(const MCSymbol *Sym, unsigned Encoding,
                                    MCStreamer &Streamer) const override;

  bool isValidUnquotedName(StringRef Name) const override;
};

}  // end namespace llvm

#endif
