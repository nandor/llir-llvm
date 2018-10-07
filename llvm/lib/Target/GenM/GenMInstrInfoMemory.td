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
class GenMSt<GenMRegClass R, string Name, int Opcode>
  : I
    < (outs)
    , (ins i32off_op:$off, I64:$addr, R:$val)
    , []
    , !strconcat("st.", !strconcat(Name, "\t[${addr}, ${off}], $val"))
    , Opcode
    >
{
  let mayStore = 1;
}

def ST_1_I32 : GenMSt<I32, "1.i32", 0x30>;
def ST_2_I32 : GenMSt<I32, "2.i32", 0x31>;
def ST_4_I32 : GenMSt<I32, "4.i32", 0x32>;
def ST_1_I64 : GenMSt<I64, "1.i64", 0x33>;
def ST_2_I64 : GenMSt<I64, "2.i64", 0x34>;
def ST_4_I64 : GenMSt<I64, "4.i64", 0x35>;
def ST_8_I64 : GenMSt<I64, "8.i64", 0x36>;

class StorePatNoOffset<ValueType ty, PatFrag node, I inst>
  : Pat
    < (node ty:$val, I64:$addr)
    , (inst 0, I64:$addr, ty:$val)
    >;

def : StorePatNoOffset<i32, truncstorei8,   ST_1_I32>;
def : StorePatNoOffset<i32, truncstorei16,  ST_2_I32>;
def : StorePatNoOffset<i32, store,          ST_4_I32>;
def : StorePatNoOffset<i64, truncstorei8,   ST_1_I64>;
def : StorePatNoOffset<i64, truncstorei16,  ST_2_I64>;
def : StorePatNoOffset<i64, truncstorei32,  ST_4_I64>;
def : StorePatNoOffset<i64, store,          ST_8_I64>;



// Defines a load instruction.
class GenMLd<GenMRegClass R, string Name, int Opcode>
  : I
    < (outs)
    , (ins i32off_op:$off, I64:$addr, R:$val)
    , []
    , !strconcat("ld.", !strconcat(Name, "\t[${addr}, ${off}], $val"))
    , Opcode
    >
{
  let mayLoad = 1;
}

def LD_1_I32 : GenMLd<I32, "1.i32", 0x40>;
def LD_2_I32 : GenMLd<I32, "2.i32", 0x41>;
def LD_4_I32 : GenMLd<I32, "4.i32", 0x42>;
def LD_1_U32 : GenMLd<I32, "1.u32", 0x43>;
def LD_2_U32 : GenMLd<I32, "2.u32", 0x44>;
def LD_4_U32 : GenMLd<I32, "4.u32", 0x45>;
def LD_1_I64 : GenMLd<I64, "1.i64", 0x46>;
def LD_2_I64 : GenMLd<I64, "2.i64", 0x47>;
def LD_4_I64 : GenMLd<I64, "4.i64", 0x48>;
def LD_8_I64 : GenMLd<I64, "8.i64", 0x49>;
def LD_1_U64 : GenMLd<I64, "1.u64", 0x4A>;
def LD_2_U64 : GenMLd<I64, "2.u64", 0x4B>;
def LD_4_U64 : GenMLd<I64, "4.u64", 0x4C>;
def LD_8_U64 : GenMLd<I64, "8.u64", 0x4D>;
