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

//===----------------------------------------------------------------------===//
// Integer extension
//===----------------------------------------------------------------------===//

multiclass Ext
  < SDNode node
  , string name
  , bits<32> op
  >
{
  def _I8_I16   : Conv<I16,  I8,   node, name,  "i16",  0x00>;
  def _I8_I32   : Conv<I32,  I8,   node, name,  "i32",  0x00>;
  def _I16_I32  : Conv<I32,  I16,  node, name,  "i32",  0x00>;
  def _I8_I64   : Conv<I64,  I8,   node, name,  "i64",  0x00>;
  def _I16_I64  : Conv<I64,  I16,  node, name,  "i64",  0x00>;
  def _I32_I64  : Conv<I64,  I32,  node, name,  "i64",  0x00>;
  def _I8_I128  : Conv<I128, I8,   node, name,  "i128", 0x00>;
  def _I16_I128 : Conv<I128, I16,  node, name,  "i128", 0x00>;
  def _I32_I128 : Conv<I128, I32,  node, name,  "i128", 0x00>;
  def _I64_I128 : Conv<I128, I64,  node, name,  "i128", 0x00>;
}

defm XEXT : Ext<anyext, "x_ext", 0x00>;
defm SEXT : Ext<sext,   "s_ext", 0x00>;
defm ZEXT : Ext<zext,   "z_ext", 0x00>;

//===----------------------------------------------------------------------===//
// Integer truncation
//===----------------------------------------------------------------------===//

def TRUNC_I16_I8   : Conv<I8,  I16,  trunc,  "trunc", "i8",  0x00>;
def TRUNC_I32_I8   : Conv<I8,  I32,  trunc,  "trunc", "i8",  0x00>;
def TRUNC_I64_I8   : Conv<I8,  I64,  trunc,  "trunc", "i8",  0x00>;
def TRUNC_I128_I8  : Conv<I8,  I128, trunc,  "trunc", "i8",  0x00>;
def TRUNC_I32_I16  : Conv<I16, I32,  trunc,  "trunc", "i16", 0x00>;
def TRUNC_I64_I16  : Conv<I16, I64,  trunc,  "trunc", "i16", 0x00>;
def TRUNC_I128_I16 : Conv<I16, I128, trunc,  "trunc", "i16", 0x00>;
def TRUNC_I64_I32  : Conv<I32, I64,  trunc,  "trunc", "i32", 0x00>;
def TRUNC_I128_I32 : Conv<I32, I128, trunc,  "trunc", "i32", 0x00>;
def TRUNC_I128_I64 : Conv<I64, I128, trunc,  "trunc", "i64", 0x00>;

//===----------------------------------------------------------------------===//
// FP extension
//===----------------------------------------------------------------------===//

multiclass StrictConv
  < LLIRRegClass oc
  , LLIRRegClass ic
  , SDNode node
  , SDNode strict
  , string name
  , string type
  , bits<32> op
  >
{
  def : Conv<oc, ic, node, name, type, op>;
  def _STRICT : Conv<oc, ic, strict, name, !strconcat(type, ".strict"), op>;
}

