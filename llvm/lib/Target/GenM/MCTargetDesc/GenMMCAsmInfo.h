//===- GenMMCAsmInfo.h - GenM asm properties -----------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the GenMMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_MCTARGETDESC_GENMMCASMINFO_H
#define LLVM_LIB_TARGET_GENM_MCTARGETDESC_GENMMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class GenMELFMCAsmInfo : public MCAsmInfoELF {
public:
  explicit GenMELFMCAsmInfo(const Triple &TheTriple);

  const MCExpr* getExprForPersonalitySymbol(
      const MCSymbol *Sym,
      unsigned Encoding,
      MCStreamer &Streamer
  ) const override;

  const MCExpr* getExprForFDESymbol(
      const MCSymbol *Sym,
      unsigned Encoding,
      MCStreamer &Streamer
  ) const override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_GENM_MCTARGETDESC_GENMMCASMINFO_H
