//===- LLIRMachineFunctionInfo.h - LLIR Machine Function Info ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares  LLIR specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIRMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_LLIR_LLIRMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

class LLIRMachineFunctionInfo : public MachineFunctionInfo {
public:
  LLIRMachineFunctionInfo(const MachineFunction &MF);

  /// Information about parameters.
  struct Parameter {
    MVT VT;
    uint64_t Flags;

    Parameter(MVT VT, uint64_t Flags) : VT(VT), Flags(Flags) {}
  };
  void addParam(MVT VT, uint64_t Flags) { Params.emplace_back(VT, Flags); }
  const std::vector<Parameter> &getParams() const { return Params; }

  void setGMReg(unsigned VReg, unsigned GMReg);
  unsigned getGMReg(unsigned VReg) const;
  bool hasGMReg(unsigned VReg) const;

private:
  /// Reference to the machine function.
  const MachineFunction &MF;
  /// List of parameters to a function.
  std::vector<Parameter> Params;
  /// A mapping from CodeGen vreg index to WebAssembly register number.
  mutable std::vector<unsigned> GMRegs;
};

}

#endif
