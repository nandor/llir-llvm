//===-- LLIRMCInstLower.h - Lower MachineInstr to MCInst -*- C++ --------*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file declares the class to lower LLIR MachineInstrs to
/// their corresponding MCInst records.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIRMCINSTLOWER_H
#define LLVM_LIB_TARGET_LLIR_LLIRMCINSTLOWER_H

#include "llvm/MC/MCInst.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class LLIRAsmPrinter;
class MCContext;
class MCSymbol;
class MachineInstr;
class MachineOperand;
class MachineFunction;

/// This class is used to lower an MachineInstr into an MCInst.
class LLVM_LIBRARY_VISIBILITY LLIRMCInstLower {
public:
  LLIRMCInstLower(
      MCContext &Ctx,
      const MachineFunction &MF,
      LLIRAsmPrinter &Printer)
    : Ctx(Ctx)
    , MF(MF)
    , Printer(Printer)
  {
  }

  void Lower(const MachineInstr *MI, MCInst &OutMI) const;

private:
  MCContext &Ctx;
  const MachineFunction &MF;
  LLIRAsmPrinter &Printer;

  MCSymbol *GetSymbolFromOperand(const MachineOperand &MO) const;

  MCOperand LowerSymbolOperand(
      MCSymbol *Sym,
      int64_t Offset,
      bool IsFunc,
      bool IsGlob
  ) const;
};
} // end namespace llvm

#endif
