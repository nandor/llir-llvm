//===- LLIRInstrControl.td-LLIR control-flow --------------------*- tablegen -*-
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// LLIR control-flow code-gen constructs.
///
//===----------------------------------------------------------------------===//

class Return<LLIRRegClass vt>
  : TerminatorInst
    < (ins vt:$val)
    , [(LLIRreturn vt:$val)]
    , "ret\t$val"
    , 0x70
    >
{
  let isReturn = 1;
  let isBarrier = 1;
}

def RETURN_I8 : Return<I8>;
def RETURN_I16 : Return<I16>;
def RETURN_I32 : Return<I32>;
def RETURN_I64 : Return<I64>;
def RETURN_F32 : Return<F32>;
def RETURN_F64 : Return<F64>;
def RETURN_F80 : Return<F80>;
def RETURN_VOID : TerminatorInst<(ins), [(LLIRreturn)], "ret", 0x70>
{
  let isReturn = 1;
  let isBarrier = 1;
}

def JT : BranchInst<(ins I8:$cond, bb_op:$dst), [], "jt \t$cond, $dst", 0x72>;
def JF : BranchInst<(ins I8:$cond, bb_op:$dst), [], "jf \t$cond, $dst", 0x73>;
def JMP : BranchInst<(ins bb_op:$dst), [(br bb:$dst)], "jmp \t$dst", 0x71>
{
  let isBarrier = 1;
}

def : Pat
  < (brcond (i8 (setne I8:$cond, 0)), bb:$dst)
  , (JT I8:$cond, bb_op:$dst)
  >;

def : Pat
  < (brcond i8:$cond, bb:$dst)
  , (JT I8:$cond, bb_op:$dst)
  >;

def : Pat
  < (brcond (i8 (seteq I8:$cond, 0)), bb:$dst)
  , (JF I8:$cond, bb_op:$dst)
  >;

def TRAP: TerminatorInst<(ins), [(trap)], "trap", 0x75>;

def SWITCH_I64 : TerminatorInst
  < (ins I64:$index, variable_ops)
  , [(LLIRswitch I64:$index)]
  , "switch\t$index"
  , 0x72
  >
{
  let hasCtrlDep = 1;
  let isBarrier = 1;
}
