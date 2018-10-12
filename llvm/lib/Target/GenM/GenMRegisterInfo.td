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

def PC : GenMReg<"%pc">;
def SP : GenMReg<"%sp">;
def FP : GenMReg<"%sp">;

def I32_0 : GenMReg<"%i32.0">;
def I64_0 : GenMReg<"%i64.0">;

def I32 : GenMRegClass<[i32], 32, (add I32_0)>;
def I64 : GenMRegClass<[i64], 64, (add I64_0, SP, FP)>;

