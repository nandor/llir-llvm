//===-- LLIRInstrX86.td - LLIR X86 specific instructions ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the LLIR instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

// Landing pad.
def LANDING_PAD_I64_I64 : I
  < (outs I64:$ptr, I64:$sel)
  , (ins)
  , []
  , "landing_pad.i64.i64\t$ptr, $sel"
  , 0x20
  >
{
  let hasSideEffects = 1;
  let isBarrier = 1;
  let isNotDuplicable = 1;
  let hasCtrlDep = 1;
}
