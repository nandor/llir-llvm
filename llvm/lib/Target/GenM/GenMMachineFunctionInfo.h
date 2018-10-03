//===- GenMMachineFunctionInfo.h - GenM Machine Function Info ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares  GenM specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_GENM_GENMMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_GENM_GENMMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

class GenMMachineFunctionInfo : public MachineFunctionInfo {
public:
  GenMMachineFunctionInfo();
  GenMMachineFunctionInfo(const MachineFunction &MF);
};

}

#endif
