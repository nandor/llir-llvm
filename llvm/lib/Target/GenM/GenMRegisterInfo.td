//===-- GenMRegisterInfo.td - GenM Register defs -----------*- tablegen -*-===//
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


def PC : GenMReg<"%pc">;
def SP : GenMReg<"%sp">;

def GPR : GenMRegClass<[i64], 64, (add PC, SP)>;
