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


multiclass CALL<LLIRRegClass vt, SDNode node, SDNode node_va>
{
  def "" : CallInst
    < (outs vt:$dst)
    , (ins i32imm:$conv, I64:$callee, variable_ops)
    , [(set vt:$dst, (node I64:$callee, timm:$conv))]
    , ""
    , 0x50
    >;

  def _VA : CallInst
    < (outs vt:$dst)
    , (ins i32imm:$fixed, i32imm:$conv, I64:$callee, variable_ops)
    , [(set vt:$dst, (node_va I64:$callee, timm:$conv, timm:$fixed))]
    , ""
    , 0x51
    >;
}

defm CALL_I8   : CALL<I8,   LLIRcall, LLIRcall_va>;
defm CALL_I16  : CALL<I16,  LLIRcall, LLIRcall_va>;
defm CALL_I32  : CALL<I32,  LLIRcall, LLIRcall_va>;
defm CALL_I64  : CALL<I64,  LLIRcall, LLIRcall_va>;
defm CALL_I128 : CALL<I128, LLIRcall, LLIRcall_va>;
defm CALL_F32  : CALL<F32,  LLIRcall, LLIRcall_va>;
defm CALL_F64  : CALL<F64,  LLIRcall, LLIRcall_va>;
defm CALL_F80  : CALL<F80,  LLIRcall, LLIRcall_va>;
defm CALL_F128 : CALL<F128, LLIRcall, LLIRcall_va>;

def CALL_VOID : CallInst
  < (outs)
  , (ins i32imm:$conv, I64:$callee, variable_ops)
  , [(LLIRvoid I64:$callee, timm:$conv)]
  , ""
  , 0x53
  >;

def CALL_VOID_VA : CallInst
  < (outs)
  , (ins i32imm:$fixed, i32imm:$conv, I64:$callee, variable_ops)
  , [(LLIRvoid_va I64:$callee, timm:$conv, timm:$fixed)]
  , ""
  , 0x53
  >;



multiclass TCALL<LLIRRegClass vt, SDNode node, SDNode node_va>
{
  def "" : CallInst
    < (outs vt:$dst)
    , (ins i32imm:$conv, I64:$callee, variable_ops)
    , [(set vt:$dst, (node I64:$callee, timm:$conv))]
    , ""
    , 0x50
    >
  {
    let isTerminator = 1;
  }

  def _VA : CallInst
    < (outs vt:$dst)
    , (ins i32imm:$fixed, i32imm:$conv, I64:$callee, variable_ops)
    , [(set vt:$dst, (node_va I64:$callee, timm:$conv, timm:$fixed))]
    , ""
    , 0x51
    >
  {
    let isTerminator = 1;
  }
}

defm TCALL_I8   : TCALL<I8,   LLIRtcall, LLIRtcall_va>;
defm TCALL_I16  : TCALL<I16,  LLIRtcall, LLIRtcall_va>;
defm TCALL_I32  : TCALL<I32,  LLIRtcall, LLIRtcall_va>;
defm TCALL_I64  : TCALL<I64,  LLIRtcall, LLIRtcall_va>;
defm TCALL_I128 : TCALL<I128, LLIRtcall, LLIRtcall_va>;
defm TCALL_F32  : TCALL<F32,  LLIRtcall, LLIRtcall_va>;
defm TCALL_F64  : TCALL<F64,  LLIRtcall, LLIRtcall_va>;
defm TCALL_F80  : TCALL<F80,  LLIRtcall, LLIRtcall_va>;
defm TCALL_F128 : TCALL<F128, LLIRtcall, LLIRtcall_va>;

def TCALL_VOID : CallInst
  < (outs)
  , (ins i32imm:$conv, I64:$callee, variable_ops)
  , [(LLIRtvoid I64:$callee, timm:$conv)]
  , ""
  , 0x54
  >
{
  let isTerminator = 1;
}

def TCALL_VOID_VA : CallInst
  < (outs)
  , (ins i32imm:$fixed, i32imm:$conv, I64:$callee, variable_ops)
  , [(LLIRtvoid_va I64:$callee, timm:$conv, timm:$fixed)]
  , ""
  , 0x54
  >
{
  let isTerminator = 1;
}
