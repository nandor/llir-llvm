//===-- LLIRInstrConv.td-LLIR Conversion support -----------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// LLIR datatype conversions, truncations, reinterpretations,
/// promotions, and demotions operand code-gen constructs.
///
//===----------------------------------------------------------------------===//

class Conv
  < LLIRRegClass oc
  , LLIRRegClass ic
  , SDNode node
  , string name
  , string type
  , bits<32> op
  >
  : I
    < (outs oc:$dst)
    , (ins ic:$src)
    , [(set oc:$dst, (node ic:$src))]
    , !strconcat(name, !strconcat(".", !strconcat(type, "\t$dst, $src")))
    , op
    >
{
}

def TRUNC_I16_I8  : Conv<I8,  I16, trunc,  "trunc", "i8",  0x00>;
def TRUNC_I32_I8  : Conv<I8,  I32, trunc,  "trunc", "i8",  0x00>;
def TRUNC_I64_I8  : Conv<I8,  I64, trunc,  "trunc", "i8",  0x00>;
def TRUNC_I32_I16 : Conv<I16, I32, trunc,  "trunc", "i16", 0x00>;
def TRUNC_I64_I16 : Conv<I16, I64, trunc,  "trunc", "i16", 0x00>;
def TRUNC_I64_I32 : Conv<I32, I64, trunc,  "trunc", "i32", 0x00>;

def XEXT_I8_I16   : Conv<I16, I8,  anyext, "zext",  "i16", 0x00>;
def XEXT_I8_I32   : Conv<I32, I8,  anyext, "zext",  "i32", 0x00>;
def XEXT_I16_I32  : Conv<I32, I16, anyext, "zext",  "i32", 0x00>;
def XEXT_I8_I64   : Conv<I64, I8,  anyext, "zext",  "i64", 0x00>;
def XEXT_I16_I64  : Conv<I64, I16, anyext, "zext",  "i64", 0x00>;
def XEXT_I32_I64  : Conv<I64, I32, anyext, "zext",  "i64", 0x00>;

def SEXT_I8_I16   : Conv<I16, I8,  sext,   "sext",  "i16", 0x00>;
def SEXT_I8_I32   : Conv<I32, I8,  sext,   "sext",  "i32", 0x00>;
def SEXT_I16_I32  : Conv<I32, I16, sext,   "sext",  "i32", 0x00>;
def SEXT_I8_I64   : Conv<I64, I8,  sext,   "sext",  "i64", 0x00>;
def SEXT_I16_I64  : Conv<I64, I16, sext,   "sext",  "i64", 0x00>;
def SEXT_I32_I64  : Conv<I64, I32, sext,   "sext",  "i64", 0x00>;

def ZEXT_I8_I16   : Conv<I16, I8,  zext,   "zext",  "i16", 0x00>;
def ZEXT_I8_I32   : Conv<I32, I8,  zext,   "zext",  "i32", 0x00>;
def ZEXT_I16_I32  : Conv<I32, I16, zext,   "zext",  "i32", 0x00>;
def ZEXT_I8_I64   : Conv<I64, I8,  zext,   "zext",  "i64", 0x00>;
def ZEXT_I16_I64  : Conv<I64, I16, zext,   "zext",  "i64", 0x00>;
def ZEXT_I32_I64  : Conv<I64, I32, zext,   "zext",  "i64", 0x00>;

class FConv
  < LLIRRegClass oc
  , LLIRRegClass ic
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
  < LLIRRegClass oc
  , LLIRRegClass ic
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
  < LLIRRegClass oc
  , LLIRRegClass ic
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
def I64_TRUNC_S_F32 : FTrunc<I64, F32, "i64", fp_to_sint, 0xF0>;
def I32_TRUNC_S_F64 : FTrunc<I32, F64, "i32", fp_to_sint, 0xF0>;
def I64_TRUNC_S_F64 : FTrunc<I64, F64, "i64", fp_to_sint, 0xF0>;
def I32_TRUNC_S_F80 : FTrunc<I32, F80, "i32", fp_to_sint, 0xF0>;
def I64_TRUNC_S_F80 : FTrunc<I64, F80, "i64", fp_to_sint, 0xF0>;


