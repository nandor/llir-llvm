//===- LLIRMCAsmInfo.cpp - LLIR asm properties ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the LLIRMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "LLIRMCAsmInfo.h"
#include "llvm/ADT/Triple.h"
#include "llvm/BinaryFormat/Dwarf.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCTargetOptions.h"

using namespace llvm;

LLIRMCAsmInfo::LLIRMCAsmInfo(const Triple &TheTriple) {
  CommentString = "#";
  ZeroDirective = "\t.space\t";
  Data64bitsDirective = "\t.quad\t";
  WeakRefDirective = "\t.weak\t";
  HasFunctionAlignment = true;
  HasDotTypeDotSizeDirective = false;
  HasNoDeadStrip = true;
  SupportsDebugInformation = false;
  HasIdentDirective = true;
  ExceptionsType = ExceptionHandling::None;
  PrivateGlobalPrefix = ".L";
  PrivateLabelPrefix = ".L";
}

const MCExpr* LLIRMCAsmInfo::getExprForPersonalitySymbol(
    const MCSymbol *Sym,
    unsigned Encoding,
    MCStreamer &Streamer) const
{
  llvm_unreachable("getExprForPersonalitySymbol");
}

const MCExpr* LLIRMCAsmInfo::getExprForFDESymbol(
    const MCSymbol *Sym,
    unsigned Encoding,
    MCStreamer &Streamer) const
{
  llvm_unreachable("getExprForFDESymbol");
}


bool LLIRMCAsmInfo::isValidUnquotedName(StringRef Name) const
{
  if (Name.empty()) {
    return false;
  }
  if (Name[0] == '\1' && MCAsmInfo::isValidUnquotedName(Name.substr(1))) {
    return true;
  }
  return MCAsmInfo::isValidUnquotedName(Name);
}
