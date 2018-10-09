//===- GenMMCAsmInfo.cpp - GenM asm properties ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the GenMMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "GenMMCAsmInfo.h"
#include "llvm/ADT/Triple.h"
#include "llvm/BinaryFormat/Dwarf.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCTargetOptions.h"

using namespace llvm;

GenMELFMCAsmInfo::GenMELFMCAsmInfo(const Triple &TheTriple) {
}

const MCExpr* GenMELFMCAsmInfo::getExprForPersonalitySymbol(
    const MCSymbol *Sym,
    unsigned Encoding,
    MCStreamer &Streamer) const
{
  llvm_unreachable("getExprForPersonalitySymbol");
}

const MCExpr* GenMELFMCAsmInfo::getExprForFDESymbol(
    const MCSymbol *Sym,
    unsigned Encoding,
    MCStreamer &Streamer) const
{
  llvm_unreachable("getExprForFDESymbol");
}
