// GenMInstrInteger.td-GenM Integer codegen ---------------------*- tablegen -*-
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// GenM Integer operand code-gen constructs.
///
//===----------------------------------------------------------------------===//


multiclass BinaryInt
  < SDNode node
  , string name
  , bits<32> i32Inst
  , bits<32> i64Inst
  >
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (node I32:$lhs, I32:$rhs))]
    , !strconcat(!strconcat(name, ".i32"), " \t$dst, $lhs, $rhs")
    , i32Inst
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I64:$lhs, I64:$rhs)
    , [(set I64:$dst, (node I64:$lhs, I64:$rhs))]
    , !strconcat(!strconcat(name, ".i64"), " \t$dst, $lhs, $rhs")
    , i64Inst
    >;
}

multiclass CommutativeInt
  < SDNode node
  , string name
  , bits<32> i32Inst
  , bits<32> i64Inst
  >
{
  let isCommutable = 1 in
  defm "" : BinaryInt<node, name, i32Inst, i64Inst>;
}

multiclass TernaryInt
  < SDNode node
  , string name
  , bits<32> i32Inst
  , bits<32> i64Inst
  >
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins I32:$op, I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (node I32:$op, I32:$lhs, I32:$rhs))]
    , !strconcat(!strconcat(name, ".i32"), " \t$dst, $op, $lhs, $rhs")
    , i32Inst
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I32:$op, I64:$lhs, I64:$rhs)
    , [(set I64:$dst, (node I32:$op, I64:$lhs, I64:$rhs))]
    , !strconcat(!strconcat(name, ".i64"), " \t$dst, $op, $lhs, $rhs")
    , i64Inst
    >;
}


defm ADD : CommutativeInt<add, "add",  0x90, 0x91>;
defm XOR : CommutativeInt<xor, "xor",  0x92, 0x93>;
defm OR  : CommutativeInt<or,  "or",   0x94, 0x95>;
defm AND : CommutativeInt<and, "and",  0x96, 0x97>;

defm SUB : BinaryInt<sub, "sub", 0xA0, 0xA1>;

defm SRL : BinaryInt<srl, "srl", 0xA2, 0xA3>;
defm SHL : BinaryInt<shl, "shl", 0xA4, 0xA5>;

defm SELECT : TernaryInt<select, "select", 0xB0, 0xB1>;

def : Pat
  < (select (i32 (setne I32:$cond, 0)), I32:$lhs, I32:$rhs)
  , (SELECT_I32 I32:$cond, I32:$lhs, I32:$rhs)
  >;
def : Pat
  < (select (i32 (setne I32:$cond, 0)), I64:$lhs, I64:$rhs)
  , (SELECT_I64 I32:$cond, I64:$lhs, I64:$rhs)
  >;
def : Pat
  < (select (i32 (seteq I32:$cond, 0)), I32:$lhs, I32:$rhs)
  , (SELECT_I32 I32:$cond, I32:$rhs, I32:$lhs)
  >;
def : Pat
  < (select (i32 (seteq I32:$cond, 0)), I64:$lhs, I64:$rhs)
  , (SELECT_I64 I32:$cond, I64:$rhs, I64:$lhs)
  >;

