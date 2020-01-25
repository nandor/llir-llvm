//===- GenMInstrCall.td-GenM Call codegen support -------------*- tablegen -*-//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// GenM Call operand code-gen constructs.
///
//===----------------------------------------------------------------------===//


multiclass CALL<GenMRegClass vt, SDNode node, SDNode node_va>
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

defm CALL_I8  : CALL<I8, GenMcall, GenMcall_va>;
defm CALL_I16 : CALL<I16, GenMcall, GenMcall_va>;
defm CALL_I32 : CALL<I32, GenMcall, GenMcall_va>;
defm CALL_I64 : CALL<I64, GenMcall, GenMcall_va>;
defm CALL_F32 : CALL<F32, GenMcall, GenMcall_va>;
defm CALL_F64 : CALL<F64, GenMcall, GenMcall_va>;
defm CALL_F80 : CALL<F80, GenMcall, GenMcall_va>;

def CALL_VOID : CallInst
  < (outs)
  , (ins i32imm:$conv, I64:$callee, variable_ops)
  , [(GenMvoid I64:$callee, timm:$conv)]
  , ""
  , 0x53
  >;

def CALL_VOID_VA : CallInst
  < (outs)
  , (ins i32imm:$fixed, i32imm:$conv, I64:$callee, variable_ops)
  , [(GenMvoid_va I64:$callee, timm:$conv, timm:$fixed)]
  , ""
  , 0x53
  >;



multiclass TCALL<GenMRegClass vt, SDNode node, SDNode node_va>
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

defm TCALL_I8  : TCALL<I8,  GenMtcall, GenMtcall_va>;
defm TCALL_I16 : TCALL<I16, GenMtcall, GenMtcall_va>;
defm TCALL_I32 : TCALL<I32, GenMtcall, GenMtcall_va>;
defm TCALL_I64 : TCALL<I64, GenMtcall, GenMtcall_va>;
defm TCALL_F32 : TCALL<F32, GenMtcall, GenMtcall_va>;
defm TCALL_F64 : TCALL<F64, GenMtcall, GenMtcall_va>;
defm TCALL_F80 : TCALL<F80, GenMtcall, GenMtcall_va>;

def TCALL_VOID : CallInst
  < (outs)
  , (ins i32imm:$conv, I64:$callee, variable_ops)
  , [(GenMtvoid I64:$callee, timm:$conv)]
  , ""
  , 0x54
  >
{
  let isTerminator = 1;
}

def TCALL_VOID_VA : CallInst
  < (outs)
  , (ins i32imm:$fixed, i32imm:$conv, I64:$callee, variable_ops)
  , [(GenMtvoid_va I64:$callee, timm:$conv, timm:$fixed)]
  , ""
  , 0x54
  >
{
  let isTerminator = 1;
}
