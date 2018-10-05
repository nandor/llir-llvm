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
    >
  {
    let Defs = [ARGS];
  }

  def _I64 : I
    < (outs I64:$dst)
    , (ins I64:$lhs, I64:$rhs)
    , [(set I64:$dst, (node I64:$lhs, I64:$rhs))]
    , !strconcat(!strconcat(name, ".i64"), " \t$dst, $lhs, $rhs")
    , i64Inst
    >
  {
    let Defs = [ARGS];
  }
}

multiclass CommutativeInt
  < SDNode node
  , string name
  , bits<32> i32Inst
  , bits<32> i64Inst
  >
{
  let isCommutable = 1 in
  defm : BinaryInt<node, name, i32Inst, i64Inst>;
}

defm ADD : BinaryInt<add, "add",  0x90, 0x91>;
defm XOR : BinaryInt<xor, "xor",  0x92, 0x93>;
defm OR  : BinaryInt<or,  "or",   0x94, 0x95>;
defm AND : BinaryInt<and, "and",  0x96, 0x97>;
