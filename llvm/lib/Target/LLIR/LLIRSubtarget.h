//===-- LLIRSubtarget.h - Define Subtarget for the LLIR ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the LLIR specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LLIR_LLIRSUBTARGET_H
#define LLVM_LIB_TARGET_LLIR_LLIRSUBTARGET_H

#include <string>

#include "LLIRFrameLowering.h"
#include "LLIRISelLowering.h"
#include "LLIRInstrInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"

#define GET_SUBTARGETINFO_HEADER
#include "LLIRGenSubtargetInfo.inc"

namespace llvm {

class LLIRSubtarget : public LLIRGenSubtargetInfo {
 public:
   LLIRSubtarget(const Triple &TT, StringRef CPU, StringRef TuneCPU,
                 StringRef FS, const TargetMachine &TM);

   const LLIRRegisterInfo *getRegisterInfo() const override {
     return &RegisterInfo;
  }

  const LLIRInstrInfo *getInstrInfo() const override { return &InstrInfo; }

  const TargetFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }

  const LLIRTargetLowering *getTargetLowering() const override {
    return &TargetLowering;
  }

  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &DAGTargetInfo;
  }

  bool enableMachineScheduler() const override;

  bool isX86_32() const {
    return TargetTriple.getArch() == llvm::Triple::llir_x86;
  }

  bool isX86_64() const {
    return TargetTriple.getArch() == llvm::Triple::llir_x86_64;
  }

  bool isX86() const { return isX86_32() || isX86_64(); }

  bool isAArch64() const {
    return TargetTriple.getArch() == llvm::Triple::llir_aarch64;
  }

  bool isPPC64le() const {
    return TargetTriple.getArch() == llvm::Triple::llir_ppc64le;
  }

  bool isRISCV() const {
    return TargetTriple.getArch() == llvm::Triple::llir_riscv64;
  }

  bool isTargetDarwin() const { return TargetTriple.isOSDarwin(); }

  bool isTargetWindows() const { return TargetTriple.isOSWindows(); }

  bool isTargetWin64() const {
    return TargetTriple.isArch64Bit() && isTargetWindows();
  }

  bool isTargetILP32() const { return TargetTriple.isArch32Bit(); }

  bool is32Bit() const { return TargetTriple.isArch32Bit(); }
  bool is64Bit() const { return TargetTriple.isArch64Bit(); }

  bool isCallingConvWin64(CallingConv::ID CC) const {
    if (isX86_64()) {
      switch (CC) {
      // On Win64, all these conventions just use the default convention.
      case CallingConv::C:
      case CallingConv::Fast:
      case CallingConv::Tail:
      case CallingConv::Swift:
      case CallingConv::X86_FastCall:
      case CallingConv::X86_StdCall:
      case CallingConv::X86_ThisCall:
      case CallingConv::X86_VectorCall:
      case CallingConv::Intel_OCL_BI:
        return isTargetWin64();
      // This convention allows using the Win64 convention on other targets.
      case CallingConv::Win64:
        return true;
      // This convention allows using the SysV convention on Windows targets.
      case CallingConv::X86_64_SysV:
        return false;
      // Otherwise, who knows what this is.
      default:
        return false;
      }
    } else if (isAArch64()) {
      switch (CC) {
      case CallingConv::C:
      case CallingConv::Fast:
      case CallingConv::Swift:
        return isTargetWindows();
      case CallingConv::Win64:
        return true;
      default:
        return false;
      }
    } else {
      return false;
    }
  }

  bool hasMacroFusion() const {
    return isX86_64();
  }

  bool hasBranchFusion() const {
    return isX86_64();
  }

  bool hasP9Vector() const { return false; }

 private:
  void ParseSubtargetFeatures(StringRef CPU, StringRef TrueCPU, StringRef FS);

 private:
  Triple TargetTriple;

  LLIRRegisterInfo RegisterInfo;
  LLIRInstrInfo InstrInfo;
  LLIRTargetLowering TargetLowering;
  LLIRFrameLowering FrameLowering;
  SelectionDAGTargetInfo DAGTargetInfo;
};

}  // end namespace llvm

#endif
