//===- MCSectionGenM.h - GenM Machine Code Sections -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the MCSectionGenM class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_MC_MCSECTIONGENM_H
#define LLVM_MC_MCSECTIONGENM_H

#include "llvm/ADT/Twine.h"
#include "llvm/MC/MCSection.h"
#include "llvm/MC/MCSymbolGenM.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {

class MCSymbol;

/// This represents a section on genm.
class MCSectionGenM final : public MCSection {
  friend class MCContext;
  MCSectionGenM(
      StringRef Section,
      SectionKind K,
      MCSymbol *Begin)
    : MCSection(SV_GenM, K, Begin)
  {
  }

public:
  ~MCSectionGenM();

  void PrintSwitchToSection(
      const MCAsmInfo &MAI,
      const Triple &T,
      raw_ostream &OS,
      const MCExpr *Subsection
  ) const override;

  bool UseCodeAlign() const override;
  bool isVirtualSection() const override;

  static bool classof(const MCSection *S) { return S->getVariant() == SV_GenM; }
};

} // end namespace llvm

#endif
