//===-- GenMInstrConv.td-GenM Conversion support -----------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// GenM datatype conversions, truncations, reinterpretations,
/// promotions, and demotions operand code-gen constructs.
///
//===----------------------------------------------------------------------===//

def TRUNC_I64_I32 : I
  < (outs I32:$dst)
  , (ins I64:$src)
  , [(set I32:$dst, (trunc I64:$src))]
  , "trunc.i64.i32\t$dst, $src"
  , 0xE0
  >;


def SEXT_I32_I64 : I
  < (outs I64:$dst)
  , (ins I32:$src)
  , [(set I64:$dst, (sext I32:$src))]
  , "sext.i32.i64\t$dst, $src"
  , 0xE0
  >;
