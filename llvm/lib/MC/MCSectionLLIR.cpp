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

static void printName(raw_ostream &OS, StringRef Name) {
  OS << '"';
  for (const char *B = Name.begin(), *E = Name.end(); B < E; ++B) {
    if (*B == '"') // Unquoted "
      OS << "\\\"";
    else if (*B != '\\') // Neither " or backslash
      OS << *B;
    else if (B + 1 == E) // Trailing backslash
      OS << "\\\\";
    else {
      OS << B[0] << B[1]; // Quoted character
      ++B;
    }
  }
  OS << '"';
}

void MCSectionLLIR::PrintSwitchToSection(
    const MCAsmInfo &MAI,
    const Triple &T,
    raw_ostream &OS,
    const MCExpr *Subsection) const
{
  OS << ".section\t"; printName(OS, Name); OS << "\n";
}

bool MCSectionLLIR::UseCodeAlign() const { return false; }

bool MCSectionLLIR::isVirtualSection() const { return false; }
