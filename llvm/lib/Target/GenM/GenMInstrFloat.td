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
  , bits<32> Op32
  , bits<32> Op64
  , bits<32> Op80
  >
{
  def _F32 : I
    < (outs F32:$dst)
    , (ins F32:$src)
    , [(set F32:$dst, (node F32:$src))]
    , !strconcat(name, ".f32\t$dst, $src")
    , Op32
    >;

  def _F64 : I
    < (outs F64:$dst)
    , (ins F64:$src)
    , [(set F64:$dst, (node F64:$src))]
    , !strconcat(name, ".f64\t$dst, $src")
    , Op64
    >;

  def _F80 : I
    < (outs F80:$dst)
    , (ins F80:$src)
    , [(set F80:$dst, (node F80:$src))]
    , !strconcat(name, ".f80\t$dst, $src")
    , Op80
    >;
}

multiclass BinaryFloat
  < SDNode node
  , string name
  , bits<32> Op32
  , bits<32> Op64
  , bits<32> Op80
  >
{
  def _F32 : I
    < (outs F32:$dst)
    , (ins F32:$lhs, F32:$rhs)
    , [(set F32:$dst, (node F32:$lhs, F32:$rhs))]
    , !strconcat(name, ".f32\t$dst, $lhs, $rhs")
    , Op32
    >;

  def _F64 : I
    < (outs F64:$dst)
    , (ins F64:$lhs, F64:$rhs)
    , [(set F64:$dst, (node F64:$lhs, F64:$rhs))]
    , !strconcat(name, ".f64\t$dst, $lhs, $rhs")
    , Op64
    >;

  def _F80 : I
    < (outs F80:$dst)
    , (ins F80:$lhs, F80:$rhs)
    , [(set F80:$dst, (node F80:$lhs, F80:$rhs))]
    , !strconcat(name, ".f80\t$dst, $lhs, $rhs")
    , Op80
    >;
}

multiclass CommBinaryFloat
  < SDNode node
  , string name
  , bits<32> Op32
  , bits<32> Op64
  , bits<32> Op80
  >
{
  let isCommutable = 1 in
  defm "" : BinaryFloat<node, name, Op32, Op64, Op80>;
}