defm I8_R_S_F32    : StrictConv<I8,  F32,  fp_to_sint, strict_fp_to_sint, "s_ext", "i8",  0xF0>;
defm I16_R_S_F32   : StrictConv<I16, F32,  fp_to_sint, strict_fp_to_sint, "s_ext", "i16", 0xF0>;
defm I32_R_S_F32   : StrictConv<I32, F32,  fp_to_sint, strict_fp_to_sint, "s_ext", "i32", 0xF0>;
defm I64_R_S_F32   : StrictConv<I64, F32,  fp_to_sint, strict_fp_to_sint, "s_ext", "i64", 0xF0>;
defm I8_R_S_F64    : StrictConv<I8,  F64,  fp_to_sint, strict_fp_to_sint, "s_ext", "i8",  0xF0>;
defm I16_R_S_F64   : StrictConv<I16, F64,  fp_to_sint, strict_fp_to_sint, "s_ext", "i16", 0xF0>;
defm I32_R_S_F64   : StrictConv<I32, F64,  fp_to_sint, strict_fp_to_sint, "s_ext", "i32", 0xF0>;
defm I64_R_S_F64   : StrictConv<I64, F64,  fp_to_sint, strict_fp_to_sint, "s_ext", "i64", 0xF0>;
defm I8_R_S_F80    : StrictConv<I8,  F80,  fp_to_sint, strict_fp_to_sint, "s_ext", "i8",  0xF0>;
defm I16_R_S_F80   : StrictConv<I16, F80,  fp_to_sint, strict_fp_to_sint, "s_ext", "i16", 0xF0>;
defm I32_R_S_F80   : StrictConv<I32, F80,  fp_to_sint, strict_fp_to_sint, "s_ext", "i32", 0xF0>;
defm I64_R_S_F80   : StrictConv<I64, F80,  fp_to_sint, strict_fp_to_sint, "s_ext", "i64", 0xF0>;
defm I8_R_S_F128   : StrictConv<I8,  F128, fp_to_sint, strict_fp_to_sint, "s_ext", "i8",  0xF0>;
defm I16_R_S_F128  : StrictConv<I16, F128, fp_to_sint, strict_fp_to_sint, "s_ext", "i16", 0xF0>;
defm I32_R_S_F128  : StrictConv<I32, F128, fp_to_sint, strict_fp_to_sint, "s_ext", "i32", 0xF0>;
defm I64_R_S_F128  : StrictConv<I64, F128, fp_to_sint, strict_fp_to_sint, "s_ext", "i64", 0xF0>;

defm I8_R_U_F32    : StrictConv<I8,  F32,  fp_to_uint, strict_fp_to_uint, "z_ext", "i8",  0xF0>;
defm I16_R_U_F32   : StrictConv<I16, F32,  fp_to_uint, strict_fp_to_uint, "z_ext", "i16", 0xF0>;
defm I32_R_U_F32   : StrictConv<I32, F32,  fp_to_uint, strict_fp_to_uint, "z_ext", "i32", 0xF0>;
defm I64_R_U_F32   : StrictConv<I64, F32,  fp_to_uint, strict_fp_to_uint, "z_ext", "i64", 0xF0>;
defm I8_R_U_F64    : StrictConv<I8,  F64,  fp_to_uint, strict_fp_to_uint, "z_ext", "i8",  0xF0>;
defm I16_R_U_F64   : StrictConv<I16, F64,  fp_to_uint, strict_fp_to_uint, "z_ext", "i16", 0xF0>;
defm I32_R_U_F64   : StrictConv<I32, F64,  fp_to_uint, strict_fp_to_uint, "z_ext", "i32", 0xF0>;
defm I64_R_U_F64   : StrictConv<I64, F64,  fp_to_uint, strict_fp_to_uint, "z_ext", "i64", 0xF0>;
defm I8_R_U_F80    : StrictConv<I8,  F80,  fp_to_uint, strict_fp_to_uint, "z_ext", "i8",  0xF0>;
defm I16_R_U_F80   : StrictConv<I16, F80,  fp_to_uint, strict_fp_to_uint, "z_ext", "i16", 0xF0>;
defm I32_R_U_F80   : StrictConv<I32, F80,  fp_to_uint, strict_fp_to_uint, "z_ext", "i32", 0xF0>;
defm I64_R_U_F80   : StrictConv<I64, F80,  fp_to_uint, strict_fp_to_uint, "z_ext", "i64", 0xF0>;
defm I8_R_U_F128   : StrictConv<I8,  F128, fp_to_uint, strict_fp_to_uint, "z_ext", "i8",  0xF0>;
defm I16_R_U_F128  : StrictConv<I16, F128, fp_to_uint, strict_fp_to_uint, "z_ext", "i16", 0xF0>;
defm I32_R_U_F128  : StrictConv<I32, F128, fp_to_uint, strict_fp_to_uint, "z_ext", "i32", 0xF0>;
defm I64_R_U_F128  : StrictConv<I64, F128, fp_to_uint, strict_fp_to_uint, "z_ext", "i64", 0xF0>;

