//===-- GenMMachineFunctionInfo.cpp - GenM Machine Function Info --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "GenMMachineFunctionInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

GenMMachineFunctionInfo::GenMMachineFunctionInfo(const MachineFunction &MF)
  : MF(MF)
{
}

static constexpr unsigned UnusedReg = -1u;

void GenMMachineFunctionInfo::setGMReg(unsigned VReg, unsigned GMReg) {
  if (GMRegs.empty()) {
    GMRegs.resize(MF.getRegInfo().getNumVirtRegs(), UnusedReg);
  }

  assert(GMReg != UnusedReg);
  const auto I = TargetRegisterInfo::virtReg2Index(VReg);
  assert(I < GMRegs.size());
  GMRegs[I] = GMReg;
}

unsigned GenMMachineFunctionInfo::getGMReg(unsigned VReg) const {
  if (GMRegs.empty()) {
    GMRegs.resize(MF.getRegInfo().getNumVirtRegs(), UnusedReg);
  }

  const auto I = TargetRegisterInfo::virtReg2Index(VReg);
  assert(I < GMRegs.size());
  const auto R = GMRegs[I];
  assert(R != UnusedReg && "Invalid register");
  return R;
}

bool GenMMachineFunctionInfo::hasGMReg(unsigned VReg) const {
  if (GMRegs.empty()) {
    GMRegs.resize(MF.getRegInfo().getNumVirtRegs(), UnusedReg);
  }

  const auto I = TargetRegisterInfo::virtReg2Index(VReg);
  assert(I < GMRegs.size());
  return GMRegs[I] != UnusedReg;
}
