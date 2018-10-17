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


class CALL<GenMRegClass vt, string suffix> : CallInst
  < (outs vt:$dst)
  , (ins I64:$callee, variable_ops)
  , [(set vt:$dst, (GenMcall I64:$callee))]
  , !strconcat("call.", !strconcat(suffix, "\t$dst, $callee"))
  , 0x50
  >
{
}

def CALL_I32: CALL<I32, "i32">;
def CALL_I64: CALL<I64, "i64">;

def CALL_VOID : CallInst
  < (outs)
  , (ins I64:$callee, variable_ops)
  , [(GenMvoid I64:$callee)]
  , "call\t$callee"
  , 0x52
  >;
