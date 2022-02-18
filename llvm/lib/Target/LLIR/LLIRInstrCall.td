//===- LLIRInstrCall.td-LLIR Call codegen support -------------*- tablegen -*-//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// LLIR Call operand code-gen constructs.
///
//===----------------------------------------------------------------------===//

foreach i = [I8, I16, I32, I64, I128, F32, F64, F80, F128] in {
  def CALL_ # i : CallInst
    < (outs i:$dst)
    , (ins i32imm:$conv, variable_ops)
    , [(set i:$dst, (LLIRcall timm:$conv))]
    , ""
    , 0x50
    >;

  def CALL_VA_ # i : CallInst
    < (outs i:$dst)
    , (ins i32imm:$fixed, i32imm:$conv, variable_ops)
    , [(set i:$dst, (LLIRcall_va timm:$conv, timm:$fixed))]
    , ""
    , 0x50
    >;

  def TCALL_ # i : CallInst
    < (outs i:$dst)
    , (ins i32imm:$conv, variable_ops)
    , [(set i:$dst, (LLIRtcall timm:$conv))]
    , ""
    , 0x50
    >
  {
    let isTerminator = 1;
  }

  def TCALL_VA_ # i : CallInst
    < (outs i:$dst)
    , (ins i32imm:$fixed, i32imm:$conv, variable_ops)
    , [(set i:$dst, (LLIRtcall_va timm:$conv, timm:$fixed))]
    , ""
    , 0x51
    >
  {
    let isTerminator = 1;
  }

  def INVOKE_ # i : CallInst
    < (outs i:$dst)
    , (ins i32imm:$conv, bb_op:$throw, variable_ops)
    , [(set i:$dst, (LLIRtcall timm:$conv, bb:$throw))]
    , ""
    , 0x50
    >
  {
    let isTerminator = 1;
  }

  def INVOKE_VA_ # i : CallInst
    < (outs i:$dst)
    , (ins i32imm:$fixed, i32imm:$conv, bb_op:$throw, variable_ops)
    , [(set i:$dst, (LLIRtcall_va timm:$conv, timm:$fixed, bb:$throw))]
    , ""
    , 0x51
    >
  {
    let isTerminator = 1;
  }
}

def CALL_VOID : CallInst
  < (outs)
  , (ins i32imm:$conv, variable_ops)
  , [(LLIRvoid timm:$conv)]
  , ""
  , 0x53
  >;

def TCALL_VOID : CallInst
  < (outs)
  , (ins i32imm:$conv, variable_ops)
  , [(LLIRtvoid timm:$conv)]
  , ""
  , 0x54
  >
{
  let isTerminator = 1;
}

def INVOKE_VOID : CallInst
  < (outs)
  , (ins i32imm:$conv, bb_op:$throw, variable_ops)
  , [(LLIRivoid timm:$conv, bb:$throw)]
  , ""
  , 0x54
  >
{
  let isTerminator = 1;
}

def CALL_VA_VOID : CallInst
  < (outs)
  , (ins i32imm:$fixed, i32imm:$conv, variable_ops)
  , [(LLIRvoid_va timm:$conv, timm:$fixed)]
  , ""
  , 0x53
  >;

def TCALL_VA_VOID : CallInst
  < (outs)
  , (ins i32imm:$fixed, i32imm:$conv, variable_ops)
  , [(LLIRtvoid_va timm:$conv, timm:$fixed)]
  , ""
  , 0x54
  >
{
  let isTerminator = 1;
}

def INVOKE_VA_VOID : CallInst
  < (outs)
  , (ins i32imm:$fixed, i32imm:$conv, bb_op:$throw, variable_ops)
  , [(LLIRtvoid_va timm:$conv, timm:$fixed, bb:$throw)]
  , ""
  , 0x54
  >
{
  let isTerminator = 1;
}
