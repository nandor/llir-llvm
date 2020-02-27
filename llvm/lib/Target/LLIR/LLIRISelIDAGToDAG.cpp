//===-- LLIRISelDAGToDAG.cpp - A dag to dag inst selector for LLIR --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the LLIR target.
//
//===----------------------------------------------------------------------===//

#include "LLIRTargetMachine.h"
#include "LLIR.h"
#include "MCTargetDesc/LLIRMCTargetDesc.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

//===----------------------------------------------------------------------===//
// Instruction Selector Implementation
//===----------------------------------------------------------------------===//

namespace {
class LLIRDAGToDAGISel : public SelectionDAGISel {
public:
  explicit LLIRDAGToDAGISel(LLIRTargetMachine &tm, CodeGenOpt::Level OL)
    : SelectionDAGISel(tm)
  {
  }

  bool runOnMachineFunction(MachineFunction &MF) override
  {
    Subtarget = &MF.getSubtarget<LLIRSubtarget>();
    return SelectionDAGISel::runOnMachineFunction(MF);
  }

  void Select(SDNode *N) override;

  bool SelectInlineAsmMemoryOperand(const SDValue &Op,
                                    unsigned ConstraintID,
                                    std::vector<SDValue> &OutOps) override;

  StringRef getPassName() const override
  {
    return "LLIR DAG->DAG Pattern Instruction Selection";
  }

private:
  const LLIRSubtarget *Subtarget;
  #include "LLIRGenDAGISel.inc"
};
}  // end anonymous namespace

void LLIRDAGToDAGISel::Select(SDNode *N)
{
  if (N->isMachineOpcode()) {
    N->setNodeId(-1);
    return;
  }

  switch (N->getOpcode()) {
    default: {
      SelectCode(N);
      break;
    }
  }
}

bool LLIRDAGToDAGISel::SelectInlineAsmMemoryOperand(
    const SDValue &Op,
    unsigned ConstraintID,
    std::vector<SDValue> &OutOps)
{
  llvm_unreachable("not implemented");
}

FunctionPass *llvm::createLLIRISelDag(LLIRTargetMachine &TM) {
  return new LLIRDAGToDAGISel(TM, CodeGenOpt::Default);
}
