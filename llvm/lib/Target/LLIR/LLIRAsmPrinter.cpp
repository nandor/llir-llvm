//===-- LLIRAsmPrinter.cpp - LLIR LLVM assembly writer --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to GAS-format SPARC assembly language.
//
//===----------------------------------------------------------------------===//

#include "LLIRAsmPrinter.h"

#include "LLIR.h"
#include "LLIRInstrInfo.h"
#include "LLIRMCInstLower.h"
#include "LLIRMachineFunctionInfo.h"
#include "LLIRTargetMachine.h"
#include "MCTargetDesc/LLIRMCTargetDesc.h"
#include "MCTargetDesc/LLIRMCTargetStreamer.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfoImpls.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

void LLIRAsmPrinter::emitInstruction(const MachineInstr *MI) {
  LLVM_DEBUG(dbgs() << "EmitInstruction: " << *MI << '\n');
  auto &MF = *MI->getParent()->getParent();

  LLIRMCInstLower MCInstLowering(OutContext, MF, *this);
  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

void LLIRAsmPrinter::emitGlobalVariable(const GlobalVariable *GV) {
  AsmPrinter::emitGlobalVariable(GV);
  getTargetStreamer().emitEnd();
}

void LLIRAsmPrinter::emitFunctionBodyStart() {
  MachineFrameInfo &MFI = MF->getFrameInfo();
  auto *FuncInfo = MF->getInfo<LLIRMachineFunctionInfo>();
  auto &F = MF->getFunction();
  auto &Streamer = getTargetStreamer();

  for (unsigned I = 0, N = MFI.getNumObjects(); I < N; ++I) {
    auto Size = MFI.getAnyObjectSize(I);
    if (Size > 0) {
      Streamer.emitStackObject(I, Size, MFI.getAnyObjectAlign(I).value());
    }
  }

  if (F.hasFnAttribute(Attribute::NoInline)) {
    Streamer.emitNoInline();
  }
  if (MF->getFunction().isVarArg()) {
    Streamer.emitVarArg();
  }

  auto &Params = FuncInfo->getParams();
  if (!Params.empty()) {
    Streamer.emitParams(Params);
  }

  Streamer.emitCallingConv(MF->getFunction().getCallingConv());

  AsmPrinter::emitFunctionBodyStart();
}

void LLIRAsmPrinter::emitFunctionBodyEnd() { getTargetStreamer().emitEnd(); }

void LLIRAsmPrinter::emitJumpTableInfo() {
  // Nothing to do; jump tables are incorporated into the instruction stream.
}

void LLIRAsmPrinter::emitXXStructorList(const DataLayout &DL,
                                        const Constant *List, bool IsCtor) {
  auto &Streamer = getTargetStreamer();

  SmallVector<Structor, 8> Structors;
  preprocessXXStructorList(DL, List, Structors);
  if (Structors.empty())
    return;
  for (const Structor &S : Structors) {
    auto *GV = dyn_cast<GlobalValue>(S.Func);
    assert(GV && "not a global value");
    if (IsCtor) {
      Streamer.emitCtor(S.Priority, getSymbol(GV));
    } else {
      Streamer.emitDtor(S.Priority, getSymbol(GV));
    }
  }
}

bool LLIRAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                     const char *ExtraCode, raw_ostream &O) {
  if (ExtraCode) llvm_unreachable("not implemented");

  const MachineOperand &MO = MI->getOperand(OpNo);
  assert(MO.getType() == MachineOperand::MO_Register && "register expected");
  auto &MFI = *MI->getParent()->getParent()->getInfo<LLIRMachineFunctionInfo>();
  O << "$" << MFI.getGMReg(MO.getReg());
  return false;
}

bool LLIRAsmPrinter::PrintAsmMemoryOperand(const MachineInstr *MI,
                                           unsigned OpNo, const char *ExtraCode,
                                           raw_ostream &O) {
  return PrintAsmOperand(MI, OpNo, ExtraCode, O);
}

LLIRMCTargetStreamer &LLIRAsmPrinter::getTargetStreamer() {
  return static_cast<LLIRMCTargetStreamer &>(*OutStreamer->getTargetStreamer());
}

// Force static initialization.
extern "C" void LLVMInitializeLLIRAsmPrinter() {
  RegisterAsmPrinter<LLIRAsmPrinter> X(getTheLLIR_X86_64Target());
  RegisterAsmPrinter<LLIRAsmPrinter> Y(getTheLLIR_AArch64Target());
}
