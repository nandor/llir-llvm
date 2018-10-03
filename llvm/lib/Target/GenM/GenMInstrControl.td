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

class RETURN<GenMRegClass vt>
  : I
    < (outs)
    , (ins vt:$val)
    , [(GenMreturn vt:$val)]
    , "return  \t$val"
    , 0x70
    >
{
}

let isReturn = 1 in {
  def RETURN_I32: RETURN<I32>;
  def RETURN_I64: RETURN<I64>;
}
