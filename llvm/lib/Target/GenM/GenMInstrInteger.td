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
  def I32 : I
    < (outs I32:$dst)
    , (ins I32:$lhs, I32:$rhs)
    , [(set I32:$dst, (node I32:$lhs, I32:$rhs))]
    , !strconcat("i32.", !strconcat(name, " \t$dst, $lhs, $rhs"))
    , i32Inst
    >
  {
    let Defs = [ARGS];
  }

  def I64 : I
    < (outs I64:$dst)
    , (ins I64:$lhs, I64:$rhs)
    , [(set I64:$dst, (node I64:$lhs, I64:$rhs))]
    , !strconcat("i64.", !strconcat(name, " \t$dst, $lhs, $rhs"))
    , i64Inst
    >
  {
    let Defs = [ARGS];
  }
}

defm ADD : BinaryInt<add, "add", 0x90, 0x91>;
