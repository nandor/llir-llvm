//===-- LLIRInstrConst.td - LLIR constants -----------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

class LLIRIConst
  < LLIRRegClass R
  , string Type
  , LLIROperand Value
  , int Opcode
  >
  : I
    < (outs R:$res)
    , (ins Value:$imm)
    , [(set R:$res, imm:$imm)]
    , !strconcat(!strconcat("mov.", Type), " \t$res, $imm")
    , Opcode
    >
{
  let isMoveImm = 1;
  let isAsCheapAsAMove = 1;
  let isReMaterializable = 1;
}

class LLIRFConst
  < LLIRRegClass R
  , string Type
  , LLIROperand Value
  , int Opcode
  >
  : I
    < (outs R:$res)
    , (ins Value:$imm)
    , [(set R:$res, fpimm:$imm)]
    , !strconcat(!strconcat("mov.", Type), " \t$res, $imm")
    , Opcode
    >
{
  let isMoveImm = 1;
  let isAsCheapAsAMove = 1;
  let isReMaterializable = 1;
}

def CONST_I8   : LLIRIConst<I8,   "i8",   i8imm_op,  0x05>;
def CONST_I16  : LLIRIConst<I16,  "i16",  i16imm_op, 0x05>;
def CONST_I32  : LLIRIConst<I32,  "i32",  i32imm_op, 0x05>;
def CONST_I64  : LLIRIConst<I64,  "i64",  i64imm_op, 0x06>;
def CONST_F32  : LLIRFConst<F32,  "f32",  f32imm_op, 0x07>;
def CONST_F64  : LLIRFConst<F64,  "f64",  f64imm_op, 0x08>;
def CONST_F80  : LLIRFConst<F80,  "f80",  f80imm_op, 0x09>;
def CONST_F128 : LLIRFConst<F128, "f128", f128imm_op, 0x09>;
