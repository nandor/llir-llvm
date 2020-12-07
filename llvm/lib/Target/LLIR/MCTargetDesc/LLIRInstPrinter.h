//===-- LLIRInstPrinter.h - Convert LLIR MCInst to assembly syntax ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an LLIR MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_INSTPRINTER_LLIRINSTPRINTER_H
#define LLVM_LIB_TARGET_LLIR_INSTPRINTER_LLIRINSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"

namespace llvm {

class LLIRInstPrinter : public MCInstPrinter {
 public:
  LLIRInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                  const MCRegisterInfo &MRI);

  void printRegName(raw_ostream &OS, unsigned RegNo) const override;

  void printInst(const MCInst *MI, uint64_t Address, StringRef Annot, const MCSubtargetInfo &STI,  raw_ostream &O) override;

  void printInstruction(const MCInst *MI, uint64_t Address,
                        const MCSubtargetInfo &STI, raw_ostream &O);

  bool printAliasInstr(const MCInst *MI, uint64_t Address,
                       const MCSubtargetInfo &STI, raw_ostream &O);

  void printOperand(const MCInst *MI, int opNum, const MCSubtargetInfo &STI,
                    raw_ostream &OS);

  static const char *getRegisterName(unsigned RegNo);

 private:
   void printReturn(llvm::raw_ostream &OS, const MCInst *MI,
                    const MCSubtargetInfo &STI);

   void printCall(const char *Op, llvm::raw_ostream &OS, const MCInst *MI,
                  const MCSubtargetInfo &STI, bool isVoid, bool isVA,
                  bool isTail);
};
}  // end namespace llvm

#endif
