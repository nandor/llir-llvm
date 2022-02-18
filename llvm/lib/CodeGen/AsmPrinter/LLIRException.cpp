//===-- CodeGen/AsmPrinter/LLIRException.cpp - LLIR Exception Impl ------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LLIRException.h"

#include "llvm/ADT/Twine.h"
#include "llvm/BinaryFormat/LLIR.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSection.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
using namespace llvm;

LLIRException::LLIRException(AsmPrinter *A) : EHStreamer(A) {}

LLIRException::~LLIRException() {}

void LLIRException::markFunctionEnd() {}

void LLIRException::endFragment() {}

void LLIRException::endModule() {}

void LLIRException::beginFunction(const MachineFunction *MF) {}

void LLIRException::endFunction(const MachineFunction *) {}

void LLIRException::beginFragment(const MachineBasicBlock *MBB,
                                  ExceptionSymbolProvider ESP) {}

void LLIRException::beginBasicBlock(const MachineBasicBlock &MBB) {}

void LLIRException::endBasicBlock(const MachineBasicBlock &MBB) {}
