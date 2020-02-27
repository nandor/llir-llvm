//===-- LLIRArgumentMove.cpp - Argument instruction moving ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/LLIRMCTargetDesc.h"
#include "LLIR.h"
#include "LLIRMachineFunctionInfo.h"
#include "LLIRSubtarget.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "llir-argument-move"

namespace {
class LLIRArgumentMove final : public MachineFunctionPass {
public:
  static char ID;
  LLIRArgumentMove() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override { return "LLIR Argument Move"; }

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

char LLIRArgumentMove::ID = 0;
INITIALIZE_PASS(LLIRArgumentMove, DEBUG_TYPE,
                "Move ARGUMENT instructions for LLIR", false, false)

FunctionPass *llvm::createLLIRArgumentMove() {
  return new LLIRArgumentMove();
}

bool LLIRArgumentMove::runOnMachineFunction(MachineFunction &MF) {
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

bool LLIRArgumentMove::isArgument(MachineInstr &MI) {
  switch (MI.getOpcode()) {
  case LLIR::ARG_I32:
  case LLIR::ARG_I64:
  case LLIR::ARG_F32:
  case LLIR::ARG_F64:
  case LLIR::ARG_F80:
    return true;
  default:
    return false;
  }
}
