//===-- LLIRRegInfo.td - LLIR Reg defs -----------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the LLIR register file
//===----------------------------------------------------------------------===//

class LLIRReg<string n>
    : Register<n>
{
  let Namespace = "LLIR";
}

class LLIRRegClass<list<ValueType> regTypes, int alignment, dag regList>
    : RegisterClass<"LLIR", regTypes, alignment, regList>
{
}

//===----------------------------------------------------------------------===//
// Special registers.
//===----------------------------------------------------------------------===//

def SP : LLIRReg<"%sp">;
def FRAME_ADDR : LLIRReg<"%frame_addr">;

def R32 : LLIRRegClass<[i32], 32, (add SP, FRAME_ADDR)>;
def R64 : LLIRRegClass<[i64], 64, (add SP, FRAME_ADDR)>;

//===----------------------------------------------------------------------===//
// Virtual register types.
//===----------------------------------------------------------------------===//

def I8_0   : LLIRReg<"%i8.0">;
def I16_0  : LLIRReg<"%i16.0">;
def I32_0  : LLIRReg<"%i32.0">;
def I64_0  : LLIRReg<"%i64.0">;
def I128_0 : LLIRReg<"%i128.0">;
def F32_0  : LLIRReg<"%f32.0">;
def F64_0  : LLIRReg<"%f64.0">;
def F80_0  : LLIRReg<"%f80.0">;
def F128_0 : LLIRReg<"%f128.0">;

def I8   : LLIRRegClass<  [i8],   8, (add   I8_0)>;
def I16  : LLIRRegClass< [i16],  16, (add  I16_0)>;
def I32  : LLIRRegClass< [i32],  32, (add  I32_0)>;
def I64  : LLIRRegClass< [i64],  64, (add  I64_0)>;
def I128 : LLIRRegClass<[i128], 128, (add I128_0)>;
def F32  : LLIRRegClass< [f32],  32, (add  F32_0)>;
def F64  : LLIRRegClass< [f64],  64, (add  F64_0)>;
def F80  : LLIRRegClass< [f80],  80, (add  F80_0)>;
def F128 : LLIRRegClass<[f128], 128, (add F128_0)>;

//===----------------------------------------------------------------------===//
// Composite register classes.
//===----------------------------------------------------------------------===//

def GI32 : LLIRRegClass<[i32], 32, (add I32_0, SP, FRAME_ADDR)>;
def GI64 : LLIRRegClass<[i64], 64, (add I64_0, SP, FRAME_ADDR)>;
