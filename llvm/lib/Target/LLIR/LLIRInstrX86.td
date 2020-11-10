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

let Predicates = [IsX86_64] in {

def ATOMIC_XCHG_64 : I
  < (outs I64:$dst)
  , (ins I64:$addr, I64:$src)
  , [(set I64:$dst, (atomic_swap_64 I64:$addr, I64:$src))]
  , "x86_xchg.i64\t$dst, $addr, $src"
  , 0xE1
  >;

def RDTSC_I64 : I
  < (outs I64:$res)
  , (ins)
  , [(set I64:$res, LLIRrdtsc)]
  , "x86_rdtsc.i64\t$res"
  , 0x15
  >;

}
