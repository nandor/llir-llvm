// LLIRInstrFloat.td-LLIR Float codegen --------------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// LLIR Float operand code-gen constructs.
///
//===----------------------------------------------------------------------===//

multiclass UnaryFloat
  < SDNode node
  , string name
  , bits<32> Op
  >
{
  def _F32 : I
    < (outs F32:$dst)
    , (ins F32:$src)
    , [(set F32:$dst, (node F32:$src))]
    , !strconcat(name, ".f32\t$dst, $src")
    , Op
    >;

  def _F64 : I
    < (outs F64:$dst)
    , (ins F64:$src)
    , [(set F64:$dst, (node F64:$src))]
    , !strconcat(name, ".f64\t$dst, $src")
    , Op
    >;

  def _F80 : I
    < (outs F80:$dst)
    , (ins F80:$src)
    , [(set F80:$dst, (node F80:$src))]
    , !strconcat(name, ".f80\t$dst, $src")
    , Op
    >;

  def _F128 : I
    < (outs F128:$dst)
    , (ins F128:$src)
    , [(set F128:$dst, (node F128:$src))]
    , !strconcat(name, ".f128\t$dst, $src")
    , Op
    >;
}

multiclass BinaryFloat
  < SDNode node
  , string name
  , bits<32> Op
  >
{
  def _F32 : I
    < (outs F32:$dst)
    , (ins F32:$lhs, F32:$rhs)
    , [(set F32:$dst, (node F32:$lhs, F32:$rhs))]
    , !strconcat(name, ".f32\t$dst, $lhs, $rhs")
    , Op
    >;

  def _F64 : I
    < (outs F64:$dst)
    , (ins F64:$lhs, F64:$rhs)
    , [(set F64:$dst, (node F64:$lhs, F64:$rhs))]
    , !strconcat(name, ".f64\t$dst, $lhs, $rhs")
    , Op
    >;

  def _F80 : I
    < (outs F80:$dst)
    , (ins F80:$lhs, F80:$rhs)
    , [(set F80:$dst, (node F80:$lhs, F80:$rhs))]
    , !strconcat(name, ".f80\t$dst, $lhs, $rhs")
    , Op
    >;

  def _F128 : I
    < (outs F128:$dst)
    , (ins F128:$lhs, F128:$rhs)
    , [(set F128:$dst, (node F128:$lhs, F128:$rhs))]
    , !strconcat(name, ".f128\t$dst, $lhs, $rhs")
    , Op
    >;
}

multiclass CommBinaryFloat
  < SDNode node
  , string name
  , bits<32> Op
  >
{
  let isCommutable = 1 in
  defm "" : BinaryFloat<node, name, Op>;
}

