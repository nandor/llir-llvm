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

multiclass UnaryInt
  < SDNode node
  , string name
  , bits<32> b32
  , bits<32> b64
  >
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins I32:$arg)
    , [(set I32:$dst, (node I32:$arg))]
    , !strconcat(name, ".i32\t$dst, $arg")
    , b32
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I64:$arg)
    , [(set I64:$dst, (node I64:$arg))]
    , !strconcat(name, ".i64\t$dst, $arg")
    , b64
    >;
}

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
    , (ins I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (node I32:$lhs, I32:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "32\t$dst, $lhs, $rhs")
      )
    , b32
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I64:$lhs, I64:$rhs)
    , [(set I64:$dst, (node I64:$lhs, I64:$rhs))]
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
    , (ins I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (setcc I32:$lhs, I32:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i32\t$dst, $lhs, $rhs"))
    , enc
    >;

  def _I64 : I
    < (outs I32:$dst)
    , (ins I64:$lhs, I64:$rhs)
    , [(set I32:$dst, (setcc I64:$lhs, I64:$rhs, cond))]
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
    , (ins I32:$op, I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (node I32:$op, I32:$lhs, I32:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "32\t$dst, $op, $lhs, $rhs")
      )
    , b32
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I32:$op, I64:$lhs, I64:$rhs)
    , [(set I64:$dst, (node I32:$op, I64:$lhs, I64:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "64\t$dst, $op, $lhs, $rhs")
      )
    , b64
    >;
}

multiclass OverflowInt
  < SDNode node
  , string name
  , string type
  , bits<32> b32
  , bits<32> b64
  >
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (node I32:$lhs, I32:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "\t$dst, $lhs, $rhs")
      )
    , b32
    >;

  def _I64 : I
    < (outs I32:$dst)
    , (ins I64:$lhs, I64:$rhs)
    , [(set I32:$dst, (node I64:$lhs, I64:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "\t$dst, $lhs, $rhs")
      )
    , b64
    >;
}

defm POPCNT : UnaryInt<ctpop, "popcnt", 0x90, 0x91>;
defm CLZ : UnaryInt<ctlz, "clz", 0x90, 0x91>;

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
defm NE : CommCmpInt<SETNE, "ne", 0x47>;

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

defm SADDO : OverflowInt<GenMsaddo, "saddo", "i32", 0x70, 0x70>;
defm UADDO : OverflowInt<GenMuaddo, "uaddo", "i32", 0x71, 0x71>;
defm SSUBO : OverflowInt<GenMssubo, "ssubo", "i32", 0x72, 0x72>;
defm USUBO : OverflowInt<GenMusubo, "usubo", "i32", 0x73, 0x73>;
defm SMULO : OverflowInt<GenMsmulo, "smulo", "i32", 0x74, 0x74>;
defm UMULO : OverflowInt<GenMumulo, "umulo", "i32", 0x75, 0x75>;
