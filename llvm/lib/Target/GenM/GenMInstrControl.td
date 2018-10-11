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
  let isReturn = 1;
}

def RETURN_I32: Return<I32>;
def RETURN_I64: Return<I64>;
def RETURN_VOID: ReturnVoid;

def JMP : BranchInst<(ins bb_op:$dst), [(br bb:$dst)], "jmp \t$dst", 0x71>;
def JT : BranchInst<(ins bb_op:$dst, I32:$cond), [], "jt \t$cond, $dst", 0x72>;
def JF : BranchInst<(ins bb_op:$dst, I32:$cond), [], "jf \t$cond, $dst", 0x73>;

def : Pat
  < (brcond (i32 (setne I32:$cond, 0)), bb:$dst)
  , (JT bb_op:$dst, I32:$cond)
  >;

def : Pat
  < (brcond i32:$cond, bb:$dst)
  , (JT bb_op:$dst, I32:$cond)
  >;

def : Pat
  < (brcond (i32 (seteq I32:$cond, 0)), bb:$dst)
  , (JF bb_op:$dst, I32:$cond)
  >;

def JTABLE : I
  < (outs)
  , (ins I64:$idx, I32:$table)
  , [(GenMbrjt I64:$idx, tjumptable:$table)]
  , "jtable.i64\t$idx, $table"
  , 0x74
  >;
