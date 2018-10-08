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
  llvm_unreachable("not implemented");
}

MCSymbol *
GenMMCInstLower::GetExternalSymbolSymbol(const MachineOperand &MO) const
{
  llvm_unreachable("not implemented");
}

MCOperand GenMMCInstLower::LowerSymbolOperand(
    MCSymbol *Sym,
    int64_t Offset,
    bool IsFunc,
    bool IsGlob) const
{
  llvm_unreachable("not implemented");
}


void GenMMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const
{
  OutMI.setOpcode(MI->getOpcode());
  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);

    MCOperand MCOp;
    switch (MO.getType()) {
      case MachineOperand::MO_MachineBasicBlock: {
        const MCSymbol *Sym = MO.getMBB()->getSymbol();
        MCOp = MCOperand::createExpr(MCSymbolRefExpr::create(Sym, Ctx));
        break;
      }
      case MachineOperand::MO_Register: {
        if (MO.isImplicit()) {
          continue;
        }

        if (GenMRegisterInfo::isVirtualRegister(MO.getReg())) {
          const auto &MF = *MI->getParent()->getParent();
          const auto &MFI = *MF.getInfo<GenMMachineFunctionInfo>();
          MCOp = MCOperand::createReg(MFI.getGMReg(MO.getReg()));
        } else {
          MCOp = MCOperand::createReg(MO.getReg());
        }

        break;
      }
      case MachineOperand::MO_Immediate: {
        MCOp = MCOperand::createImm(MO.getImm());
        break;
      }
      case MachineOperand::MO_FPImmediate: {
        llvm_unreachable("not implemented");
      }
      case MachineOperand::MO_GlobalAddress: {
        MCOp = LowerSymbolOperand(
            GetGlobalAddressSymbol(MO),
            MO.getOffset(),
            MO.getGlobal()->getValueType()->isFunctionTy(),
            false
        );
        break;
      }
      case MachineOperand::MO_ExternalSymbol: {
        llvm_unreachable("not implemented");
      }
      default: {
        MI->print(errs());
        llvm_unreachable("unknown operand type");
      }
    }

    OutMI.addOperand(MCOp);
  }
}
