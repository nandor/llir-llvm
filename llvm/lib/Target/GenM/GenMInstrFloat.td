// GenMInstrFloat.td-GenM Float codegen --------------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// GenM Float operand code-gen constructs.
///
//===----------------------------------------------------------------------===//

multiclass UnaryFloat
  < SDNode node
  , string name
  , bits<32> b32
  , bits<64> b64
  >
{
  def _F32 : I
    < (outs F32:$dst)
    , (ins F32:$src)
    , [(set F32:$dst, (node F32:$src))]
    , !strconcat(name, ".f32\t$dst, $src")
    , b32
    >;

  def _F64 : I
    < (outs F64:$dst)
    , (ins F64:$src)
    , [(set F64:$dst, (node F64:$src))]
    , !strconcat(name, ".f64\t$dst, $src")
    , b32
    >;
}

multiclass BinaryFloat
  < SDNode node
  , string name
  , bits<32> b32
  , bits<32> b64
  >
{
  def _F32 : I
    < (outs F32:$dst)
    , (ins F32:$lhs, F32:$rhs)
    , [(set F32:$dst, (node F32:$lhs, F32:$rhs))]
    , !strconcat(name, ".f32\t$dst, $lhs, $rhs")
    , b32
    >;

  def _F64 : I
    < (outs F64:$dst)
    , (ins F64:$lhs, F64:$rhs)
    , [(set F64:$dst, (node F64:$lhs, F64:$rhs))]
    , !strconcat(name, ".f64\t$dst, $lhs, $rhs")
    , b64
    >;
}

multiclass CommBinaryFloat<SDNode node, string name, bits<32> b32, bits<32> b64>
{
  let isCommutable = 1 in
  defm "" : BinaryFloat<node, name, b32, b64>;
}

multiclass CmpFloat
  < CondCode cond
  , string name
  , bits<32> enc
  >
{
  def _F32 : I
    < (outs I32:$dst)
    , (ins F32:$lhs, F32:$rhs)
    , [(set I32:$dst, (setcc F32:$lhs, F32:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i32\t$dst, $lhs, $rhs"))
    , enc
    >;

  def _F64 : I
    < (outs I32:$dst)
    , (ins F64:$lhs, F64:$rhs)
    , [(set I32:$dst, (setcc F64:$lhs, F64:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i32\t$dst, $lhs, $rhs"))
    , enc
    >;
}

multiclass CommCmpFloat
  < SDNode node
  , string name
  , bits<32> enc
  >
{
  let isCommutable = 1 in
  defm "" : CmpFloat<node, name, enc>;
}


multiclass TernaryFloat
  < SDNode node
  , string name
  , bits<32> b32
  , bits<32> b64
  >
{
  def _F32 : I
    < (outs F32:$dst)
    , (ins G32:$op, F32:$lhs, F32:$rhs)
    , [(set F32:$dst, (node G32:$op, F32:$lhs, F32:$rhs))]
    , !strconcat(name, ".f32\t$dst, $op, $lhs, $rhs")
    , b32
    >;

  def _F64 : I
    < (outs F64:$dst)
    , (ins G32:$op, F64:$lhs, F64:$rhs)
    , [(set F64:$dst, (node G32:$op, F64:$lhs, F64:$rhs))]
    , !strconcat(name, ".f64\t$dst, $op, $lhs, $rhs")
    , b64
    >;
}


defm FABS  : UnaryFloat<fabs,  "abs",  0xB0, 0xB1>;
defm FNEG  : UnaryFloat<fneg,  "neg",  0xB2, 0xB3>;
defm FSIN  : UnaryFloat<fsin,  "sin", 0xB2, 0xB3>;
defm FCOS  : UnaryFloat<fcos,  "cos", 0xB2, 0xB3>;
defm FSQRT : UnaryFloat<fsqrt, "sqrt", 0xB2, 0xB3>;
defm COPYSIGN : BinaryFloat<fcopysign, "copysign", 0x98, 0xa6>;

defm FADD : CommBinaryFloat<fadd, "add",  0x90, 0x91>;
defm FMUL : CommBinaryFloat<fmul, "mul",  0x90, 0x90>;
defm FSUB : BinaryFloat<fsub, "sub", 0xA0, 0xA1>;
defm FDIV : BinaryFloat<fdiv, "div", 0xA1, 0xA2>;
defm FREM : BinaryFloat<frem, "rem", 0xA3, 0xA2>;
defm FPOW : BinaryFloat<fpow, "pow", 0xA4, 0xA5>;

defm OEQ : CmpFloat<SETOEQ, "oeq", 0x5b>;
defm ONE : CmpFloat<SETONE, "one", 0x5c>;
defm OLT : CmpFloat<SETOLT, "olt", 0x5d>;
defm OLE : CmpFloat<SETOLE, "ole", 0x5f>;
defm OGT : CmpFloat<SETOGT, "ogt", 0x5e>;
defm OGE : CmpFloat<SETOGE, "oge", 0x60>;

defm UEQ : CmpFloat<SETUEQ, "ueq", 0x5b>;
defm UNE : CmpFloat<SETUNE, "une", 0x5c>;
defm ULT : CmpFloat<SETULT, "ult", 0x5d>;
defm ULE : CmpFloat<SETULE, "ule", 0x5f>;
defm UGT : CmpFloat<SETUGT, "ugt", 0x5e>;
defm UGE : CmpFloat<SETUGE, "uge", 0x60>;


defm SELECT : TernaryFloat<select, "select", 0xB0, 0xB1>;

def : Pat
  < (select (i32 (setne G32:$cond, 0)), F32:$lhs, F32:$rhs)
  , (SELECT_I32 G32:$cond, F32:$lhs, F32:$rhs)
  >;
def : Pat
  < (select (i32 (setne G32:$cond, 0)), F64:$lhs, F64:$rhs)
  , (SELECT_I64 G32:$cond, F64:$lhs, F64:$rhs)
  >;
def : Pat
  < (select (i32 (seteq G32:$cond, 0)), F32:$lhs, F32:$rhs)
  , (SELECT_I32 G32:$cond, F32:$rhs, F32:$lhs)
  >;
def : Pat
  < (select (i32 (seteq G32:$cond, 0)), F64:$lhs, F64:$rhs)
  , (SELECT_I64 G32:$cond, F64:$rhs, F64:$lhs)
  >;
