//===-- LLIRTargetTransformInfo.cpp - LLIR specific TTI pass --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LLIRTargetTransformInfo.h"

using namespace llvm;

#define DEBUG_TYPE "llirtti"

bool LLIRTTIImpl::canMacroFuseCmp() {
  return ST->hasMacroFusion() || ST->hasBranchFusion();
}

bool LLIRTTIImpl::isLSRCostLess(TargetTransformInfo::LSRCost &C1,
                               TargetTransformInfo::LSRCost &C2) {
  if (ST->isX86()) {
    // X86 specific here are "instruction number 1st priority".
    return std::tie(C1.Insns, C1.NumRegs, C1.AddRecCost,
                    C1.NumIVMuls, C1.NumBaseAdds,
                    C1.ScaleCost, C1.ImmCost, C1.SetupCost) <
           std::tie(C2.Insns, C2.NumRegs, C2.AddRecCost,
                    C2.NumIVMuls, C2.NumBaseAdds,
                    C2.ScaleCost, C2.ImmCost, C2.SetupCost);
  }
  return BaseT::isLSRCostLess(C1, C2);
}
