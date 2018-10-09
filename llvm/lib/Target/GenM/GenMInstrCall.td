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


class CALL<GenMRegClass vt, string suffix>
  : CallInst
    < (outs vt:$dst)
    , (ins i64func_op:$callee, variable_ops)
    , [(set vt:$dst, (GenMcall (i64 imm:$callee)))]
    , !strconcat("call.", !strconcat(suffix, "\t$dst, $callee"))
    , 0x50
    >
{
}

def CALL_I32 : CALL<I32, "i32">;
def CALL_I64 : CALL<I64, "i64">;

def CALL_VOID : CallInst
  < (outs)
  , (ins i64func_op:$callee, variable_ops)
  , [(GenMvoid (i64 imm:$callee))]
  , "call\t$callee"
  , 0x50
  >;

def : Pat
  < (i32 (GenMcall tglobaladdr:$callee))
  , (CALL_I32 tglobaladdr:$callee)
  >;
def : Pat
  < (i64 (GenMcall tglobaladdr:$callee))
  , (CALL_I64 tglobaladdr:$callee)
  >;
def : Pat
  < (GenMvoid tglobaladdr:$callee)
  , (CALL_VOID tglobaladdr:$callee)
  >;

def : Pat
  < (i32 (GenMcall texternalsym:$callee))
  , (CALL_I32 texternalsym:$callee)
  >;
def : Pat
  < (i64 (GenMcall texternalsym:$callee))
  , (CALL_I64 texternalsym:$callee)
  >;
def : Pat
  < (GenMvoid texternalsym:$callee)
  , (CALL_VOID texternalsym:$callee)
  >;