defm F32_R_S_I8   : StrictConv<F32,  I8,  sint_to_fp, strict_sint_to_fp, "s_ext", "f32",  0xE4>;
defm F64_R_S_I8   : StrictConv<F64,  I8,  sint_to_fp, strict_sint_to_fp, "s_ext", "f64",  0xE6>;
defm F80_R_S_I8   : StrictConv<F80,  I8,  sint_to_fp, strict_sint_to_fp, "s_ext", "f80",  0xE6>;
defm F128_R_S_I8  : StrictConv<F128, I8,  sint_to_fp, strict_sint_to_fp, "s_ext", "f128", 0xE6>;
defm F32_R_S_I16  : StrictConv<F32,  I16, sint_to_fp, strict_sint_to_fp, "s_ext", "f32",  0xE4>;
defm F64_R_S_I16  : StrictConv<F64,  I16, sint_to_fp, strict_sint_to_fp, "s_ext", "f64",  0xE6>;
defm F80_R_S_I16  : StrictConv<F80,  I16, sint_to_fp, strict_sint_to_fp, "s_ext", "f80",  0xE6>;
defm F128_R_S_I16 : StrictConv<F128, I16, sint_to_fp, strict_sint_to_fp, "s_ext", "f128", 0xE6>;
defm F32_R_S_I32  : StrictConv<F32,  I32, sint_to_fp, strict_sint_to_fp, "s_ext", "f32",  0xE4>;
defm F64_R_S_I32  : StrictConv<F64,  I32, sint_to_fp, strict_sint_to_fp, "s_ext", "f64",  0xE6>;
defm F80_R_S_I32  : StrictConv<F80,  I32, sint_to_fp, strict_sint_to_fp, "s_ext", "f80",  0xE6>;
defm F128_R_S_I32 : StrictConv<F128, I32, sint_to_fp, strict_sint_to_fp, "s_ext", "f128", 0xE6>;
defm F32_R_S_I64  : StrictConv<F32,  I64, sint_to_fp, strict_sint_to_fp, "s_ext", "f32",  0xE8>;
defm F64_R_S_I64  : StrictConv<F64,  I64, sint_to_fp, strict_sint_to_fp, "s_ext", "f64",  0xEA>;
defm F80_R_S_I64  : StrictConv<F80,  I64, sint_to_fp, strict_sint_to_fp, "s_ext", "f80",  0xEA>;
defm F128_R_S_I64 : StrictConv<F128, I64, sint_to_fp, strict_sint_to_fp, "s_ext", "f128", 0xEA>;

defm F32_R_U_I8   : StrictConv<F32,  I8,  uint_to_fp, strict_uint_to_fp, "z_ext", "f32",  0xE5>;
defm F64_R_U_I8   : StrictConv<F64,  I8,  uint_to_fp, strict_uint_to_fp, "z_ext", "f64",  0xE7>;
defm F80_R_U_I8   : StrictConv<F80,  I8,  uint_to_fp, strict_uint_to_fp, "z_ext", "f80",  0xE7>;
defm F128_R_U_I8  : StrictConv<F128, I8,  uint_to_fp, strict_uint_to_fp, "z_ext", "f128", 0xE7>;
defm F32_R_U_I16  : StrictConv<F32,  I16, uint_to_fp, strict_uint_to_fp, "z_ext", "f32",  0xE5>;
defm F64_R_U_I16  : StrictConv<F64,  I16, uint_to_fp, strict_uint_to_fp, "z_ext", "f64",  0xE7>;
defm F80_R_U_I16  : StrictConv<F80,  I16, uint_to_fp, strict_uint_to_fp, "z_ext", "f80",  0xE7>;
defm F128_R_U_I16 : StrictConv<F128, I16, uint_to_fp, strict_uint_to_fp, "z_ext", "f128", 0xE7>;
defm F32_R_U_I32  : StrictConv<F32,  I32, uint_to_fp, strict_uint_to_fp, "z_ext", "f32",  0xE5>;
defm F64_R_U_I32  : StrictConv<F64,  I32, uint_to_fp, strict_uint_to_fp, "z_ext", "f64",  0xE7>;
defm F80_R_U_I32  : StrictConv<F80,  I32, uint_to_fp, strict_uint_to_fp, "z_ext", "f80",  0xE7>;
defm F128_R_U_I32 : StrictConv<F128, I32, uint_to_fp, strict_uint_to_fp, "z_ext", "f128", 0xE7>;
defm F32_R_U_I64  : StrictConv<F32,  I64, uint_to_fp, strict_uint_to_fp, "z_ext", "f32",  0xE9>;
defm F64_R_U_I64  : StrictConv<F64,  I64, uint_to_fp, strict_uint_to_fp, "z_ext", "f64",  0xEB>;
defm F80_R_U_I64  : StrictConv<F80,  I64, uint_to_fp, strict_uint_to_fp, "z_ext", "f80",  0xEB>;
defm F128_R_U_I64 : StrictConv<F128, I64, uint_to_fp, strict_uint_to_fp, "z_ext", "f128", 0xEB>;


