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


multiclass BinaryInt<SDNode node, string name, bits<32> b32, bits<32> b64>
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (node I32:$lhs, I32:$rhs))]
    , !strconcat(name, ".i32 \t$dst, $lhs, $rhs")
    , b32
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I64:$lhs, I64:$rhs)
    , [(set I64:$dst, (node I64:$lhs, I64:$rhs))]
    , !strconcat(name, ".i64 \t$dst, $lhs, $rhs")
    , b64
    >;
}

multiclass CommBinaryInt<SDNode node, string name, bits<32> b32, bits<32> b64>
{
  let isCommutable = 1 in
  defm "" : BinaryInt<node, name, b32, b64>;
}

multiclass CmpInt<CondCode cond, string name, bits<32> b32, bits<32> b64>
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (setcc I32:$lhs, I32:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i32 \t$dst, $lhs, $rhs"))
    , b32
    >;

  def _I64 : I
    < (outs I32:$dst)
    , (ins I64:$lhs, I64:$rhs)
    , [(set I32:$dst, (setcc I64:$lhs, I64:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i64 \t$dst, $lhs, $rhs"))
    , b64
    >;
}

multiclass CommCmpInt<SDNode node, string name, bits<32> b32, bits<32> b64>
{
  let isCommutable = 1 in
  defm "" : CmpInt<node, name, b32, b64>;
}

multiclass TernaryInt
  < SDNode node
  , string name
  , bits<32> b32
  , bits<32> b64
  >
{
  def _I32 : I
    < (outs I32:$dst)
    , (ins I32:$op, I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (node I32:$op, I32:$lhs, I32:$rhs))]
    , !strconcat(name, ".i32 \t$dst, $op, $lhs, $rhs")
    , b32
    >;

  def _I64 : I
    < (outs I64:$dst)
    , (ins I32:$op, I64:$lhs, I64:$rhs)
    , [(set I64:$dst, (node I32:$op, I64:$lhs, I64:$rhs))]
    , !strconcat(name, ".i64 \t$dst, $op, $lhs, $rhs")
    , b64
    >;
}


defm ADD : CommBinaryInt<add, "add",  0x90, 0x91>;
defm XOR : CommBinaryInt<xor, "xor",  0x92, 0x93>;
defm OR  : CommBinaryInt<or,  "or",   0x94, 0x95>;
defm AND : CommBinaryInt<and, "and",  0x96, 0x97>;

defm SUB : BinaryInt<sub, "sub", 0xA0, 0xA1>;
defm SRL : BinaryInt<srl, "srl", 0xA2, 0xA3>;
defm SHL : BinaryInt<shl, "shl", 0xA4, 0xA5>;
defm SRA : BinaryInt<sra, "sra", 0xA6, 0xA7>;

defm EQ : CommCmpInt<SETEQ, "eq", 0x46, 0x51>;
defm NE : CommCmpInt<SETNE, "ne", 0x47, 0x52>;

defm LT_S : CmpInt<SETLT,  "lt_s", 0x48, 0x53>;
defm LT_U : CmpInt<SETULT, "lt_u", 0x49, 0x54>;
defm GT_S : CmpInt<SETGT,  "gt_s", 0x4a, 0x55>;
defm GT_U : CmpInt<SETUGT, "gt_u", 0x4b, 0x56>;
defm LE_S : CmpInt<SETLE,  "le_s", 0x4c, 0x57>;
defm LE_U : CmpInt<SETULE, "le_u", 0x4d, 0x58>;
defm GE_S : CmpInt<SETGE,  "ge_s", 0x4e, 0x59>;
defm GE_U : CmpInt<SETUGE, "ge_u", 0x4f, 0x5a>;

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

