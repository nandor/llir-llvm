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

let Predicates = [IsRISCV] in {

def RISCV_ATOMIC_XCHG_32 : I
  < (outs I64:$dst)
  , (ins I64:$addr, I64:$src)
  , [(set I64:$dst, (atomic_swap_32 I64:$addr, I64:$src))]
  , "riscv_xchg.i64\t$dst, $addr, $src"
  , 0xE1
  >;

def RISCV_ATOMIC_XCHG_64 : I
  < (outs I64:$dst)
  , (ins I64:$addr, I64:$src)
  , [(set I64:$dst, (atomic_swap_64 I64:$addr, I64:$src))]
  , "riscv_xchg.i64\t$dst, $addr, $src"
  , 0xE1
  >;

}
