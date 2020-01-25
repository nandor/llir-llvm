// GenMInstrMemory.td - GenM Memory codegen support -----------*- tablegen -*-//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// GenM Memory operand code-gen constructs.
///
//===----------------------------------------------------------------------===//


// Defines a store instruction.
class GenMSt
  < GenMRegClass R
  , PatFrag Node
  , string Size
  , int Opcode
  >
  : I
    < (outs)
    , (ins I64:$addr, R:$val)
    , [(Node R:$val, I64:$addr)]
    , !strconcat(!strconcat("st.", Size), "\t[${addr}], $val")
    , Opcode
    >
{
  let mayStore = 1;
}

def ST_1_I8  : GenMSt<I8,  store,         "1", 0x30>;

def ST_1_I16 : GenMSt<I16, truncstorei8,  "1", 0x30>;
def ST_2_I16 : GenMSt<I16, store,         "2", 0x31>;

def ST_1_I32 : GenMSt<I32, truncstorei8,  "1", 0x30>;
def ST_2_I32 : GenMSt<I32, truncstorei16, "2", 0x31>;
def ST_4_I32 : GenMSt<I32, store,         "4", 0x32>;

def ST_1_I64 : GenMSt<I64, truncstorei8,  "1", 0x33>;
def ST_2_I64 : GenMSt<I64, truncstorei16, "2", 0x34>;
def ST_4_I64 : GenMSt<I64, truncstorei32, "4", 0x35>;
def ST_8_I64 : GenMSt<I64, store,         "8", 0x36>;

def ST_4_F32  : GenMSt<F32, store,  "4", 0x37>;
def ST_8_F64  : GenMSt<F64, store,  "8", 0x38>;
def ST_10_F80 : GenMSt<F80, store, "10", 0x39>;


// Defines a load instruction.
class GenMLd<GenMRegClass R, PatFrag Node, string Name, int Opcode>
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

def LD_1_I8  : GenMLd<I8,  load,        "1.u8",  0x43>;
def LD_1_U8  : GenMLd<I8,  load,        "1.i8",  0x43>;

def LD_1_X16 : GenMLd<I16, extloadi8,   "1.u16", 0x42>;
def LD_1_I16 : GenMLd<I16, sextloadi8,  "1.i16", 0x40>;
def LD_1_U16 : GenMLd<I16, zextloadi8,  "1.u16", 0x41>;
def LD_2_I16 : GenMLd<I16, load,        "2.u16", 0x43>;
def LD_2_U16 : GenMLd<I16, load,        "2.i16", 0x43>;

def LD_1_X32 : GenMLd<I32, extloadi8,   "1.u32", 0x42>;
def LD_1_I32 : GenMLd<I32, sextloadi8,  "1.i32", 0x40>;
def LD_1_U32 : GenMLd<I32, zextloadi8,  "1.u32", 0x41>;
def LD_2_X32 : GenMLd<I32, extloadi16,  "2.u32", 0x43>;
def LD_2_I32 : GenMLd<I32, sextloadi16, "2.i32", 0x43>;
def LD_2_U32 : GenMLd<I32, zextloadi16, "2.u32", 0x44>;
def LD_4_I32 : GenMLd<I32, load,        "4.i32", 0x45>;
def LD_4_U32 : GenMLd<I32, load,        "4.u32", 0x45>;

def LD_1_X64 : GenMLd<I64, extloadi8,   "1.u64", 0x46>;
def LD_1_I64 : GenMLd<I64, sextloadi8,  "1.i64", 0x46>;
def LD_1_U64 : GenMLd<I64, zextloadi8,  "1.u64", 0x4A>;
def LD_2_X64 : GenMLd<I64, extloadi16,  "2.u64", 0x47>;
def LD_2_I64 : GenMLd<I64, sextloadi16, "2.i64", 0x47>;
def LD_2_U64 : GenMLd<I64, zextloadi16, "2.u64", 0x4B>;
def LD_4_X64 : GenMLd<I64, extloadi32,  "4.u64", 0x48>;
def LD_4_I64 : GenMLd<I64, sextloadi32, "4.i64", 0x48>;
def LD_4_U64 : GenMLd<I64, zextloadi32, "4.u64", 0x4C>;
def LD_8_I64 : GenMLd<I64, load,        "8.i64", 0x49>;
def LD_8_U64 : GenMLd<I64, load,        "8.u64", 0x4D>;

def LD_4_F32  : GenMLd<F32, load,        "4.f32", 0x50>;
def LD_8_F64  : GenMLd<F64, load,        "8.f64", 0x51>;
def LD_10_F80 : GenMLd<F80, load,       "10.f80", 0x53>;

def LD_1_X32_ATOMIC : GenMLd<I32, atomic_load_8,  "1.i32", 0x4D>;
def LD_2_X32_ATOMIC : GenMLd<I32, atomic_load_16, "2.i32", 0x4D>;
def LD_4_X32_ATOMIC : GenMLd<I32, atomic_load_32, "4.i32", 0x4D>;
