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
  , string type
  , bits<32> b32
  , bits<32> b64
  >
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins G32:$lhs, G32:$rhs)
    , [(set I32:$dst, (node G32:$lhs, G32:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "32\t$dst, $lhs, $rhs")
      )
    , b32
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins G64:$lhs, G64:$rhs)
    , [(set I64:$dst, (node G64:$lhs, G64:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "64\t$dst, $lhs, $rhs")
      )
    , b64
    >;
}

multiclass CommBinaryInt
  < SDNode node
  , string name
  , string type
  , bits<32> b32
  , bits<32> b64
  >
{
  let isCommutable = 1 in
  defm "" : BinaryInt<node, name, type, b32, b64>;
}

multiclass CmpInt
  < CondCode cond
  , string name
  , bits<32> enc
  >
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins G32:$lhs, G32:$rhs)
    , [(set I32:$dst, (setcc G32:$lhs, G32:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i32\t$dst, $lhs, $rhs"))
    , enc
    >;

  def _I64 : I
    < (outs I32:$dst)
    , (ins G64:$lhs, G64:$rhs)
    , [(set I32:$dst, (setcc G64:$lhs, G64:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i32\t$dst, $lhs, $rhs"))
    , enc
    >;
}

multiclass CommCmpInt
  < SDNode node
  , string name
  , bits<32> enc
  >
{
  let isCommutable = 1 in
  defm "" : CmpInt<node, name, enc>;
}

multiclass TernaryInt
  < SDNode node
  , string name
  , string type
  , bits<32> b32
  , bits<32> b64
  >
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins G32:$op, G32:$lhs, G32:$rhs)
    , [(set I32:$dst, (node G32:$op, G32:$lhs, G32:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "32\t$dst, $op, $lhs, $rhs")
      )
    , b32
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins G32:$op, G64:$lhs, G64:$rhs)
    , [(set I64:$dst, (node G32:$op, G64:$lhs, G64:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "64\t$dst, $op, $lhs, $rhs")
      )
    , b64
    >;
}


defm ADD : CommBinaryInt<add, "add",  "i", 0x90, 0x91>;
defm XOR : CommBinaryInt<xor, "xor",  "i", 0x90, 0x93>;
defm OR  : CommBinaryInt<or,  "or",   "i", 0x90, 0x95>;
defm AND : CommBinaryInt<and, "and",  "i", 0x90, 0x97>;
defm MUL : CommBinaryInt<mul, "mul",  "i", 0x90, 0x90>;

defm SUB  : BinaryInt<sub,  "sub",  "i", 0xA0, 0xA1>;
defm SDIV : BinaryInt<sdiv, "div",  "i", 0xA1, 0xA2>;
defm UDIV : BinaryInt<udiv, "div",  "u", 0xA2, 0xA4>;
defm SREM : BinaryInt<srem, "rem",  "i", 0xA3, 0xA2>;
defm UREM : BinaryInt<urem, "rem",  "u", 0xA4, 0xA4>;
defm SRL  : BinaryInt<srl,  "srl",  "i", 0xA5, 0xA6>;
defm SHL  : BinaryInt<shl,  "sll",  "i", 0xA6, 0xA8>;
defm SRA  : BinaryInt<sra,  "sra",  "i", 0xA7, 0xAA>;
defm ROTL : BinaryInt<rotl, "rotl", "i", 0xA8, 0xAB>;

defm EQ : CommCmpInt<SETEQ, "eq",  0x46>;
defm NE : CommCmpInt<SETNE, "neq", 0x47>;

defm LT_S : CmpInt<SETLT,  "lt",  0x48>;
defm LT_U : CmpInt<SETULT, "ult", 0x49>;
defm GT_S : CmpInt<SETGT,  "gt",  0x4a>;
defm GT_U : CmpInt<SETUGT, "ugt", 0x4b>;
defm LE_S : CmpInt<SETLE,  "le",  0x4c>;
defm LE_U : CmpInt<SETULE, "ule", 0x4d>;
defm GE_S : CmpInt<SETGE,  "ge",  0x4e>;
defm GE_U : CmpInt<SETUGE, "uge", 0x4f>;

defm SELECT : TernaryInt<select, "select", "i", 0xB0, 0xB1>;

def : Pat
  < (select (i32 (setne G32:$cond, 0)), G32:$lhs, G32:$rhs)
  , (SELECT_I32 G32:$cond, G32:$lhs, G32:$rhs)
  >;
def : Pat
  < (select (i32 (setne G32:$cond, 0)), G64:$lhs, G64:$rhs)
  , (SELECT_I64 G32:$cond, G64:$lhs, G64:$rhs)
  >;
def : Pat
  < (select (i32 (seteq G32:$cond, 0)), G32:$lhs, G32:$rhs)
  , (SELECT_I32 G32:$cond, G32:$rhs, G32:$lhs)
  >;
def : Pat
  < (select (i32 (seteq G32:$cond, 0)), G64:$lhs, G64:$rhs)
  , (SELECT_I64 G32:$cond, G64:$rhs, G64:$lhs)
  >;

