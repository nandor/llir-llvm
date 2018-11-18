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
// Basic register types.
//===----------------------------------------------------------------------===//

def SP : GenMReg<"%sp">;
def VA : GenMReg<"%va">;

def I32_0 : GenMReg<"%i32.0">;
def I64_0 : GenMReg<"%i64.0">;
def F32_0 : GenMReg<"%f32.0">;
def F64_0 : GenMReg<"%f64.0">;

// Virtual integer registers, can be used in all places.
def I32 : GenMRegClass<[i32], 32, (add I32_0)>;
def I64 : GenMRegClass<[i64], 64, (add I64_0)>;
def F32 : GenMRegClass<[f32], 32, (add F32_0)>;
def F64 : GenMRegClass<[f64], 64, (add F64_0)>;

// Virtual registers + special ones, can only be used as operands.
def G32 : GenMRegClass<[i32], 32, (add I32_0)>;
def G64 : GenMRegClass<[i64], 64, (add I64_0, SP, VA)>;

