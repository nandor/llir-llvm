//===- GenMInstrControl.td-GenM control-flow --------------------*- tablegen -*-
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// GenM control-flow code-gen constructs.
///
//===----------------------------------------------------------------------===//

class Return<GenMRegClass vt>
  : TerminatorInst
    < (ins vt:$val)
    , [(GenMreturn vt:$val)]
    , "return \t$val"
    , 0x70
    >
{
  let isReturn = 1;
}

class ReturnVoid
  : TerminatorInst<(ins), [(GenMreturn)], "return", 0x70>
{
}

def RETURN_I32: Return<I32>;
def RETURN_I64: Return<I64>;
def RETURN_VOID: ReturnVoid;

let isBranch = 1, isTerminator = 1, hasCtrlDep = 1 in {
  def BR_IF
    : I
      < (outs)
      , (ins bb_op:$dst, I32:$cond)
      , [(brcond I32:$cond, bb:$dst)]
      , "br_if   \t$dst, $cond"
      , 0x0d
      >;
}
