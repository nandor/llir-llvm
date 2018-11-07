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

class Return<GenMRegClass vt, string name>
  : TerminatorInst
    < (ins vt:$val)
    , [(GenMreturn vt:$val)]
    , !strconcat("ret.", !strconcat(name, "\t$val"))
    , 0x70
    >
{
  let isReturn = 1;
  let isBarrier = 1;
}

def RETURN_I32: Return<G32, "i32">;
def RETURN_I64: Return<G64, "i64">;
def RETURN_VOID: TerminatorInst<(ins), [(GenMreturn)], "ret", 0x70>
{
  let isReturn = 1;
  let isBarrier = 1;
}

def JT : BranchInst<(ins G32:$cond, bb_op:$dst), [], "jt \t$cond, $dst", 0x72>;
def JF : BranchInst<(ins G32:$cond, bb_op:$dst), [], "jf \t$cond, $dst", 0x73>;
def JMP : BranchInst<(ins bb_op:$dst), [(br bb:$dst)], "jmp \t$dst", 0x71>
{
  let isBarrier = 1;
}

def : Pat
  < (brcond (i32 (setne G32:$cond, 0)), bb:$dst)
  , (JT G32:$cond, bb_op:$dst)
  >;

def : Pat
  < (brcond i32:$cond, bb:$dst)
  , (JT G32:$cond, bb_op:$dst)
  >;

def : Pat
  < (brcond (i32 (seteq G32:$cond, 0)), bb:$dst)
  , (JF G32:$cond, bb_op:$dst)
  >;

def TRAP: TerminatorInst<(ins), [(trap)], "trap", 0x75>;