multiclass CmpFloat
  < CondCode cond
  , string name
  , bits<32> Op32
  , bits<32> Op64
  , bits<32> Op80
  >
{
  def _F32 : I
    < (outs I8:$dst)
    , (ins F32:$lhs, F32:$rhs)
    , [(set I8:$dst, (setcc F32:$lhs, F32:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i8\t$dst, $lhs, $rhs"))
    , Op32
    >;

  def _F64 : I
    < (outs I8:$dst)
    , (ins F64:$lhs, F64:$rhs)
    , [(set I8:$dst, (setcc F64:$lhs, F64:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i8\t$dst, $lhs, $rhs"))
    , Op64
    >;

  def _F80 : I
    < (outs I8:$dst)
    , (ins F80:$lhs, F80:$rhs)
    , [(set I8:$dst, (setcc F80:$lhs, F80:$rhs, cond))]
    , !strconcat("cmp.", !strconcat(name, ".i8\t$dst, $lhs, $rhs"))
    , Op80
    >;
}

multiclass CommCmpFloat
  < SDNode node
  , string name
  , bits<32> Op32
  , bits<32> Op64
  , bits<32> Op80
  >
{
  let isCommutable = 1 in
  defm "" : CmpFloat< node, name, Op32, Op64, Op80>;
}

multiclass TernaryFloat
  < SDNode node
  , string name
  , bits<32> Op32
  , bits<32> Op64
  , bits<32> Op80
  >
{
  def _F32 : I
    < (outs F32:$dst)
    , (ins I8:$op, F32:$lhs, F32:$rhs)
    , [(set F32:$dst, (node I8:$op, F32:$lhs, F32:$rhs))]
    , !strconcat(name, ".f32\t$dst, $op, $lhs, $rhs")
    , Op32
    >;

  def _F64 : I
    < (outs F64:$dst)
    , (ins I8:$op, F64:$lhs, F64:$rhs)
    , [(set F64:$dst, (node I8:$op, F64:$lhs, F64:$rhs))]
    , !strconcat(name, ".f64\t$dst, $op, $lhs, $rhs")
    , Op64
    >;

  def _F80 : I
    < (outs F80:$dst)
    , (ins I8:$op, F80:$lhs, F80:$rhs)
    , [(set F80:$dst, (node I8:$op, F80:$lhs, F80:$rhs))]
    , !strconcat(name, ".f80\t$dst, $op, $lhs, $rhs")
    , Op80
    >;
}


defm FABS   : UnaryFloat<fabs,   "abs",    0xB0, 0xB1, 0x00>;
defm FNEG   : UnaryFloat<fneg,   "neg",    0xB2, 0xB3, 0x00>;
defm FSIN   : UnaryFloat<fsin,   "sin",    0xB2, 0xB3, 0x00>;
defm FCOS   : UnaryFloat<fcos,   "cos",    0xB2, 0xB3, 0x00>;
defm FSQRT  : UnaryFloat<fsqrt,  "sqrt",   0xB2, 0xB3, 0x00>;
defm FEXP   : UnaryFloat<fexp,   "exp",    0xB2, 0xB3, 0x00>;
defm FEXP2  : UnaryFloat<fexp2,  "exp2",   0xB2, 0xB3, 0x00>;
defm FLOG   : UnaryFloat<flog,   "log",    0xB2, 0xB3, 0x00>;
defm FLOG2  : UnaryFloat<flog2,  "log2",   0xB2, 0xB3, 0x00>;
defm FLOG10 : UnaryFloat<flog10, "log10",  0xB2, 0xB3, 0x00>;
defm FCEIL  : UnaryFloat<fceil,  "fceil",  0xB2, 0xB3, 0x00>;
defm FFLOOR : UnaryFloat<ffloor, "ffloor", 0xB2, 0xB3, 0x00>;

defm FADD : CommBinaryFloat<fadd, "add",  0x90, 0x91, 0x00>;
defm FMUL : CommBinaryFloat<fmul, "mul",  0x90, 0x90, 0x00>;

defm COPYSIGN : BinaryFloat<fcopysign, "copysign", 0x98, 0xa6, 0x00>;
defm FSUB     : BinaryFloat<fsub, "sub", 0xA0, 0xA1, 0x00>;
defm FDIV     : BinaryFloat<fdiv, "div", 0xA1, 0xA2, 0x00>;
defm FREM     : BinaryFloat<frem, "rem", 0xA3, 0xA2, 0x00>;
defm FPOW     : BinaryFloat<fpow, "pow", 0xA4, 0xA5, 0x00>;

defm OEQ : CmpFloat<SETOEQ, "oeq", 0x5b, 0x00, 0x00>;
defm ONE : CmpFloat<SETONE, "one", 0x5c, 0x00, 0x00>;
defm OLT : CmpFloat<SETOLT, "olt", 0x5d, 0x00, 0x00>;
defm OLE : CmpFloat<SETOLE, "ole", 0x5f, 0x00, 0x00>;
defm OGT : CmpFloat<SETOGT, "ogt", 0x5e, 0x00, 0x00>;
defm OGE : CmpFloat<SETOGE, "oge", 0x60, 0x00, 0x00>;

defm UEQ : CmpFloat<SETUEQ, "ueq", 0x5b, 0x00, 0x00>;
defm UNE : CmpFloat<SETUNE, "une", 0x5c, 0x00, 0x00>;
defm ULT : CmpFloat<SETULT, "ult", 0x5d, 0x00, 0x00>;
defm ULE : CmpFloat<SETULE, "ule", 0x5f, 0x00, 0x00>;
defm UGT : CmpFloat<SETUGT, "ugt", 0x5e, 0x00, 0x00>;
defm UGE : CmpFloat<SETUGE, "uge", 0x60, 0x00, 0x00>;

defm EQ : CmpFloat<SETEQ, "eq", 0x5b, 0x00, 0x00>;
defm NE : CmpFloat<SETNE, "ne", 0x5c, 0x00, 0x00>;
defm LT : CmpFloat<SETLT, "lt", 0x5d, 0x00, 0x00>;
defm LE : CmpFloat<SETLE, "le", 0x5f, 0x00, 0x00>;
defm GT : CmpFloat<SETGT, "gt", 0x5e, 0x00, 0x00>;
defm GE : CmpFloat<SETGE, "ge", 0x60, 0x00, 0x00>;

defm SELECT : TernaryFloat<select, "select", 0xB0, 0xB1, 0xB2>;

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

