//===-- LLIRMCTargetDesc.h - LLIR Target Descriptions -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides LLIR specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_MCTARGETDESC_LLIRMCTARGETDESC_H
#define LLVM_LIB_TARGET_LLIR_MCTARGETDESC_LLIRMCTARGETDESC_H

#include <memory>

#include "llvm/MC/MCInstrDesc.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class Target;

Target &getTheLLIR_X86_64Target();
Target &getTheLLIR_AArch64Target();

MCAsmBackend *createLLIRAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                   const MCRegisterInfo &MRI,
                                   const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter> createLLIRObjectWriter();

namespace LLIR {
enum OperandType {
  // Basic block address.
  OPERAND_BASIC_BLOCK = MCOI::OPERAND_FIRST_TARGET,
  // 8-bit integer immediate.
  OPERAND_I8IMM,
  // 16-bit integer immediate.
  OPERAND_I16IMM,
  // 32-bit integer immediate.
  OPERAND_I32IMM,
  // 64-bit integer immediate.
  OPERAND_I64IMM,
  // 32-bit float immediate.
  OPERAND_F32IMM,
  // 64-bit float immediate.
  OPERAND_F64IMM,
  // 80-bit float immediate.
  OPERAND_F80IMM,
  // 32-bit integer offset.
  OPERAND_I32OFF,
  // 32-bit function identifier.
  OPERAND_I64FUNC,
};
}

}  // namespace llvm

#define GET_REGINFO_ENUM
#include "LLIRGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "LLIRGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "LLIRGenSubtargetInfo.inc"

#endif
