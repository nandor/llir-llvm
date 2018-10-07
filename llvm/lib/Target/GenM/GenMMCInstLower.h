//===-- GenMMCInstLower.h - Lower MachineInstr to MCInst -*- C++ --------*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file declares the class to lower GenM MachineInstrs to
/// their corresponding MCInst records.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_GENMMCINSTLOWER_H
#define LLVM_LIB_TARGET_GENM_GENMMCINSTLOWER_H

#include "llvm/MC/MCInst.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class GenMAsmPrinter;
class MCContext;
class MCSymbol;
class MachineInstr;
class MachineOperand;

/// This class is used to lower an MachineInstr into an MCInst.
class LLVM_LIBRARY_VISIBILITY GenMMCInstLower {
public:
  GenMMCInstLower(MCContext &ctx, GenMAsmPrinter &printer)
      : Ctx(ctx)
  {
  }

  void Lower(const MachineInstr *MI, MCInst &OutMI) const;

private:
  MCContext &Ctx;

  MCSymbol *GetGlobalAddressSymbol(const MachineOperand &MO) const;

  MCSymbol *GetExternalSymbolSymbol(const MachineOperand &MO) const;

  MCOperand LowerSymbolOperand(
      MCSymbol *Sym,
      int64_t Offset,
      bool IsFunc,
      bool IsGlob
  ) const;
};
} // end namespace llvm

#endif
