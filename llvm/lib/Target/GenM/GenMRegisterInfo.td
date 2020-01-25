//===-- GenMRegInfo.td - GenM Reg defs -----------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the GenM register file
//===----------------------------------------------------------------------===//

class GenMReg<string n>
    : Register<n>
{
  let Namespace = "GenM";
}

class GenMRegClass<list<ValueType> regTypes, int alignment, dag regList>
    : RegisterClass<"GenM", regTypes, alignment, regList>
{
}

//===----------------------------------------------------------------------===//
// Special registers.
//===----------------------------------------------------------------------===//

def RSP : GenMReg<"%rsp">;
def FRAME_ADDR : GenMReg<"%frame_addr">;

def R64 : GenMRegClass<[i64], 64, (add RSP, FRAME_ADDR)>;
def ARGUMENTS : GenMReg<"ARGUMENTS">;

//===----------------------------------------------------------------------===//
// Virtual register types.
//===----------------------------------------------------------------------===//

def I8_0 : GenMReg<"%i8.0">;
def I16_0 : GenMReg<"%i16.0">;
def I32_0 : GenMReg<"%i32.0">;
def I64_0 : GenMReg<"%i64.0">;
def F32_0 : GenMReg<"%f32.0">;
def F64_0 : GenMReg<"%f64.0">;
def F80_0 : GenMReg<"%f80.0">;

def I8 : GenMRegClass<[i8], 8, (add I8_0)>;
def I16 : GenMRegClass<[i16], 16, (add I16_0)>;
def I32 : GenMRegClass<[i32], 32, (add I32_0)>;
def I64 : GenMRegClass<[i64], 64, (add I64_0)>;
def F32 : GenMRegClass<[f32], 32, (add F32_0)>;
def F64 : GenMRegClass<[f64], 64, (add F64_0)>;
def F80 : GenMRegClass<[f80], 80, (add F80_0)>;

//===----------------------------------------------------------------------===//
// Composite register classes.
//===----------------------------------------------------------------------===//

def GI64 : GenMRegClass<[i64], 64, (add I64_0, RSP, FRAME_ADDR)>;
