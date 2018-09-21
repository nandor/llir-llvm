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

#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {
class Target;

Target &getTheGenMTarget();

} // End llvm namespace


#define GET_REGINFO_ENUM
#include "GenMGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "GenMGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "GenMGenSubtargetInfo.inc"

#endif