multiclass CmpFloatVariant
  < SDNode cc
  , CondCode cond
  , string name
  , bits<32> op
  >
{
  def _F32 : I
    < (outs I8:$dst)
    , (ins F32:$lhs, F32:$rhs)
    , [(set I8:$dst, (cc F32:$lhs, F32:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i8\t$dst, $lhs, $rhs"))
    , op
    >;

  def _F64 : I
    < (outs I8:$dst)
    , (ins F64:$lhs, F64:$rhs)
    , [(set I8:$dst, (cc F64:$lhs, F64:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i8\t$dst, $lhs, $rhs"))
    , op
    >;

  def _F80 : I
    < (outs I8:$dst)
    , (ins F80:$lhs, F80:$rhs)
    , [(set I8:$dst, (cc F80:$lhs, F80:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i8\t$dst, $lhs, $rhs"))
    , op
    >;

  def _F128 : I
    < (outs I8:$dst)
    , (ins F128:$lhs, F128:$rhs)
    , [(set I8:$dst, (cc F128:$lhs, F128:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i8\t$dst, $lhs, $rhs"))
    , op
    >;
}

multiclass CmpFloat
  < CondCode cond
  , string name
  , bits<32> op
  >
{
  defm _SETCC
    : CmpFloatVariant
      < setcc
      , cond
      , name
      , op
      >;
  defm _ANY_FSETCC
    : CmpFloatVariant
      < strict_fsetcc
      , cond
      , !strconcat(name, ".strict")
      , op
      >;
  defm _STRICT_FSETCCS
    : CmpFloatVariant
      < strict_fsetccs
      , cond
      , !strconcat(name, ".strict")
      , op
      >;
}

multiclass TernaryFloat
  < SDNode node
  , string name
  , bits<32> Op
  >
{
  def _F32 : I
    < (outs F32:$dst)
    , (ins I8:$op, F32:$lhs, F32:$rhs)
    , [(set F32:$dst, (node I8:$op, F32:$lhs, F32:$rhs))]
    , !strconcat(name, ".f32\t$dst, $op, $lhs, $rhs")
    , Op
    >;

  def _F64 : I
    < (outs F64:$dst)
    , (ins I8:$op, F64:$lhs, F64:$rhs)
    , [(set F64:$dst, (node I8:$op, F64:$lhs, F64:$rhs))]
    , !strconcat(name, ".f64\t$dst, $op, $lhs, $rhs")
    , Op
    >;

  def _F80 : I
    < (outs F80:$dst)
    , (ins I8:$op, F80:$lhs, F80:$rhs)
    , [(set F80:$dst, (node I8:$op, F80:$lhs, F80:$rhs))]
    , !strconcat(name, ".f80\t$dst, $op, $lhs, $rhs")
    , Op
    >;

  def _F128 : I
    < (outs F128:$dst)
    , (ins I8:$op, F128:$lhs, F128:$rhs)
    , [(set F128:$dst, (node I8:$op, F128:$lhs, F128:$rhs))]
    , !strconcat(name, ".f128\t$dst, $op, $lhs, $rhs")
    , Op
    >;
}


defm FABS   : UnaryFloat<fabs,   "abs",    0xB0>;
defm FNEG   : UnaryFloat<fneg,   "neg",    0xB2>;
defm FSIN   : UnaryFloat<fsin,   "sin",    0xB2>;
defm FCOS   : UnaryFloat<fcos,   "cos",    0xB2>;
defm FSQRT  : UnaryFloat<fsqrt,  "sqrt",   0xB2>;
defm FEXP   : UnaryFloat<fexp,   "exp",    0xB2>;
defm FEXP2  : UnaryFloat<fexp2,  "exp2",   0xB2>;
defm FLOG   : UnaryFloat<flog,   "log",    0xB2>;
defm FLOG2  : UnaryFloat<flog2,  "log2",   0xB2>;
defm FLOG10 : UnaryFloat<flog10, "log10",  0xB2>;
defm FCEIL  : UnaryFloat<fceil,  "f_ceil",  0xB2>;
defm FFLOOR : UnaryFloat<ffloor, "f_floor", 0xB2>;
defm FTRUNC : UnaryFloat<ftrunc, "trunc",  0xB2>;

defm FADD_R : CommBinaryFloat<fadd, "add",  0x90>;
defm FMUL_R : CommBinaryFloat<fmul, "mul",  0x90>;
defm FADD_S : CommBinaryFloat<strict_fadd, "add.strict",  0x90>;
defm FMUL_S : CommBinaryFloat<strict_fmul, "mul.strict",  0x90>;

defm FSUB_R    : BinaryFloat<fsub, "sub", 0xA0>;
defm FDIV_R    : BinaryFloat<fdiv, "s_div", 0xA1>;
defm FREM_R    : BinaryFloat<frem, "s_rem", 0xA3>;
defm FSUB_S    : BinaryFloat<strict_fsub, "sub.strict", 0xA0>;
defm FDIV_S    : BinaryFloat<strict_fdiv, "s_div.strict", 0xA1>;
defm FREM_S    : BinaryFloat<strict_frem, "s_rem.strict", 0xA3>;

defm COPYSIGN : BinaryFloat<fcopysign, "copy_sign", 0x98>;
defm FPOW     : BinaryFloat<fpow, "pow", 0xA4>;

defm O : CmpFloat<SETO, "o", 0x5b>;
defm OEQ : CmpFloat<SETOEQ, "oeq", 0x5b>;
defm ONE : CmpFloat<SETONE, "one", 0x5c>;
defm OLT : CmpFloat<SETOLT, "olt", 0x5d>;
defm OLE : CmpFloat<SETOLE, "ole", 0x5f>;
defm OGT : CmpFloat<SETOGT, "ogt", 0x5e>;
defm OGE : CmpFloat<SETOGE, "oge", 0x60>;

defm UO : CmpFloat<SETUO, "uo", 0x5b>;
defm UEQ : CmpFloat<SETUEQ, "ueq", 0x5b>;
defm UNE : CmpFloat<SETUNE, "une", 0x5c>;
defm ULT : CmpFloat<SETULT, "ult", 0x5d>;
defm ULE : CmpFloat<SETULE, "ule", 0x5f>;
defm UGT : CmpFloat<SETUGT, "ugt", 0x5e>;
defm UGE : CmpFloat<SETUGE, "uge", 0x60>;

defm EQ : CmpFloat<SETEQ, "eq", 0x5b>;
defm NE : CmpFloat<SETNE, "ne", 0x5c>;
defm LT : CmpFloat<SETLT, "lt", 0x5d>;
defm LE : CmpFloat<SETLE, "le", 0x5f>;
defm GT : CmpFloat<SETGT, "gt", 0x5e>;
defm GE : CmpFloat<SETGE, "ge", 0x60>;

defm SELECT : TernaryFloat<select, "select", 0xB0>;

def : Pat
  < (select (i8 (setne I8:$cond, 0)), F32:$lhs, F32:$rhs)
  , (SELECT_F32 I8:$cond, F32:$lhs, F32:$rhs)
  >;
def : Pat
  < (select (i8 (setne I8:$cond, 0)), F64:$lhs, F64:$rhs)
  , (SELECT_F64 I8:$cond, F64:$lhs, F64:$rhs)
  >;
def : Pat
  < (select (i8 (setne I8:$cond, 0)), F80:$lhs, F80:$rhs)
  , (SELECT_F80 I8:$cond, F80:$lhs, F80:$rhs)
  >;
def : Pat
  < (select (i8 (setne I8:$cond, 0)), F128:$lhs, F128:$rhs)
  , (SELECT_F128 I8:$cond, F128:$lhs, F128:$rhs)
  >;

def : Pat
  < (select (i8 (seteq I8:$cond, 0)), F32:$lhs, F32:$rhs)
  , (SELECT_F32 I8:$cond, F32:$rhs, F32:$lhs)
  >;
def : Pat
  < (select (i8 (seteq I8:$cond, 0)), F64:$lhs, F64:$rhs)
  , (SELECT_F64 I8:$cond, F64:$rhs, F64:$lhs)
  >;
def : Pat
  < (select (i8 (seteq I8:$cond, 0)), F80:$lhs, F80:$rhs)
  , (SELECT_F80 I8:$cond, F80:$rhs, F80:$lhs)
  >;
def : Pat
  < (select (i8 (seteq I8:$cond, 0)), F128:$lhs, F128:$rhs)
  , (SELECT_F128 I8:$cond, F128:$rhs, F128:$lhs)
  >;

