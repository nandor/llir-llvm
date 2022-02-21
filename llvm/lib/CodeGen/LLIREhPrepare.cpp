//===-- LLIREHPrepare - Prepare excepton handling for LLIR ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Triple.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/InitializePasses.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicsLLIR.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

#define DEBUG_TYPE "llirehprepare"

namespace {
class LLIREHPrepare : public FunctionPass {
  Function *LandingPadFn = nullptr; // llir.landing_pad

public:
  static char ID; // Pass identification, replacement for typeid

  LLIREHPrepare() : FunctionPass(ID) {}
  bool runOnFunction(Function &F) override;

  StringRef getPassName() const override {
    return "LLIR Exception handling preparation";
  }
};
} // end anonymous namespace

char LLIREHPrepare::ID = 0;
INITIALIZE_PASS_BEGIN(LLIREHPrepare, DEBUG_TYPE,
                      "Prepare LLIR exceptions", false, false)
INITIALIZE_PASS_END(LLIREHPrepare, DEBUG_TYPE, "Prepare LLIR exceptions",
                    false, false)

FunctionPass *llvm::createLLIREHPass() { return new LLIREHPrepare(); }

bool LLIREHPrepare::runOnFunction(Function &F) {
  return true;
}
