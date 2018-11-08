//===-- GenMISelDAGToDAG.cpp - A dag to dag inst selector for GenM --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the GenM target.
//
//===----------------------------------------------------------------------===//

#include "GenMTargetMachine.h"
#include "GenM.h"
#include "MCTargetDesc/GenMMCTargetDesc.h"
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
class GenMDAGToDAGISel : public SelectionDAGISel {
public:
  explicit GenMDAGToDAGISel(GenMTargetMachine &tm)
    : SelectionDAGISel(tm)
  {
  }

  bool runOnMachineFunction(MachineFunction &MF) override
  {
    Subtarget = &MF.getSubtarget<GenMSubtarget>();
    return SelectionDAGISel::runOnMachineFunction(MF);
  }

  void Select(SDNode *N) override;

  bool SelectInlineAsmMemoryOperand(const SDValue &Op,
                                    unsigned ConstraintID,
                                    std::vector<SDValue> &OutOps) override;

  StringRef getPassName() const override
  {
    return "GenM DAG->DAG Pattern Instruction Selection";
  }

private:
  const GenMSubtarget *Subtarget;
  #include "GenMGenDAGISel.inc"
};
}  // end anonymous namespace

void GenMDAGToDAGISel::Select(SDNode *N)
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

bool GenMDAGToDAGISel::SelectInlineAsmMemoryOperand(
    const SDValue &Op,
    unsigned ConstraintID,
    std::vector<SDValue> &OutOps)
{
  llvm_unreachable("not implemented");
}

FunctionPass *llvm::createGenMISelDag(GenMTargetMachine &TM) {
  return new GenMDAGToDAGISel(TM);
}
