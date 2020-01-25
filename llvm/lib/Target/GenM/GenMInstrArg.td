//===-- GenMInstrArg.td - GenM arguments -------------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

class GenMArg
  < GenMRegClass R
  , string Type
  , int Opcode
  >
  : I
    < (outs R:$res)
    , (ins i32imm:$argno)
    , [(set R:$res, (GenMargument timm:$argno))]
    , !strconcat(!strconcat("arg.", Type), "\t$res, $argno")
    , Opcode
    >
{
  let hasSideEffects = 1;
  let Uses = [ARGUMENTS];
}

def ARG_I8  : GenMArg<I8,  "i8",  0x1>;
def ARG_I16 : GenMArg<I16, "i16", 0x1>;
def ARG_I32 : GenMArg<I32, "i32", 0x1>;
def ARG_I64 : GenMArg<I64, "i64", 0x2>;
def ARG_F32 : GenMArg<F32, "f32", 0x3>;
def ARG_F64 : GenMArg<F64, "f64", 0x4>;
def ARG_F80 : GenMArg<F80, "f80", 0x5>;
