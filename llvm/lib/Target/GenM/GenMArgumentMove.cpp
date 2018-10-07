//===-- GenMArgumentMove.cpp - Argument instruction moving ----------------===//
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

#define DEBUG_TYPE "wasm-argument-move"

namespace {
class GenMArgumentMove final : public MachineFunctionPass {
public:
  static char ID; // Pass identification, replacement for typeid
  GenMArgumentMove() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override { return "GenM Argument Move"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    AU.addPreserved<MachineBlockFrequencyInfo>();
    AU.addPreservedID(MachineDominatorsID);
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
};
} // end anonymous namespace

char GenMArgumentMove::ID = 0;
INITIALIZE_PASS(
    GenMArgumentMove,
    DEBUG_TYPE,
    "Move ARGUMENT instructions for GenM",
    false,
    false
)

FunctionPass *llvm::createGenMArgumentMove() {
  return new GenMArgumentMove();
}

bool GenMArgumentMove::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs()
      << "********** Argument Move **********\n"
      << "********** Function: " << MF.getName() << '\n'
  );

  bool Changed = false;
  MachineBasicBlock &EntryMBB = MF.front();
  MachineBasicBlock::iterator InsertPt = EntryMBB.end();

  // Look for the first NonArg instruction.
  for (MachineInstr &MI : EntryMBB) {
    if (!GenM::isArgument(MI)) {
      InsertPt = MI;
      break;
    }
  }

  // Now move any argument instructions later in the block
  // to before our first NonArg instruction.
  for (MachineInstr &MI : llvm::make_range(InsertPt, EntryMBB.end())) {
    if (GenM::isArgument(MI)) {
      EntryMBB.insert(InsertPt, MI.removeFromParent());
      Changed = true;
    }
  }

  return Changed;
}
