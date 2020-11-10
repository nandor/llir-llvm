//===-- LLIRInstrAArch64.td - LLIR AArch64 specific instructions ----------===//
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

let Predicates = [IsAArch64] in {

def ATOMIC_STLXR_32 : I
  < (outs I32:$dst)
  , (ins I32:$val, I64:$addr)
  , [(set I32:$dst, (int_llir_stlxr I32:$val, I64:$addr))]
  , "aarch64_sc.i32\t$dst, $addr, $val"
  , 0xE1
  >;

def ATOMIC_STLXR_64 : I
  < (outs I32:$dst)
  , (ins I64:$val, I64:$addr)
  , [(set I32:$dst, (int_llir_stlxr I64:$val, I64:$addr))]
  , "aarch64_sc.i32\t$dst, $addr, $val"
  , 0xE1
  >;

def ATOMIC_LDAXR_32 : I
  < (outs I32:$dst)
  , (ins I64:$addr)
  , [(set I32:$dst, (int_llir_ldaxr I64:$addr))]
  , "aarch64_ll.i32\t$dst, $addr"
  , 0xE1
  >;

def ATOMIC_LDAXR_64 : I
  < (outs I64:$dst)
  , (ins I64:$addr)
  , [(set I64:$dst, (int_llir_ldaxr I64:$addr))]
  , "aarch64_ll.i32\t$dst, $addr"
  , 0xE1
  >;

}
