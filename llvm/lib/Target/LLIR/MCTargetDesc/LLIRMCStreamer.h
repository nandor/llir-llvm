//===- LLIRELFStreamer.h - ELF Object Output --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This is a custom MCELFStreamer which allows us to insert some hooks before
// emitting data into an actual object file.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_MCTARGETDESC_LLIRMCSTREAMER_H
#define LLVM_LIB_TARGET_LLIR_MCTARGETDESC_LLIRMCSTREAMER_H

#include <memory>

#include "llvm/ADT/SmallVector.h"
#include "llvm/MC/MCELFStreamer.h"

namespace llvm {

class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCSubtargetInfo;
struct MCDwarfFrameInfo;

class LLIRMCStreamer : public MCELFStreamer {
public:
  LLIRMCStreamer(MCContext &Context, std::unique_ptr<MCAsmBackend> MAB,
                 std::unique_ptr<MCObjectWriter> OW,
                 std::unique_ptr<MCCodeEmitter> Emitter);

  // Overriding these functions allows us to avoid recording of these labels
  // in EmitLabel and later marking them as microLLIR.
  void emitCFIStartProcImpl(MCDwarfFrameInfo &Frame) override;
  void emitCFIEndProcImpl(MCDwarfFrameInfo &Frame) override;
};

MCELFStreamer *createLLIRMCStreamer(MCContext &Context,
                                    std::unique_ptr<MCAsmBackend> MAB,
                                    std::unique_ptr<MCObjectWriter> OW,
                                    std::unique_ptr<MCCodeEmitter> Emitter,
                                    bool RelaxAll);
}  // end namespace llvm

#endif  // LLVM_LIB_TARGET_LLIR_MCTARGETDESC_LLIRMCSTREAMER_H
