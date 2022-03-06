//===-- LLIRMCTargetStreamer.h - LLIR Target Streamer ----------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_MCTARGETDESC_LLIRMCTARGETSTREAMER_H
#define LLVM_LIB_TARGET_LLIR_MCTARGETDESC_LLIRMCTARGETSTREAMER_H

#include "llvm/IR/CallingConv.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/MachineValueType.h"
#include "LLIRMachineFunctionInfo.h"

namespace llvm {
class LLIRMCTargetStreamer : public MCTargetStreamer {
public:
  LLIRMCTargetStreamer(MCStreamer &S);

  /// .features
  virtual void emitFeatures(StringRef cpu, StringRef tuneCPU, StringRef fs) = 0;
  /// .stack_obj
  virtual void emitStackObject(int Offset, int Size, int Align) = 0;
  /// .vararg
  virtual void emitVarArg() = 0;
  /// .args
  virtual void emitParams(ArrayRef<LLIRMachineFunctionInfo::Parameter> params) = 0;
  /// .fast, .c, etc.
  virtual void emitCallingConv(CallingConv::ID CallConv) = 0;
  /// .ctor
  virtual void emitCtor(int Priority, const MCSymbol *Sym) = 0;
  /// .dtor
  virtual void emitDtor(int Priority, const MCSymbol *Sym) = 0;
  /// .thread_local
  virtual void emitThreadLocal() = 0;
  /// .end
  virtual void emitEnd() = 0;
  /// .noinline
  virtual void emitNoInline() = 0;
  /// .cfi_personality
  virtual void emitCFIPersonality(const MCSymbol *Sym) = 0;
};

// This part is for ascii assembly output
class LLIRMCTargetAsmStreamer : public LLIRMCTargetStreamer {
public:
  LLIRMCTargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);

  virtual void emitStackObject(int Offset, int Size, int Align) override;
  virtual void emitVarArg() override;
  virtual void emitFeatures(StringRef cpu, StringRef tuneCPU, StringRef fs) override;
  virtual void emitParams(ArrayRef<LLIRMachineFunctionInfo::Parameter> params) override;
  virtual void emitCallingConv(CallingConv::ID CallConv) override;
  virtual void emitCtor(int Priority, const MCSymbol *Sym) override;
  virtual void emitDtor(int Priority, const MCSymbol *Sym) override;
  virtual void emitThreadLocal() override;
  virtual void emitEnd() override;
  virtual void emitNoInline() override;
  virtual void emitCFIPersonality(const MCSymbol *Sym) override;

private:
  formatted_raw_ostream &OS;
};

// This part is for ELF object output
class LLIRMCTargetLLIRStreamer : public LLIRMCTargetStreamer {
public:
  LLIRMCTargetLLIRStreamer(MCStreamer &S);

  virtual void emitStackObject(int Offset, int Size, int Align) override;
  virtual void emitVarArg() override;
  virtual void emitFeatures(StringRef cpu, StringRef tuneCPU, StringRef fs) override;
  virtual void emitParams(ArrayRef<LLIRMachineFunctionInfo::Parameter> params) override;
  virtual void emitCallingConv(CallingConv::ID CallConv) override;
  virtual void emitCtor(int Priority, const MCSymbol *Sym) override;
  virtual void emitDtor(int Priority, const MCSymbol *Sym) override;
  virtual void emitThreadLocal() override;
  virtual void emitEnd() override;
  virtual void emitNoInline() override;
  virtual void emitCFIPersonality(const MCSymbol *Sym) override;
};
} // end namespace llvm

#endif
