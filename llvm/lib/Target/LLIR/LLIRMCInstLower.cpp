// LLIRMCInstLower.cpp - Convert LLIR MachineInstr to an MCInst -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains code to lower LLIR MachineInstrs to their
/// corresponding MCInst records.
///
//===----------------------------------------------------------------------===//

#include "LLIRMCInstLower.h"

#include "LLIRAsmPrinter.h"
#include "LLIRMachineFunctionInfo.h"
#include "MCTargetDesc/LLIRMCTargetDesc.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbolWasm.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

MCSymbol *LLIRMCInstLower::GetSymbolFromOperand(
    const MachineOperand &MO) const {
  const DataLayout &DL = MF.getDataLayout();
  MCSymbol *Sym = nullptr;
  SmallString<128> Name;

  if (MO.isGlobal()) {
    const GlobalValue *GV = MO.getGlobal();
    if (GV->hasName() && GV->getName()[0] == '\1') {
      Sym = Ctx.getOrCreateSymbol(GV->getName());
    } else {
      Printer.getNameWithPrefix(Name, GV);
      Sym = Ctx.getOrCreateSymbol(Name);
    }
  } else if (MO.isSymbol()) {
    auto name = MO.getSymbolName();
    if (name[0] == '\1') {
      Sym = Ctx.getOrCreateSymbol(name);
    } else {
      Mangler::getNameWithPrefix(Name, name, DL);
      Sym = Ctx.getOrCreateSymbol(Name);
    }
  } else if (MO.isMBB()) {
    Sym = MO.getMBB()->getSymbol();
  }

  return Sym;
}

MCOperand LLIRMCInstLower::LowerSymbolOperand(MCSymbol *Sym, int64_t Offset,
                                              bool IsFunc, bool IsGlob) const {
  const MCExpr *Expr =
      MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, Ctx);
  if (Offset != 0) {
    Expr =
        MCBinaryExpr::createAdd(Expr, MCConstantExpr::create(Offset, Ctx), Ctx);
  }
  return MCOperand::createExpr(Expr);
}

void LLIRMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
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

        if (Register::isVirtualRegister(MO.getReg())) {
          const auto &MF = *MI->getParent()->getParent();
          const auto &MFI = *MF.getInfo<LLIRMachineFunctionInfo>();
          MCOp = MCOperand::createReg(LLIR::NUM_TARGET_REGS +
                                      MFI.getGMReg(MO.getReg()));
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
        // TODO(nand): proper 80-bit float support
        APFloat Val = MO.getFPImm()->getValueAPF();
        bool ignored;
        Val.convert(APFloat::IEEEdouble(), APFloat::rmTowardZero, &ignored);
        MCOp = MCOperand::createFPImm(Val.convertToDouble());
        break;
      }
      case MachineOperand::MO_GlobalAddress: {
        MCOp = LowerSymbolOperand(
            GetSymbolFromOperand(MO), MO.getOffset(),
            MO.getGlobal()->getValueType()->isFunctionTy(), false);
        break;
      }
      case MachineOperand::MO_ExternalSymbol: {
        // TODO(nand): remove hardcoded symbols.
        MCOp = LowerSymbolOperand(GetSymbolFromOperand(MO), 0, true, true);
        break;
      }
      case MachineOperand::MO_ConstantPoolIndex: {
        // TODO(nand): remove hardcoded symbols.
        MCOp = LowerSymbolOperand(Printer.GetCPISymbol(MO.getIndex()), 0, true, true);
        break;
      }
      default: {
        MI->print(errs());
        llvm_unreachable("unknown operand type");
      }
    }

    OutMI.addOperand(MCOp);
  }
}