//===----------------------------------------------------------------------===//
// FP-FP conversion
//===----------------------------------------------------------------------===//

defm F64_PROMOTE_F32  : StrictConv<F64,  F32, fpextend, strict_fpextend, "f_ext", "f64", 0xbb>;
defm F80_PROMOTE_F32  : StrictConv<F80,  F32, fpextend, strict_fpextend, "f_ext", "f80", 0xbb>;
defm F80_PROMOTE_F64  : StrictConv<F80,  F64, fpextend, strict_fpextend, "f_ext", "f80", 0xbb>;
defm F128_PROMOTE_F32 : StrictConv<F128, F32, fpextend, strict_fpextend, "f_ext", "f128", 0xbb>;
defm F128_PROMOTE_F64 : StrictConv<F128, F64, fpextend, strict_fpextend, "f_ext", "f128", 0xbb>;
defm F128_PROMOTE_F80 : StrictConv<F128, F80, fpextend, strict_fpextend, "f_ext", "f128", 0xbb>;

defm F32_R_DEMOTE_F64   : StrictConv<F32,  F64,  fpround, strict_fpround, "trunc", "f32", 0xb6>;
defm F32_R_DEMOTE_F80   : StrictConv<F32,  F80,  fpround, strict_fpround, "trunc", "f32", 0xb6>;
defm F32_R_DEMOTE_F128  : StrictConv<F32,  F128, fpround, strict_fpround, "trunc", "f32", 0xb6>;
defm F64_R_DEMOTE_F80   : StrictConv<F64,  F80,  fpround, strict_fpround, "trunc", "f64", 0xb6>;
defm F64_R_DEMOTE_F128  : StrictConv<F64,  F128, fpround, strict_fpround, "trunc", "f64", 0xb6>;
defm F80_R_DEMOTE_F128  : StrictConv<F80,  F128, fpround, strict_fpround, "trunc", "f80", 0xb6>;

//===----------------------------------------------------------------------===//
// Integer-FP bit casts
//===----------------------------------------------------------------------===//

class IntFPBitCast
  < LLIRRegClass oc
  , LLIRRegClass ic
  , string type
  , bits<32> op
  >
  : I
    < (outs oc:$dst)
    , (ins ic:$src)
    , [(set oc:$dst, (bitconvert ic:$src))]
    , !strconcat("bit_cast.", !strconcat(type, "\t$dst, $src"))
    , op
    >
{
}

def I32_BITCAST_F32   : IntFPBitCast<I32, F32, "i32", 0xbc>;
def F32_BITCAST_I32   : IntFPBitCast<F32, I32, "f32", 0xbe>;
def I64_BITCAST_F64   : IntFPBitCast<I64, F64, "i64", 0xbd>;
def F64_BITCAST_I64   : IntFPBitCast<F64, I64, "f64", 0xbf>;
def I128_BITCAST_F128 : IntFPBitCast<I128, F128, "i128", 0xbf>;
def F128_BITCAST_I128 : IntFPBitCast<F128, I128, "f128", 0xbf>;