def I32_TRUNC_U_F32 : FTrunc<I32, F32, "u32", fp_to_uint, 0xF0>;
def I64_TRUNC_U_F32 : FTrunc<I64, F32, "u64", fp_to_uint, 0xF0>;
def I32_TRUNC_U_F64 : FTrunc<I32, F64, "u32", fp_to_uint, 0xF0>;
def I64_TRUNC_U_F64 : FTrunc<I64, F64, "u64", fp_to_uint, 0xF0>;
def I32_TRUNC_U_F80 : FTrunc<I32, F80, "u32", fp_to_uint, 0xF0>;
def I64_TRUNC_U_F80 : FTrunc<I64, F80, "u64", fp_to_uint, 0xF0>;

def F32_CONVERT_S_I32 : FConv<F32, I32, "sext.f32", sint_to_fp, 0xE4>;
def F64_CONVERT_S_I32 : FConv<F64, I32, "sext.f64", sint_to_fp, 0xE6>;
def F80_CONVERT_S_I32 : FConv<F80, I32, "sext.f80", sint_to_fp, 0xE6>;
def F32_CONVERT_S_I64 : FConv<F32, I64, "sext.f32", sint_to_fp, 0xE8>;
def F64_CONVERT_S_I64 : FConv<F64, I64, "sext.f64", sint_to_fp, 0xEA>;
def F80_CONVERT_S_I64 : FConv<F80, I64, "sext.f80", sint_to_fp, 0xEA>;

def F32_CONVERT_U_I8  : FConv<F32,  I8, "zext.f32", uint_to_fp, 0xE5>;
def F64_CONVERT_U_I8  : FConv<F64,  I8, "zext.f64", uint_to_fp, 0xE7>;
def F80_CONVERT_U_I8  : FConv<F80,  I8, "zext.f80", uint_to_fp, 0xE7>;
def F32_CONVERT_U_I16 : FConv<F32, I16, "zext.f32", uint_to_fp, 0xE5>;
def F64_CONVERT_U_I16 : FConv<F64, I16, "zext.f64", uint_to_fp, 0xE7>;
def F80_CONVERT_U_I16 : FConv<F80, I16, "zext.f80", uint_to_fp, 0xE7>;
def F32_CONVERT_U_I32 : FConv<F32, I32, "zext.f32", uint_to_fp, 0xE5>;
def F64_CONVERT_U_I32 : FConv<F64, I32, "zext.f64", uint_to_fp, 0xE7>;
def F80_CONVERT_U_I32 : FConv<F80, I32, "zext.f80", uint_to_fp, 0xE7>;
def F32_CONVERT_U_I64 : FConv<F32, I64, "zext.f32", uint_to_fp, 0xE9>;
def F64_CONVERT_U_I64 : FConv<F64, I64, "zext.f64", uint_to_fp, 0xEB>;
def F80_CONVERT_U_I64 : FConv<F80, I64, "zext.f80", uint_to_fp, 0xEB>;

def I32_BITCAST_F32 : FMov<I32, F32, "i32", 0xbc>;
def F32_BITCAST_I32 : FMov<F32, I32, "f32", 0xbe>;
def I64_BITCAST_F64 : FMov<I64, F64, "i64", 0xbd>;
def F64_BITCAST_I64 : FMov<F64, I64, "f64", 0xbf>;

def F64_PROMOTE_F32 : I
  < (outs F64:$dst)
  , (ins F32:$src)
  , [(set F64:$dst, (fpextend F32:$src))]
  , "fext.f64\t$dst, $src"
  , 0xbb
  >;

def F80_PROMOTE_F32 : I
  < (outs F80:$dst)
  , (ins F32:$src)
  , [(set F80:$dst, (fpextend F32:$src))]
  , "fext.f80\t$dst, $src"
  , 0xbb
  >;

def F80_PROMOTE_F64 : I
  < (outs F80:$dst)
  , (ins F64:$src)
  , [(set F80:$dst, (fpextend F64:$src))]
  , "fext.f80\t$dst, $src"
  , 0xbb
  >;

def F32_DEMOTE_F64 : I
  < (outs F32:$dst)
  , (ins F64:$src)
  , [(set F32:$dst, (fpround F64:$src))]
  , "trunc.f32\t$dst, $src"
  , 0xb6
  >;

def F32_DEMOTE_F80 : I
  < (outs F32:$dst)
  , (ins F80:$src)
  , [(set F32:$dst, (fpround F80:$src))]
  , "trunc.f32\t$dst, $src"
  , 0xb6
  >;

def F64_DEMOTE_F80 : I
  < (outs F64:$dst)
  , (ins F80:$src)
  , [(set F64:$dst, (fpround F80:$src))]
  , "trunc.f64\t$dst, $src"
  , 0xb6
  >;
