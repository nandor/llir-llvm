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
  , (ins G64:$src)
  , [(set I32:$dst, (trunc G64:$src))]
  , "trunc.i32\t$dst, $src"
  , 0xE0
  >;

def XEXT_I32_I64 : I
  < (outs I64:$dst)
  , (ins G32:$src)
  , [(set I64:$dst, (anyext G32:$src))]
  , "zext.i64\t$dst, $src"
  , 0xE1
  >;

def SEXT_I32_I64 : I
  < (outs I64:$dst)
  , (ins G32:$src)
  , [(set I64:$dst, (sext G32:$src))]
  , "sext.i64\t$dst, $src"
  , 0xE2
  >;

def ZEXT_I32_I64 : I
  < (outs I64:$dst)
  , (ins G32:$src)
  , [(set I64:$dst, (zext G32:$src))]
  , "zext.i64\t$dst, $src"
  , 0xE3
  >;
