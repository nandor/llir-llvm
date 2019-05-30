//===- MCSymbolGenM.h -  ----------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_MC_MCSYMBOLGENM_H
#define LLVM_MC_MCSYMBOLGENM_H

#include "llvm/BinaryFormat/GenM.h"
#include "llvm/MC/MCSymbol.h"

namespace llvm {

class MCSymbolGenM : public MCSymbol {
public:
  // Use a module name of "env" for now, for compatibility with existing tools.
  // This is temporary, and may change, as the ABI is not yet stable.
  MCSymbolGenM(const StringMapEntry<bool> *Name, bool isTemporary)
      : MCSymbol(SymbolKindGenM, Name, isTemporary) {}
  static bool classof(const MCSymbol *S) { return S->isGenM(); }
};

} // end namespace llvm

#endif // LLVM_MC_MCSYMBOLGENM_H
