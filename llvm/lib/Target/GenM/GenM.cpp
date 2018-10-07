//===-- GenMUtilities.cpp - GenM Utility Functions ------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements several utility functions for GenM.
///
//===----------------------------------------------------------------------===//

#include "GenM.h"
#include "MCTargetDesc/GenMMCTargetDesc.h"
#include "llvm/CodeGen/MachineInstr.h"


bool llvm::GenM::isArgument(MachineInstr &MI)
{
  switch (MI.getOpcode()) {
    case GenM::ARG_I32:
    case GenM::ARG_I64: {
      return true;
    }
    default: {
      return false;
    }
  }
}
