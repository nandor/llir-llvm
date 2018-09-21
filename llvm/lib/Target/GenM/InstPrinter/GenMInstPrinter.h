//===-- GenMInstPrinter.h - Convert GenM MCInst to assembly syntax ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an GenM MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_INSTPRINTER_GENMINSTPRINTER_H
#define LLVM_LIB_TARGET_GENM_INSTPRINTER_GENMINSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"

namespace llvm {

class GenMInstPrinter : public MCInstPrinter {
public:
  GenMInstPrinter(
      const MCAsmInfo &MAI,
      const MCInstrInfo &MII,
      const MCRegisterInfo &MRI)
    : MCInstPrinter(MAI, MII, MRI)
  {
  }

  void printRegName(raw_ostream &OS, unsigned RegNo) const override;

  void printInst(
      const MCInst *MI,
      raw_ostream &O,
      StringRef Annot,
      const MCSubtargetInfo &STI
  ) override;

  void printInstruction(
      const MCInst *MI,
      const MCSubtargetInfo &STI,
      raw_ostream &O
  );

  bool printAliasInstr(
      const MCInst *MI,
      const MCSubtargetInfo &STI,
      raw_ostream &O
  );

  void printOperand(
      const MCInst *MI,
      int opNum,
      const MCSubtargetInfo &STI,
      raw_ostream &OS
  );

  static const char *getRegisterName(unsigned RegNo);
};
} // end namespace llvm

#endif
