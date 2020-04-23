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
  < LLIRRegClass R
  , PatFrag Node
  , int Opcode
  >
  : I
    < (outs)
    , (ins I64:$addr, R:$val)
    , [(Node R:$val, I64:$addr)]
    , "st\t[${addr}], $val"
    , Opcode
    >
{
  let mayStore = 1;
}


def ST_1_I8   : LLIRSt<I8,  store, 0x30>;
def ST_2_I16  : LLIRSt<I16, store, 0x30>;
def ST_4_I32  : LLIRSt<I32, store, 0x30>;
def ST_8_I64  : LLIRSt<I64, store, 0x30>;
def ST_4_F32  : LLIRSt<F32, store, 0x30>;
def ST_8_F64  : LLIRSt<F64, store, 0x30>;
def ST_10_F80 : LLIRSt<F80, store, 0x30>;

def ST_4_I32_ATOMIC : LLIRSt<I32, atomic_store_32, 0x4D>;
def ST_8_I64_ATOMIC : LLIRSt<I64, atomic_store_64, 0x4E>;

def : Pat<(truncstorei8  I16:$r, I64:$addr), (ST_1_I8  I64:$addr, (TRUNC_I16_I8  I16:$r))>;
def : Pat<(truncstorei8  I32:$r, I64:$addr), (ST_1_I8  I64:$addr, (TRUNC_I32_I8  I32:$r))>;
def : Pat<(truncstorei8  I64:$r, I64:$addr), (ST_1_I8  I64:$addr, (TRUNC_I64_I8  I64:$r))>;
def : Pat<(truncstorei16 I32:$r, I64:$addr), (ST_2_I16 I64:$addr, (TRUNC_I32_I16 I32:$r))>;
def : Pat<(truncstorei16 I64:$r, I64:$addr), (ST_2_I16 I64:$addr, (TRUNC_I64_I16 I64:$r))>;
def : Pat<(truncstorei32 I64:$r, I64:$addr), (ST_4_I32 I64:$addr, (TRUNC_I64_I32 I64:$r))>;


// Defines a load instruction.
class LLIRLd<LLIRRegClass R, PatFrag Node, string Name, int Opcode>
  : I
    < (outs R:$dst)
    , (ins I64:$addr)
    , [(set R:$dst, (Node I64:$addr))]
    , !strconcat("ld.", !strconcat(Name, "\t$dst, [${addr}]"))
    , Opcode
    >
{
  let mayLoad = 1;
}

def LD_1_I8   : LLIRLd<I8,  load, "i8",  0x40>;
def LD_2_I16  : LLIRLd<I16, load, "i16", 0x40>;
def LD_4_I32  : LLIRLd<I32, load, "i32", 0x40>;
def LD_8_I64  : LLIRLd<I64, load, "i64", 0x40>;
def LD_4_F32  : LLIRLd<F32, load, "f32", 0x40>;
def LD_8_F64  : LLIRLd<F64, load, "f64", 0x40>;
def LD_10_F80 : LLIRLd<F80, load, "f80", 0x40>;


def : Pat<(i16 (extloadi8   I64:$addr)), (XEXT_I8_I16  (LD_1_I8  I64:$addr))>;
def : Pat<(i32 (extloadi8   I64:$addr)), (XEXT_I8_I32  (LD_1_I8  I64:$addr))>;
def : Pat<(i64 (extloadi8   I64:$addr)), (XEXT_I8_I64  (LD_1_I8  I64:$addr))>;
def : Pat<(i32 (extloadi16  I64:$addr)), (XEXT_I16_I32 (LD_2_I16 I64:$addr))>;
def : Pat<(i64 (extloadi16  I64:$addr)), (XEXT_I16_I64 (LD_2_I16 I64:$addr))>;
def : Pat<(i64 (extloadi32  I64:$addr)), (XEXT_I32_I64 (LD_4_I32 I64:$addr))>;
def : Pat<(i16 (sextloadi8  I64:$addr)), (SEXT_I8_I16  (LD_1_I8  I64:$addr))>;
def : Pat<(i32 (sextloadi8  I64:$addr)), (SEXT_I8_I32  (LD_1_I8  I64:$addr))>;
def : Pat<(i64 (sextloadi8  I64:$addr)), (SEXT_I8_I64  (LD_1_I8  I64:$addr))>;
def : Pat<(i32 (sextloadi16 I64:$addr)), (SEXT_I16_I32 (LD_2_I16 I64:$addr))>;
def : Pat<(i64 (sextloadi16 I64:$addr)), (SEXT_I16_I64 (LD_2_I16 I64:$addr))>;
def : Pat<(i64 (sextloadi32 I64:$addr)), (SEXT_I32_I64 (LD_4_I32 I64:$addr))>;
def : Pat<(i16 (zextloadi8  I64:$addr)), (ZEXT_I8_I16  (LD_1_I8  I64:$addr))>;
def : Pat<(i32 (zextloadi8  I64:$addr)), (ZEXT_I8_I32  (LD_1_I8  I64:$addr))>;
def : Pat<(i64 (zextloadi8  I64:$addr)), (ZEXT_I8_I64  (LD_1_I8  I64:$addr))>;
def : Pat<(i32 (zextloadi16 I64:$addr)), (ZEXT_I16_I32 (LD_2_I16 I64:$addr))>;
def : Pat<(i64 (zextloadi16 I64:$addr)), (ZEXT_I16_I64 (LD_2_I16 I64:$addr))>;
def : Pat<(i64 (zextloadi32 I64:$addr)), (ZEXT_I32_I64 (LD_4_I32 I64:$addr))>;

def LD_1_I8_ATOMIC  : LLIRLd<I8,  atomic_load_8,  "i8",  0x4D>;
def LD_2_I16_ATOMIC : LLIRLd<I16, atomic_load_16, "i16", 0x4D>;
def LD_4_I32_ATOMIC : LLIRLd<I32, atomic_load_32, "i32", 0x4D>;
def LD_8_I64_ATOMIC : LLIRLd<I64, atomic_load_64, "i64", 0x4D>;

def : Pat<(i32 (atomic_load_8  I64:$addr)), (XEXT_I8_I32  (LD_1_I8_ATOMIC  I64:$addr))>;
def : Pat<(i32 (atomic_load_16 I64:$addr)), (XEXT_I16_I32 (LD_2_I16_ATOMIC I64:$addr))>;
