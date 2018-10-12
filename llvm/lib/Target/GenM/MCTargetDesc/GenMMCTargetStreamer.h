//===-- GenMMCTargetStreamer.h - GenM Target Streamer ----------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_MCTARGETDESC_GENMMCTARGETSTREAMER_H
#define LLVM_LIB_TARGET_GENM_MCTARGETDESC_GENMMCTARGETSTREAMER_H

#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCStreamer.h"

namespace llvm {
class GenMMCTargetStreamer : public MCTargetStreamer {
public:
  GenMMCTargetStreamer(MCStreamer &S);

  /// .stack
  virtual void emitStackSize(int Size) = 0;
};

// This part is for ascii assembly output
class GenMMCTargetAsmStreamer : public GenMMCTargetStreamer {
public:
  GenMMCTargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);

  virtual void emitStackSize(int Size) override;

private:
  formatted_raw_ostream &OS;
};

// This part is for ELF object output
class GenMMCTargetELFStreamer : public GenMMCTargetStreamer {
public:
  GenMMCTargetELFStreamer(MCStreamer &S);

  virtual void emitStackSize(int Size) override;

  MCELFStreamer &getStreamer();
};
} // end namespace llvm

#endif
