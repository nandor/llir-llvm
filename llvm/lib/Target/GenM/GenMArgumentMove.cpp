//===-- GenMArgumentMove.cpp - Argument instruction moving ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/GenMMCTargetDesc.h"
#include "GenM.h"
#include "GenMMachineFunctionInfo.h"
#include "GenMSubtarget.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "genm-argument-move"

namespace {
class GenMArgumentMove final : public MachineFunctionPass {
public:
  static char ID;
  GenMArgumentMove() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override { return "GenM Argument Move"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    AU.addPreserved<MachineBlockFrequencyInfo>();
    AU.addPreservedID(MachineDominatorsID);
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  static bool isArgument(MachineInstr &MI);

  bool runOnMachineFunction(MachineFunction &MF) override;
};
} // end anonymous namespace

char GenMArgumentMove::ID = 0;
INITIALIZE_PASS(GenMArgumentMove, DEBUG_TYPE,
                "Move ARGUMENT instructions for GenM", false, false)

FunctionPass *llvm::createGenMArgumentMove() {
  return new GenMArgumentMove();
}

bool GenMArgumentMove::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG({
    dbgs() << "********** Argument Move **********\n"
           << "********** Function: " << MF.getName() << '\n';
  });

  bool Changed = false;
  MachineBasicBlock &EntryMBB = MF.front();
  MachineBasicBlock::iterator InsertPt = EntryMBB.end();

  // Look for the first NonArg instruction.
  for (MachineInstr &MI : EntryMBB) {
    if (!isArgument(MI)) {
      InsertPt = MI;
      break;
    }
  }

  // Now move any argument instructions later in the block
  // to before our first NonArg instruction.
  for (MachineInstr &MI : llvm::make_range(InsertPt, EntryMBB.end())) {
    if (isArgument(MI)) {
      EntryMBB.insert(InsertPt, MI.removeFromParent());
      Changed = true;
    }
  }

  return Changed;
}

bool GenMArgumentMove::isArgument(MachineInstr &MI) {
  switch (MI.getOpcode()) {
  case GenM::ARG_I32:
  case GenM::ARG_I64:
  case GenM::ARG_F32:
  case GenM::ARG_F64:
  case GenM::ARG_F80:
    return true;
  default:
    return false;
  }
}
