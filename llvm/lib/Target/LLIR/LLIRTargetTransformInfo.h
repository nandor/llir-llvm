//===-- LLIRTargetTransformInfo.h - LLIR specific TTI -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
/// This file a TargetTransformInfo::Concept conforming object specific to the
/// LLIR target machine. It uses the target's detailed information to
/// provide more precise answers to certain TTI queries, while letting the
/// target independent and default TTI implementations handle the rest.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIRTARGETTRANSFORMINFO_H
#define LLVM_LIB_TARGET_LLIR_LLIRTARGETTRANSFORMINFO_H

#include "LLIRTargetMachine.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/BasicTTIImpl.h"

namespace llvm {

class InstCombiner;

class LLIRTTIImpl : public BasicTTIImplBase<LLIRTTIImpl> {
  typedef BasicTTIImplBase<LLIRTTIImpl> BaseT;
  typedef TargetTransformInfo TTI;
  friend BaseT;

  const LLIRSubtarget *ST;
  const LLIRTargetLowering *TLI;

  const LLIRSubtarget *getST() const { return ST; }
  const LLIRTargetLowering *getTLI() const { return TLI; }

  const FeatureBitset InlineFeatureIgnoreList = {

  };

public:
  explicit LLIRTTIImpl(const LLIRTargetMachine *TM, const Function &F)
      : BaseT(TM, F.getParent()->getDataLayout()), ST(TM->getSubtargetImpl(F)),
        TLI(ST->getTargetLowering()) {}

  bool canMacroFuseCmp();

  bool isLSRCostLess(TargetTransformInfo::LSRCost &C1,
                     TargetTransformInfo::LSRCost &C2);
};

} // end namespace llvm

#endif
