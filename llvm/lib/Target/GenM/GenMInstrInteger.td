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
  , bits<32> b
  >
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins I32:$arg)
    , [(set I32:$dst, (node I32:$arg))]
    , !strconcat(name, ".i32\t$dst, $arg")
    , b
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I64:$arg)
    , [(set I64:$dst, (node I64:$arg))]
    , !strconcat(name, ".i64\t$dst, $arg")
    , b
    >;
}

multiclass BinaryInt
  < SDNode node
  , string name
  , string type
  , bits<32> b
  >
{
  def _I8 : I
    < (outs I8:$dst)
    , (ins I8:$lhs, I8:$rhs)
    , [(set I8:$dst, (node I8:$lhs, I8:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "8\t$dst, $lhs, $rhs")
      )
    , b
    >;

  def _I16 : I
    < (outs I16:$dst)
    , (ins I16:$lhs, I16:$rhs)
    , [(set I16:$dst, (node I16:$lhs, I16:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "16\t$dst, $lhs, $rhs")
      )
    , b
    >;

  def _I32 : I
    < (outs I32:$dst)
    , (ins I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (node I32:$lhs, I32:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "32\t$dst, $lhs, $rhs")
      )
    , b
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I64:$lhs, I64:$rhs)
    , [(set I64:$dst, (node I64:$lhs, I64:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "64\t$dst, $lhs, $rhs")
      )
    , b
    >;
}

multiclass ShiftInt
  < SDNode node
  , string name
  , string type
  , bits<32> b
  >
{
  def _I8 : I
    < (outs I8:$dst)
    , (ins I8:$lhs, I8:$rhs)
    , [(set I8:$dst, (node I8:$lhs, I8:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "8\t$dst, $lhs, $rhs")
      )
    , b
    >;

  def _I16 : I
    < (outs I16:$dst)
    , (ins I16:$lhs, I8:$rhs)
    , [(set I16:$dst, (node I16:$lhs, I8:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "16\t$dst, $lhs, $rhs")
      )
    , b
    >;

  def _I32 : I
    < (outs I32:$dst)
    , (ins I32:$lhs, I8:$rhs)
    , [(set I32:$dst, (node I32:$lhs, I8:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "32\t$dst, $lhs, $rhs")
      )
    , b
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I64:$lhs, I8:$rhs)
    , [(set I64:$dst, (node I64:$lhs, I8:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "64\t$dst, $lhs, $rhs")
      )
    , b
    >;
}

multiclass CommBinaryInt
  < SDNode node
  , string name
  , string type
  , bits<32> b
  >
{
  let isCommutable = 1 in
  defm "" : BinaryInt<node, name, type, b>;
}

multiclass CmpInt
  < CondCode cond
  , string name
  , bits<32> b
  >
{
  def _I8 : I
    < (outs I8:$dst)
    , (ins I8:$lhs, I8:$rhs)
    , [(set I8:$dst, (setcc I8:$lhs, I8:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i8\t$dst, $lhs, $rhs"))
    , b
    >;

  def _I16 : I
    < (outs I8:$dst)
    , (ins I16:$lhs, I16:$rhs)
    , [(set I8:$dst, (setcc I16:$lhs, I16:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i8\t$dst, $lhs, $rhs"))
    , b
    >;

  def _I32 : I
    < (outs I8:$dst)
    , (ins I32:$lhs, I32:$rhs)
    , [(set I8:$dst, (setcc I32:$lhs, I32:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i8\t$dst, $lhs, $rhs"))
    , b
    >;

  def _I64 : I
    < (outs I8:$dst)
    , (ins I64:$lhs, I64:$rhs)
    , [(set I8:$dst, (setcc I64:$lhs, I64:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i8\t$dst, $lhs, $rhs"))
    , b
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
  , bits<32> b
  >
{
  def _I8 : I
    < (outs I8:$dst)
    , (ins I8:$op, I8:$lhs, I8:$rhs)
    , [(set I8:$dst, (node I8:$op, I8:$lhs, I8:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "8\t$dst, $op, $lhs, $rhs")
      )
    , b
    >;

  def _I16 : I
    < (outs I16:$dst)
    , (ins I8:$op, I16:$lhs, I16:$rhs)
    , [(set I16:$dst, (node I8:$op, I16:$lhs, I16:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "16\t$dst, $op, $lhs, $rhs")
      )
    , b
    >;

  def _I32 : I
    < (outs I32:$dst)
    , (ins I8:$op, I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (node I8:$op, I32:$lhs, I32:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "32\t$dst, $op, $lhs, $rhs")
      )
    , b
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I8:$op, I64:$lhs, I64:$rhs)
    , [(set I64:$dst, (node I8:$op, I64:$lhs, I64:$rhs))]
    , !strconcat(
          !strconcat(name, "."),
          !strconcat(type, "64\t$dst, $op, $lhs, $rhs")
      )
    , b
    >;
}

multiclass OverflowInt
  < SDNode node
  , string name
  , bits<32> b
  >
{
  def _I8 : I
    < (outs I8:$dst)
    , (ins I8:$lhs, I8:$rhs)
    , [(set I8:$dst, (node I8:$lhs, I8:$rhs))]
    , !strconcat(name, ".i8\t$dst, $lhs, $rhs")
    , b
    >;

  def _I16 : I
    < (outs I8:$dst)
    , (ins I16:$lhs, I16:$rhs)
    , [(set I8:$dst, (node I16:$lhs, I16:$rhs))]
    , !strconcat(name, ".i8\t$dst, $lhs, $rhs")
    , b
    >;

  def _I32 : I
    < (outs I8:$dst)
    , (ins I32:$lhs, I32:$rhs)
    , [(set I8:$dst, (node I32:$lhs, I32:$rhs))]
    , !strconcat(name, ".i8\t$dst, $lhs, $rhs")
    , b
    >;

  def _I64 : I
    < (outs I8:$dst)
    , (ins I64:$lhs, I64:$rhs)
    , [(set I8:$dst, (node I64:$lhs, I64:$rhs))]
    , !strconcat(name, ".i8\t$dst, $lhs, $rhs")
    , b
    >;
}

defm POPCNT : UnaryInt<ctpop, "popcnt", 0x90>;
defm CLZ : UnaryInt<ctlz, "clz", 0x90>;
defm CTZ : UnaryInt<cttz, "ctz", 0x90>;

defm ADD : CommBinaryInt<add, "add",  "i", 0x90>;
defm XOR : CommBinaryInt<xor, "xor",  "i", 0x90>;
defm OR  : CommBinaryInt<or,  "or",   "i", 0x90>;
defm AND : CommBinaryInt<and, "and",  "i", 0x90>;
defm MUL : CommBinaryInt<mul, "mul",  "i", 0x90>;

defm SUB  : BinaryInt<sub,  "sub",  "i", 0xA0>;
defm SDIV : BinaryInt<sdiv, "div",  "i", 0xA1>;
defm UDIV : BinaryInt<udiv, "div",  "u", 0xA2>;
defm SREM : BinaryInt<srem, "rem",  "i", 0xA3>;
defm UREM : BinaryInt<urem, "rem",  "u", 0xA4>;

defm SRL  : ShiftInt<srl,  "srl",  "i", 0xA5>;
defm SHL  : ShiftInt<shl,  "sll",  "i", 0xA6>;
defm SRA  : ShiftInt<sra,  "sra",  "i", 0xA7>;
defm ROTL : ShiftInt<rotl, "rotl", "i", 0xA8>;

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

defm SELECT : TernaryInt<select, "select", "i", 0xB0>;


def : Pat
  < (select (i8 (setne I8:$cond, 0)), I8:$lhs, I8:$rhs)
  , (SELECT_I8 I8:$cond, I8:$lhs, I8:$rhs)
  >;
def : Pat
  < (select (i8 (setne I8:$cond, 0)), I16:$lhs, I16:$rhs)
  , (SELECT_I16 I8:$cond, I16:$lhs, I16:$rhs)
  >;
def : Pat
  < (select (i8 (setne I8:$cond, 0)), I32:$lhs, I32:$rhs)
  , (SELECT_I32 I8:$cond, I32:$lhs, I32:$rhs)
  >;
def : Pat
  < (select (i8 (setne I8:$cond, 0)), I64:$lhs, I64:$rhs)
  , (SELECT_I64 I8:$cond, I64:$lhs, I64:$rhs)
  >;

def : Pat
  < (select (i8 (seteq I8:$cond, 0)), I8:$lhs, I8:$rhs)
  , (SELECT_I8 I8:$cond, I8:$rhs, I8:$lhs)
  >;
def : Pat
  < (select (i8 (seteq I8:$cond, 0)), I16:$lhs, I16:$rhs)
  , (SELECT_I16 I8:$cond, I16:$rhs, I16:$lhs)
  >;
def : Pat
  < (select (i8 (seteq I8:$cond, 0)), I32:$lhs, I32:$rhs)
  , (SELECT_I32 I8:$cond, I32:$rhs, I32:$lhs)
  >;
def : Pat
  < (select (i8 (seteq I8:$cond, 0)), I64:$lhs, I64:$rhs)
  , (SELECT_I64 I8:$cond, I64:$rhs, I64:$lhs)
  >;

defm SADDO : OverflowInt<GenMsaddo, "saddo", 0x70>;
defm UADDO : OverflowInt<GenMuaddo, "uaddo", 0x71>;
defm SSUBO : OverflowInt<GenMssubo, "ssubo", 0x72>;
defm USUBO : OverflowInt<GenMusubo, "usubo", 0x73>;
defm SMULO : OverflowInt<GenMsmulo, "smulo", 0x74>;
defm UMULO : OverflowInt<GenMumulo, "umulo", 0x75>;
