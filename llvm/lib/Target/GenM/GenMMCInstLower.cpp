// GenMMCInstLower.cpp - Convert GenM MachineInstr to an MCInst -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains code to lower GenM MachineInstrs to their
/// corresponding MCInst records.
///
//===----------------------------------------------------------------------===//

#include "GenMMCInstLower.h"
#include "GenMAsmPrinter.h"
#include "GenMMachineFunctionInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/IR/Constants.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbolWasm.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

MCSymbol *
GenMMCInstLower::GetGlobalAddressSymbol(const MachineOperand &MO) const
{
  assert(!"not implemented");
}

MCSymbol *
GenMMCInstLower::GetExternalSymbolSymbol(const MachineOperand &MO) const
{
  assert(!"not implemented");
}

MCOperand GenMMCInstLower::LowerSymbolOperand(
    MCSymbol *Sym,
    int64_t Offset,
    bool IsFunc,
    bool IsGlob) const
{
  assert(!"not implemented");
}


void GenMMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const
{
  OutMI.setOpcode(MI->getOpcode());

  const MCInstrDesc &Desc = MI->getDesc();
  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);

    MCOperand MCOp;
    switch (MO.getType()) {
      case MachineOperand::MO_MachineBasicBlock: {
        assert(!"not implemented");
      }
      case MachineOperand::MO_Register: {
        assert(!"not implemented");
      }
      case MachineOperand::MO_Immediate: {
        assert(!"not implemented");
      }
      case MachineOperand::MO_FPImmediate: {
        assert(!"not implemented");
      }
      case MachineOperand::MO_GlobalAddress: {
        assert(!"not implemented");
      }
      case MachineOperand::MO_ExternalSymbol: {
        assert(!"not implemented");
      }
      default: {
        MI->print(errs());
        llvm_unreachable("unknown operand type");
      }
    }

    OutMI.addOperand(MCOp);
  }
}
