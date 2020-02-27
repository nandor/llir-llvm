//===- MCSymbolLLIR.h -  ----------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_MC_MCSYMBOLLLIR_H
#define LLVM_MC_MCSYMBOLLLIR_H

#include "llvm/BinaryFormat/LLIR.h"
#include "llvm/MC/MCSymbol.h"

namespace llvm {

class MCSymbolLLIR : public MCSymbol {
public:
  // Use a module name of "env" for now, for compatibility with existing tools.
  // This is temporary, and may change, as the ABI is not yet stable.
  MCSymbolLLIR(const StringMapEntry<bool> *Name, bool isTemporary)
      : MCSymbol(SymbolKindLLIR, Name, isTemporary) {}
  static bool classof(const MCSymbol *S) { return S->isLLIR(); }
};

} // end namespace llvm

#endif // LLVM_MC_MCSYMBOLLLIR_H
