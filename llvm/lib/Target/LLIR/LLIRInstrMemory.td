// LLIRInstrMemory.td - LLIR Memory codegen support -----------*- tablegen -*-//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// LLIR Memory operand code-gen constructs.
///
//===----------------------------------------------------------------------===//

// Defines a store instruction.
class LLIRSt
  < LLIRRegClass ValTy
  , LLIRRegClass AddrTy
  , PatFrag Node
  , int Opcode
  >
  : I
    < (outs)
    , (ins AddrTy:$addr, ValTy:$val)
    , [(Node ValTy:$val, AddrTy:$addr)]
    , "st\t[${addr}], $val"
    , Opcode
    >
{
  let mayStore = 1;
}

// Defines a load instruction.
class LLIRLd
  < LLIRRegClass DstTy
  , LLIRRegClass AddrTy
  , PatFrag Node
  , string Name
  , int Opcode
  >
  : I
    < (outs DstTy:$dst)
    , (ins AddrTy:$addr)
    , [(set DstTy:$dst, (Node AddrTy:$addr))]
    , !strconcat("ld.", !strconcat(Name, "\t$dst, [${addr}]"))
    , Opcode
    >
{
  let mayLoad = 1;
}

// Instructions for a 32-bit target.
let Predicates = [Is32Bit] in {

def ST_1_I8_I32         : LLIRSt<I8,   I32, store, 0x30>;
def ST_2_I16_I32        : LLIRSt<I16,  I32, store, 0x30>;
def ST_4_I32_I32        : LLIRSt<I32,  I32, store, 0x30>;
def ST_8_I64_I32        : LLIRSt<I64,  I32, store, 0x30>;
def ST_4_F32_I32        : LLIRSt<F32,  I32, store, 0x30>;
def ST_8_F64_I32        : LLIRSt<F64,  I32, store, 0x30>;
def ST_16_I128_I32      : LLIRSt<I128, I32, store, 0x30>;
def ST_10_F80_I32       : LLIRSt<F80,  I32, store, 0x30>;
def ST_16_F128_I32      : LLIRSt<F128, I32, store, 0x30>;
def LD_1_I8_I32         : LLIRLd<I8,   I32, load, "i8",   0x40>;
def LD_2_I16_I32        : LLIRLd<I16,  I32, load, "i16",  0x40>;
def LD_4_I32_I32        : LLIRLd<I32,  I32, load, "i32",  0x40>;
def LD_8_I64_I32        : LLIRLd<I64,  I32, load, "i64",  0x40>;
def LD_16_I128_I32      : LLIRLd<I128, I32, load, "i128", 0x40>;
def LD_4_F32_I32        : LLIRLd<F32,  I32, load, "f32",  0x40>;
def LD_8_F64_I32        : LLIRLd<F64,  I32, load, "f64",  0x40>;
def LD_10_F80_I32       : LLIRLd<F80,  I32, load, "f80",  0x40>;
def LD_16_F128_I32      : LLIRLd<F128, I32, load, "f128", 0x40>;
def ATOMIC_LD_1_I8_I32  : LLIRLd<I8,   I32, atomic_load_8,  "i8",  0x4D>;
def ATOMIC_LD_2_I16_I32 : LLIRLd<I16,  I32, atomic_load_16, "i16", 0x4D>;
def ATOMIC_LD_4_I32_I32 : LLIRLd<I32,  I32, atomic_load_32, "i32", 0x4D>;
def ATOMIC_LD_8_I64_I32 : LLIRLd<I64,  I32, atomic_load_64, "i64", 0x4D>;

def : Pat<(truncstorei8  I16:$r,  I32:$addr), (ST_1_I8_I32  I32:$addr, (TRUNC_I16_I8   I16:$r))>;
def : Pat<(truncstorei8  I32:$r,  I32:$addr), (ST_1_I8_I32  I32:$addr, (TRUNC_I32_I8   I32:$r))>;
def : Pat<(truncstorei8  I64:$r,  I32:$addr), (ST_1_I8_I32  I32:$addr, (TRUNC_I64_I8   I64:$r))>;
def : Pat<(truncstorei8  I128:$r, I32:$addr), (ST_1_I8_I32  I32:$addr, (TRUNC_I128_I8  I128:$r))>;
def : Pat<(truncstorei16 I32:$r,  I32:$addr), (ST_2_I16_I32 I32:$addr, (TRUNC_I32_I16  I32:$r))>;
def : Pat<(truncstorei16 I64:$r,  I32:$addr), (ST_2_I16_I32 I32:$addr, (TRUNC_I64_I16  I64:$r))>;
def : Pat<(truncstorei16 I128:$r, I32:$addr), (ST_2_I16_I32 I32:$addr, (TRUNC_I128_I16 I128:$r))>;
def : Pat<(truncstorei32 I64:$r,  I32:$addr), (ST_4_I32_I32 I32:$addr, (TRUNC_I64_I32  I64:$r))>;
def : Pat<(truncstorei32 I128:$r, I32:$addr), (ST_4_I32_I32 I32:$addr, (TRUNC_I128_I32 I128:$r))>;
def : Pat<(truncstorei64 I128:$r, I32:$addr), (ST_8_I64_I32 I32:$addr, (TRUNC_I128_I64 I128:$r))>;

def : Pat<(i16  (extloadi8    I32:$addr)), (XEXT_I8_I16   (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i32  (extloadi8    I32:$addr)), (XEXT_I8_I32   (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i64  (extloadi8    I32:$addr)), (XEXT_I8_I64   (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i128 (extloadi8    I32:$addr)), (XEXT_I8_I128  (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i32  (extloadi16   I32:$addr)), (XEXT_I16_I32  (LD_2_I16_I32  I32:$addr))>;
def : Pat<(i64  (extloadi16   I32:$addr)), (XEXT_I16_I64  (LD_2_I16_I32  I32:$addr))>;
def : Pat<(i128 (extloadi16   I32:$addr)), (XEXT_I16_I128 (LD_2_I16_I32  I32:$addr))>;
def : Pat<(i64  (extloadi32   I32:$addr)), (XEXT_I32_I64  (LD_4_I32_I32  I32:$addr))>;
def : Pat<(i128 (extloadi32   I32:$addr)), (XEXT_I32_I128 (LD_4_I32_I32  I32:$addr))>;
def : Pat<(i128 (extloadi64   I32:$addr)), (XEXT_I64_I128 (LD_8_I64_I32  I32:$addr))>;
def : Pat<(i16  (sextloadi8   I32:$addr)), (SEXT_I8_I16   (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i32  (sextloadi8   I32:$addr)), (SEXT_I8_I32   (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i64  (sextloadi8   I32:$addr)), (SEXT_I8_I64   (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i128 (sextloadi8   I32:$addr)), (SEXT_I8_I128  (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i32  (sextloadi16  I32:$addr)), (SEXT_I16_I32  (LD_2_I16_I32  I32:$addr))>;
def : Pat<(i64  (sextloadi16  I32:$addr)), (SEXT_I16_I64  (LD_2_I16_I32  I32:$addr))>;
def : Pat<(i128 (sextloadi16  I32:$addr)), (SEXT_I16_I128 (LD_2_I16_I32  I32:$addr))>;
def : Pat<(i64  (sextloadi32  I32:$addr)), (SEXT_I32_I64  (LD_4_I32_I32  I32:$addr))>;
def : Pat<(i128 (sextloadi32  I32:$addr)), (SEXT_I32_I128 (LD_4_I32_I32  I32:$addr))>;
def : Pat<(i128 (sextloadi64  I32:$addr)), (SEXT_I64_I128 (LD_8_I64_I32  I32:$addr))>;
def : Pat<(i16  (zextloadi8   I32:$addr)), (ZEXT_I8_I16   (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i32  (zextloadi8   I32:$addr)), (ZEXT_I8_I32   (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i64  (zextloadi8   I32:$addr)), (ZEXT_I8_I64   (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i128 (zextloadi8   I32:$addr)), (ZEXT_I8_I128  (LD_1_I8_I32   I32:$addr))>;
def : Pat<(i32  (zextloadi16  I32:$addr)), (ZEXT_I16_I32  (LD_2_I16_I32  I32:$addr))>;
def : Pat<(i64  (zextloadi16  I32:$addr)), (ZEXT_I16_I64  (LD_2_I16_I32  I32:$addr))>;
def : Pat<(i128 (zextloadi16  I32:$addr)), (ZEXT_I16_I128 (LD_2_I16_I32  I32:$addr))>;
def : Pat<(i64  (zextloadi32  I32:$addr)), (ZEXT_I32_I64  (LD_4_I32_I32  I32:$addr))>;
def : Pat<(i128 (zextloadi32  I32:$addr)), (ZEXT_I32_I128 (LD_4_I32_I32  I32:$addr))>;
def : Pat<(i128 (zextloadi64  I32:$addr)), (ZEXT_I64_I128 (LD_8_I64_I32  I32:$addr))>;

def : Pat<(i32 (atomic_load_8  I32:$addr)), (XEXT_I8_I32  (ATOMIC_LD_1_I8_I32  I32:$addr))>;
def : Pat<(i32 (atomic_load_16 I32:$addr)), (XEXT_I16_I32 (ATOMIC_LD_2_I16_I32 I32:$addr))>;

}

// Instructions for a 64-bit target.
let Predicates = [Is64Bit] in {

def ST_1_I8_I64         : LLIRSt<I8,   I64, store, 0x30>;
def ST_2_I16_I64        : LLIRSt<I16,  I64, store, 0x30>;
def ST_4_I32_I64        : LLIRSt<I32,  I64, store, 0x30>;
def ST_8_I64_I64        : LLIRSt<I64,  I64, store, 0x30>;
def ST_4_F32_I64        : LLIRSt<F32,  I64, store, 0x30>;
def ST_8_F64_I64        : LLIRSt<F64,  I64, store, 0x30>;
def ST_16_I128_I64      : LLIRSt<I128, I64, store, 0x30>;
def ST_10_F80_I64       : LLIRSt<F80,  I64, store, 0x30>;
def ST_16_F128_I64      : LLIRSt<F128, I64, store, 0x30>;
def LD_1_I8_I64         : LLIRLd<I8,   I64, load, "i8",   0x40>;
def LD_2_I16_I64        : LLIRLd<I16,  I64, load, "i16",  0x40>;
def LD_4_I32_I64        : LLIRLd<I32,  I64, load, "i32",  0x40>;
def LD_8_I64_I64        : LLIRLd<I64,  I64, load, "i64",  0x40>;
def LD_16_I128_I64      : LLIRLd<I128, I64, load, "i128", 0x40>;
def LD_4_F32_I64        : LLIRLd<F32,  I64, load, "f32",  0x40>;
def LD_8_F64_I64        : LLIRLd<F64,  I64, load, "f64",  0x40>;
def LD_10_F80_I64       : LLIRLd<F80,  I64, load, "f80",  0x40>;
def LD_16_F128_I64      : LLIRLd<F128, I64, load, "f128", 0x40>;
def ATOMIC_LD_1_I8_I64  : LLIRLd<I8,   I64, atomic_load_8,  "i8",  0x4D>;
def ATOMIC_LD_2_I16_I64 : LLIRLd<I16,  I64, atomic_load_16, "i16", 0x4D>;
def ATOMIC_LD_4_I32_I64 : LLIRLd<I32,  I64, atomic_load_32, "i32", 0x4D>;
def ATOMIC_LD_8_I64_I64 : LLIRLd<I64,  I64, atomic_load_64, "i64", 0x4D>;

def : Pat<(truncstorei8  I16:$r,  I64:$addr), (ST_1_I8_I64  I64:$addr, (TRUNC_I16_I8   I16:$r))>;
def : Pat<(truncstorei8  I32:$r,  I64:$addr), (ST_1_I8_I64  I64:$addr, (TRUNC_I32_I8   I32:$r))>;
def : Pat<(truncstorei8  I64:$r,  I64:$addr), (ST_1_I8_I64  I64:$addr, (TRUNC_I64_I8   I64:$r))>;
def : Pat<(truncstorei8  I128:$r, I64:$addr), (ST_1_I8_I64  I64:$addr, (TRUNC_I128_I8  I128:$r))>;
def : Pat<(truncstorei16 I32:$r,  I64:$addr), (ST_2_I16_I64 I64:$addr, (TRUNC_I32_I16  I32:$r))>;
def : Pat<(truncstorei16 I64:$r,  I64:$addr), (ST_2_I16_I64 I64:$addr, (TRUNC_I64_I16  I64:$r))>;
def : Pat<(truncstorei16 I128:$r, I64:$addr), (ST_2_I16_I64 I64:$addr, (TRUNC_I128_I16 I128:$r))>;
def : Pat<(truncstorei32 I64:$r,  I64:$addr), (ST_4_I32_I64 I64:$addr, (TRUNC_I64_I32  I64:$r))>;
def : Pat<(truncstorei32 I128:$r, I64:$addr), (ST_4_I32_I64 I64:$addr, (TRUNC_I128_I32 I128:$r))>;
def : Pat<(truncstorei64 I128:$r, I64:$addr), (ST_8_I64_I64 I64:$addr, (TRUNC_I128_I64 I128:$r))>;

def : Pat<(i16  (extloadi8    I64:$addr)), (XEXT_I8_I16   (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i32  (extloadi8    I64:$addr)), (XEXT_I8_I32   (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i64  (extloadi8    I64:$addr)), (XEXT_I8_I64   (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i128 (extloadi8    I64:$addr)), (XEXT_I8_I128  (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i32  (extloadi16   I64:$addr)), (XEXT_I16_I32  (LD_2_I16_I64  I64:$addr))>;
def : Pat<(i64  (extloadi16   I64:$addr)), (XEXT_I16_I64  (LD_2_I16_I64  I64:$addr))>;
def : Pat<(i128 (extloadi16   I64:$addr)), (XEXT_I16_I128 (LD_2_I16_I64  I64:$addr))>;
def : Pat<(i64  (extloadi32   I64:$addr)), (XEXT_I32_I64  (LD_4_I32_I64  I64:$addr))>;
def : Pat<(i128 (extloadi32   I64:$addr)), (XEXT_I32_I128 (LD_4_I32_I64  I64:$addr))>;
def : Pat<(i128 (extloadi64   I64:$addr)), (XEXT_I64_I128 (LD_8_I64_I64  I64:$addr))>;
def : Pat<(i16  (sextloadi8   I64:$addr)), (SEXT_I8_I16   (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i32  (sextloadi8   I64:$addr)), (SEXT_I8_I32   (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i64  (sextloadi8   I64:$addr)), (SEXT_I8_I64   (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i128 (sextloadi8   I64:$addr)), (SEXT_I8_I128  (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i32  (sextloadi16  I64:$addr)), (SEXT_I16_I32  (LD_2_I16_I64  I64:$addr))>;
def : Pat<(i64  (sextloadi16  I64:$addr)), (SEXT_I16_I64  (LD_2_I16_I64  I64:$addr))>;
def : Pat<(i128 (sextloadi16  I64:$addr)), (SEXT_I16_I128 (LD_2_I16_I64  I64:$addr))>;
def : Pat<(i64  (sextloadi32  I64:$addr)), (SEXT_I32_I64  (LD_4_I32_I64  I64:$addr))>;
def : Pat<(i128 (sextloadi32  I64:$addr)), (SEXT_I32_I128 (LD_4_I32_I64  I64:$addr))>;
def : Pat<(i128 (sextloadi64  I64:$addr)), (SEXT_I64_I128 (LD_8_I64_I64  I64:$addr))>;
def : Pat<(i16  (zextloadi8   I64:$addr)), (ZEXT_I8_I16   (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i32  (zextloadi8   I64:$addr)), (ZEXT_I8_I32   (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i64  (zextloadi8   I64:$addr)), (ZEXT_I8_I64   (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i128 (zextloadi8   I64:$addr)), (ZEXT_I8_I128  (LD_1_I8_I64   I64:$addr))>;
def : Pat<(i32  (zextloadi16  I64:$addr)), (ZEXT_I16_I32  (LD_2_I16_I64  I64:$addr))>;
def : Pat<(i64  (zextloadi16  I64:$addr)), (ZEXT_I16_I64  (LD_2_I16_I64  I64:$addr))>;
def : Pat<(i128 (zextloadi16  I64:$addr)), (ZEXT_I16_I128 (LD_2_I16_I64  I64:$addr))>;
def : Pat<(i64  (zextloadi32  I64:$addr)), (ZEXT_I32_I64  (LD_4_I32_I64  I64:$addr))>;
def : Pat<(i128 (zextloadi32  I64:$addr)), (ZEXT_I32_I128 (LD_4_I32_I64  I64:$addr))>;
def : Pat<(i128 (zextloadi64  I64:$addr)), (ZEXT_I64_I128 (LD_8_I64_I64  I64:$addr))>;

def : Pat<(i32 (atomic_load_8  I64:$addr)), (XEXT_I8_I32  (ATOMIC_LD_1_I8_I64  I64:$addr))>;
def : Pat<(i32 (atomic_load_16 I64:$addr)), (XEXT_I16_I32 (ATOMIC_LD_2_I16_I64 I64:$addr))>;

}

