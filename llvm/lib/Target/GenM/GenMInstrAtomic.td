// GenMInstrAtomics.td-GenM Atomic codegen support-*- tablegen ----------*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// GenM Atomic operand code-gen constructs.
///
//===----------------------------------------------------------------------===//

def ATOMIC_XCHG_64 : I
  < (outs I64:$dst)
  , (ins i64addr_op:$addr, I64:$src)
  , [(set I64:$dst, (atomic_swap_64 (GenMsymbol tglobaladdr:$addr), I64:$src))]
  , "atomic.xchg.i64\t$dst, $addr, $src"
  , 0xE1
  >;
