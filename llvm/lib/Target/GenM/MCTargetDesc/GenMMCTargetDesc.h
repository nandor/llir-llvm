//===-- GenMMCTargetDesc.h - GenM Target Descriptions -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides GenM specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_MCTARGETDESC_GENMMCTARGETDESC_H
#define LLVM_LIB_TARGET_GENM_MCTARGETDESC_GENMMCTARGETDESC_H

#include "llvm/MC/MCInstrDesc.h"
#include "llvm/Support/DataTypes.h"
#include <memory>

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

Target &getTheGenMTarget();

MCCodeEmitter *createGenMMCCodeEmitter(
    const MCInstrInfo &MCII,
    const MCRegisterInfo &MRI,
    MCContext &Ctx
);

MCAsmBackend *createGenMAsmBackend(
    const Target &T,
    const MCSubtargetInfo &STI,
    const MCRegisterInfo &MRI,
    const MCTargetOptions &Options
);

std::unique_ptr<MCObjectTargetWriter> createGenMObjectWriter();

namespace GenM {
  enum OperandType {
    // Basic block address.
    OPERAND_BASIC_BLOCK = MCOI::OPERAND_FIRST_TARGET,
    // 32-bit integer immediate.
    OPERAND_I32IMM,
    // 64-bit integer immediate.
    OPERAND_I64IMM,
    // 32-bit float immediate.
    OPERAND_F32IMM,
    // 64-bit float immediate.
    OPERAND_F64IMM,
    // 32-bit integer offset.
    OPERAND_I32OFF,
    // 32-bit function identifier.
    OPERAND_I64FUNC,
  };
};

} // End llvm namespace


#define GET_REGINFO_ENUM
#include "GenMGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "GenMGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "GenMGenSubtargetInfo.inc"

#endif
