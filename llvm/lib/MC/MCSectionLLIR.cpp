//===- lib/MC/MCSectionLLIR.cpp - LLIR Code Section Representation --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/MC/MCSectionLLIR.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/raw_ostream.h"


using namespace llvm;

MCSectionLLIR::~MCSectionLLIR() {} // anchor.

void MCSectionLLIR::PrintSwitchToSection(
    const MCAsmInfo &MAI,
    const Triple &T,
    raw_ostream &OS,
    const MCExpr *Subsection) const
{
  OS << ".section\t" << Name << "\n";
}

bool MCSectionLLIR::UseCodeAlign() const { return false; }

bool MCSectionLLIR::isVirtualSection() const { return false; }
