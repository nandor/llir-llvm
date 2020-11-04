//===-- LLIRInstrArg.td - LLIR arguments -------------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

class LLIRArg
  < LLIRRegClass R
  , string Type
  , int Opcode
  >
  : I
    < (outs R:$res)
    , (ins i32imm:$argno)
    , [(set R:$res, (LLIRargument timm:$argno))]
    , !strconcat(!strconcat("arg.", Type), "\t$res, $argno")
    , Opcode
    >
{
  let hasSideEffects = 1;
  let Uses = [ARGUMENTS];
}

def ARG_I8   : LLIRArg<I8,   "i8",   0x1>;
def ARG_I16  : LLIRArg<I16,  "i16",  0x1>;
def ARG_I32  : LLIRArg<I32,  "i32",  0x1>;
def ARG_I64  : LLIRArg<I64,  "i64",  0x2>;
def ARG_I128 : LLIRArg<I128, "i128", 0x2>;
def ARG_F32  : LLIRArg<F32,  "f32",  0x3>;
def ARG_F64  : LLIRArg<F64,  "f64",  0x4>;
def ARG_F80  : LLIRArg<F80,  "f80",  0x5>;
def ARG_F128 : LLIRArg<F128, "f128", 0x5>;
