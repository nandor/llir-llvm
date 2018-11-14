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


multiclass CALL<GenMRegClass vt, string suffix>
{
  def "" : CallInst
    < (outs vt:$dst)
    , (ins i32imm:$conv, I64:$callee, variable_ops)
    , [(set vt:$dst, (GenMcall I64:$callee, timm:$conv))]
    , ""
    , 0x50
    >;

  def _VA : CallInst
    < (outs vt:$dst)
    , (ins i32imm:$conv, I64:$callee, i32imm:$fixed, variable_ops)
    , [(set vt:$dst, (GenMcall_va I64:$callee, timm:$conv, timm:$fixed))]
    , ""
    , 0x51
    >;
}

defm CALL_I32: CALL<I32, "i32">;
defm CALL_I64: CALL<I64, "i64">;
defm CALL_F32: CALL<F32, "f32">;
defm CALL_F64: CALL<F64, "f64">;

def CALL_VOID : CallInst
  < (outs)
  , (ins i32imm:$conv, I64:$callee, variable_ops)
  , [(GenMvoid I64:$callee, timm:$conv)]
  , ""
  , 0x53
  >;

def CALL_VOID_VA : CallInst
  < (outs)
  , (ins i32imm:$conv, I64:$callee, i32imm:$fixed, variable_ops)
  , [(GenMvoid_va I64:$callee, timm:$conv, timm:$fixed)]
  , ""
  , 0x53
  >;

def TCALL : CallInst
  < (outs)
  , (ins i32imm:$conv, I64:$callee, variable_ops)
  , [(GenMtcall I64:$callee, timm:$conv)]
  , ""
  , 0x54
  >
{
  let isBarrier = 1;
}

def TCALL_VA : CallInst
  < (outs)
  , (ins i32imm:$conv, I64:$callee, i32imm:$fixed, variable_ops)
  , [(GenMtcall_va I64:$callee, timm:$conv, timm:$fixed)]
  , ""
  , 0x54
  >
{
  let isBarrier = 1;
}
