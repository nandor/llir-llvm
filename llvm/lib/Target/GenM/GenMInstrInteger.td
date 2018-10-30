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
  , string type
  , bits<32> b32
  , bits<32> b64
  >
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins G32:$lhs, G32:$rhs)
    , [(set I32:$dst, (setcc G32:$lhs, G32:$rhs, cond))]
    , !strconcat(
        !strconcat("cmp.", !strconcat(name, ".")),
        !strconcat(type, "32\t$dst, $lhs, $rhs")
      )
    , b32
    >;

  def _I64 : I
    < (outs I32:$dst)
    , (ins G64:$lhs, G64:$rhs)
    , [(set I32:$dst, (setcc G64:$lhs, G64:$rhs, cond))]
    , !strconcat(
        !strconcat("cmp.", !strconcat(name, ".")),
        !strconcat(type, "64\t$dst, $lhs, $rhs")
      )
    , b64
    >;
}

multiclass CommCmpInt
  < SDNode node
  , string name
  , string type
  , bits<32> b32
  , bits<32> b64
  >
{
  let isCommutable = 1 in
  defm "" : CmpInt<node, name, type, b32, b64>;
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
defm SHL  : BinaryInt<shl,  "shl",  "i", 0xA6, 0xA8>;
defm SRA  : BinaryInt<sra,  "sra",  "i", 0xA7, 0xAA>;
defm ROTL : BinaryInt<rotl, "rotl", "i", 0xA8, 0xAB>;

defm EQ : CommCmpInt<SETEQ, "eq", "i", 0x46, 0x51>;
defm NE : CommCmpInt<SETNE, "neq", "i", 0x47, 0x52>;

defm LT_S : CmpInt<SETLT,  "lt", "i", 0x48, 0x53>;
defm LT_U : CmpInt<SETULT, "lt", "u", 0x49, 0x54>;
defm GT_S : CmpInt<SETGT,  "gt", "i", 0x4a, 0x55>;
defm GT_U : CmpInt<SETUGT, "gt", "u", 0x4b, 0x56>;
defm LE_S : CmpInt<SETLE,  "le", "i", 0x4c, 0x57>;
defm LE_U : CmpInt<SETULE, "le", "u", 0x4d, 0x58>;
defm GE_S : CmpInt<SETGE,  "ge", "i", 0x4e, 0x59>;
defm GE_U : CmpInt<SETUGE, "ge", "u", 0x4f, 0x5a>;

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

