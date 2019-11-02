//===- lib/MC/MCSectionGenM.cpp - GenM Code Section Representation --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/MC/MCSectionGenM.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/raw_ostream.h"


using namespace llvm;

MCSectionGenM::~MCSectionGenM() {} // anchor.

void MCSectionGenM::PrintSwitchToSection(
    const MCAsmInfo &MAI,
    const Triple &T,
    raw_ostream &OS,
    const MCExpr *Subsection) const
{
}

bool MCSectionGenM::UseCodeAlign() const { return false; }

bool MCSectionGenM::isVirtualSection() const { return false; }
