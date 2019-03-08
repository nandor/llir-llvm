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
    , "ret\t$val"
    , 0x70
    >
{
  let isReturn = 1;
  let isBarrier = 1;
}

def RETURN_I32: Return<I32>;
def RETURN_I64: Return<I64>;
def RETURN_F32: Return<F32>;
def RETURN_F64: Return<F64>;
def RETURN_VOID: TerminatorInst<(ins), [(GenMreturn)], "ret", 0x70>
{
  let isReturn = 1;
  let isBarrier = 1;
}

def JT : BranchInst<(ins I32:$cond, bb_op:$dst), [], "jt \t$cond, $dst", 0x72>;
def JF : BranchInst<(ins I32:$cond, bb_op:$dst), [], "jf \t$cond, $dst", 0x73>;
def JMP : BranchInst<(ins bb_op:$dst), [(br bb:$dst)], "jmp \t$dst", 0x71>
{
  let isBarrier = 1;
}

def : Pat
  < (brcond (i32 (setne I32:$cond, 0)), bb:$dst)
  , (JT I32:$cond, bb_op:$dst)
  >;

def : Pat
  < (brcond i32:$cond, bb:$dst)
  , (JT I32:$cond, bb_op:$dst)
  >;

def : Pat
  < (brcond (i32 (seteq I32:$cond, 0)), bb:$dst)
  , (JF I32:$cond, bb_op:$dst)
  >;

def TRAP: TerminatorInst<(ins), [(trap)], "trap", 0x75>;

def SWITCH_I64 : TerminatorInst
  < (ins I64:$index, variable_ops)
  , [(GenMswitch I64:$index)]
  , "switch\t$index"
  , 0x72
  >
{
  let hasCtrlDep = 1;
  let isBarrier = 1;
}
