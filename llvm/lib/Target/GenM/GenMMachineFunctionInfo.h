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
  GenMMachineFunctionInfo(const MachineFunction &MF);

  void addParam(MVT VT) { Params.push_back(VT); }
  const std::vector<MVT> &getParams() const { return Params; }

  void setGMReg(unsigned VReg, unsigned GMReg);
  unsigned getGMReg(unsigned VReg) const;
  bool hasGMReg(unsigned VReg) const;

  void setVAReg(unsigned Reg) { VAReg = Reg; }
  unsigned getVAReg() const { return VAReg; }

private:
  /// Reference to the machine function.
  const MachineFunction &MF;
  /// List of parameters to a function.
  std::vector<MVT> Params;
  /// Reference to the array storing varargs.
  unsigned VAReg;
  /// A mapping from CodeGen vreg index to WebAssembly register number.
  mutable std::vector<unsigned> GMRegs;
};

}

#endif
