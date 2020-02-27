//===-- LLIRRegisterNumbering.cpp - Register Numbering ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements a pass which assigns LLIR register
/// numbers for CodeGen virtual registers.
///
//===----------------------------------------------------------------------===//

#include "LLIR.h"
#include "LLIRMachineFunctionInfo.h"
#include "LLIRSubtarget.h"
#include "MCTargetDesc/LLIRMCTargetDesc.h"
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

#define DEBUG_TYPE "llir-reg-numbering"

namespace {
class LLIRRegisterNumbering final : public MachineFunctionPass {
  StringRef getPassName() const override {
    return "LLIR Register Numbering";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID; // Pass identification, replacement for typeid
  LLIRRegisterNumbering() : MachineFunctionPass(ID) {}
};
} // end anonymous namespace

char LLIRRegisterNumbering::ID = 0;
INITIALIZE_PASS(
    LLIRRegisterNumbering, DEBUG_TYPE,
    "Assigns LLIR register numbers for virtual registers",
    false,
    false
)

FunctionPass *llvm::createLLIRRegisterNumbering() {
  return new LLIRRegisterNumbering();
}

bool LLIRRegisterNumbering::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs()
      << "********** Register Numbering **********\n"
      << "********** Function: " << MF.getName() << '\n'
  );

  LLIRMachineFunctionInfo &MFI = *MF.getInfo<LLIRMachineFunctionInfo>();
  MachineRegisterInfo &MRI = MF.getRegInfo();

  // Start the numbering for locals after the arg regs
  unsigned CurReg = 1;
  const unsigned NumVRegs = MF.getRegInfo().getNumVirtRegs();
  for (unsigned VRegIdx = 0; VRegIdx < NumVRegs; ++VRegIdx) {
    const unsigned VReg = TargetRegisterInfo::index2VirtReg(VRegIdx);
    if (MRI.def_begin(VReg) == MRI.def_end()) {
      /// Value not defined - map to undefined reg.
      MFI.setGMReg(VReg, 0);
    } else if (!MFI.hasGMReg(VReg)) {
      /// Assign a new number.
      MFI.setGMReg(VReg, CurReg++);
      continue;
    }
  }

  return true;
}
