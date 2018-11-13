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

class FConv
  < GenMRegClass oc
  , GenMRegClass ic
  , string type
  , SDNode node
  , bits<32> op
  >
  : I
    < (outs oc:$dst)
    , (ins ic:$src)
    , [(set oc:$dst, (node ic:$src))]
    , !strconcat(type, "\t$dst, $src")
    , op
    >
{
}

class FTrunc
  < GenMRegClass oc
  , GenMRegClass ic
  , string type
  , SDNode node
  , bits<32> op
  >
  : I
    < (outs oc:$dst)
    , (ins ic:$src)
    , [(set oc:$dst, (node ic:$src))]
    , !strconcat("trunc.", !strconcat(type, "\t$dst, $src"))
    , op
    >
{
}

class FMov
  < GenMRegClass oc
  , GenMRegClass ic
  , string type
  , bits<32> op
  >
  : I
    < (outs oc:$dst)
    , (ins ic:$src)
    , [(set oc:$dst, (bitconvert ic:$src))]
    , !strconcat("mov.", !strconcat(type, "\t$dst, $src"))
    , op
    >
{
}

def I32_TRUNC_S_F32 : FTrunc<I32, F32, "i32", fp_to_sint, 0xF0>;
def I64_TRUNC_S_F32 : FTrunc<I64, F32, "i32", fp_to_sint, 0xF0>;
def I32_TRUNC_S_F64 : FTrunc<I32, F64, "i64", fp_to_sint, 0xF0>;
def I64_TRUNC_S_F64 : FTrunc<I64, F64, "i64", fp_to_sint, 0xF0>;

def I32_TRUNC_U_F32 : FTrunc<I32, F32, "u32", fp_to_uint, 0xF0>;
def I64_TRUNC_U_F32 : FTrunc<I64, F32, "u32", fp_to_uint, 0xF0>;
def I32_TRUNC_U_F64 : FTrunc<I32, F64, "u64", fp_to_uint, 0xF0>;
def I64_TRUNC_U_F64 : FTrunc<I64, F64, "u64", fp_to_uint, 0xF0>;


def F32_CONVERT_S_I32 : FConv<F32, I32, "sext.f32", sint_to_fp, 0xE4>;
def F64_CONVERT_S_I32 : FConv<F64, I32, "sext.f32", sint_to_fp, 0xE6>;
def F32_CONVERT_S_I64 : FConv<F32, I64, "sext.f64", sint_to_fp, 0xE8>;
def F64_CONVERT_S_I64 : FConv<F64, I64, "sext.f64", sint_to_fp, 0xEA>;

def F32_CONVERT_U_I32 : FConv<F32, I32, "zext.f32", uint_to_fp, 0xE5>;
def F64_CONVERT_U_I32 : FConv<F64, I32, "zext.f32", uint_to_fp, 0xE7>;
def F32_CONVERT_U_I64 : FConv<F32, I64, "zext.f64", uint_to_fp, 0xE9>;
def F64_CONVERT_U_I64 : FConv<F64, I64, "zext.f64", uint_to_fp, 0xEB>;

def I32_BITCAST_F32 : FMov<I32, F32, "i32", 0xbc>;
def F32_BITCAST_I32 : FMov<F32, I32, "f32", 0xbe>;
def I64_BITCAST_F64 : FMov<I64, F64, "f64", 0xbd>;
def F64_BITCAST_I64 : FMov<F64, I64, "i64", 0xbf>;

def F64_PROMOTE_F32 : I
  < (outs F64:$dst)
  , (ins F32:$src)
  , [(set F64:$dst, (fpextend F32:$src))]
  , "mov.f64\t$dst, $src"
  , 0xbb
  >;

def F32_DEMOTE_F64 : I
  < (outs F32:$dst)
  , (ins F64:$src)
  , [(set F32:$dst, (fpround F64:$src))]
  , "mov.f32\t$dst, $src"
  , 0xb6
  >;
