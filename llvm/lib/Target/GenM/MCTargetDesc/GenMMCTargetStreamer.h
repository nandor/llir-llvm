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

#include "llvm/IR/CallingConv.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/MachineValueType.h"

namespace llvm {
class GenMMCTargetStreamer : public MCTargetStreamer {
public:
  GenMMCTargetStreamer(MCStreamer &S);

  /// .stack_obj
  virtual void emitStackObject(int Offset, int Size, int Align) = 0;
  /// .args
  virtual void emitParams(ArrayRef<MVT> params, bool IsVA) = 0;
  /// .fast, .c, etc.
  virtual void emitCallingConv(CallingConv::ID CallConv) = 0;
  /// .end
  virtual void emitEnd() = 0;
  /// .noinline
  virtual void emitNoInline() = 0;
};

// This part is for ascii assembly output
class GenMMCTargetAsmStreamer : public GenMMCTargetStreamer {
public:
  GenMMCTargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);

  virtual void emitStackObject(int Offset, int Size, int Align) override;
  virtual void emitParams(ArrayRef<MVT> params, bool IsVA) override;
  virtual void emitCallingConv(CallingConv::ID CallConv) override;
  virtual void emitEnd() override;
  virtual void emitNoInline() override;

private:
  formatted_raw_ostream &OS;
};

// This part is for ELF object output
class GenMMCTargetGenMStreamer : public GenMMCTargetStreamer {
public:
  GenMMCTargetGenMStreamer(MCStreamer &S);

  virtual void emitStackObject(int Offset, int Size, int Align) override;
  virtual void emitParams(ArrayRef<MVT> params, bool IsVA) override;
  virtual void emitCallingConv(CallingConv::ID CallConv) override;
  virtual void emitEnd() override;
  virtual void emitNoInline() override;
};
} // end namespace llvm

#endif
