//===-- GenMRegisterNumbering.cpp - Register Numbering ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements a pass which assigns GenM register
/// numbers for CodeGen virtual registers.
///
//===----------------------------------------------------------------------===//

#include "GenM.h"
#include "GenMMachineFunctionInfo.h"
#include "GenMSubtarget.h"
#include "MCTargetDesc/GenMMCTargetDesc.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineLoopInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "genm-reg-numbering"

namespace {
class GenMRegisterNumbering final : public MachineFunctionPass {
  StringRef getPassName() const override {
    return "GenM Register Numbering";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID; // Pass identification, replacement for typeid
  GenMRegisterNumbering() : MachineFunctionPass(ID) {}
};
} // end anonymous namespace

char GenMRegisterNumbering::ID = 0;
INITIALIZE_PASS(
    GenMRegisterNumbering, DEBUG_TYPE,
    "Assigns GenM register numbers for virtual registers",
    false,
    false
)

FunctionPass *llvm::createGenMRegisterNumbering() {
  return new GenMRegisterNumbering();
}

bool GenMRegisterNumbering::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs()
      << "********** Register Numbering **********\n"
      << "********** Function: " << MF.getName() << '\n'
  );

  GenMMachineFunctionInfo &MFI = *MF.getInfo<GenMMachineFunctionInfo>();

  // Start the numbering for locals after the arg regs
  unsigned CurReg = MFI.getParams().size();
  const unsigned NumVRegs = MF.getRegInfo().getNumVirtRegs();
  for (unsigned VRegIdx = 0; VRegIdx < NumVRegs; ++VRegIdx) {
    const unsigned VReg = TargetRegisterInfo::index2VirtReg(VRegIdx);
    // Map registers.
    if (!MFI.hasGMReg(VReg)) {
      MFI.setGMReg(VReg, CurReg++);
      continue;
    }
  }

  return true;
}
