// LLIRInstrMemory.td - LLIR Memory codegen support -----------*- tablegen -*-//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// LLIR undefined value
///
//===----------------------------------------------------------------------===//

let isAsCheapAsAMove = 1, isReMaterializable = 1 in
{
  def UNDEF_I8   : I< (outs   I8:$res), (ins), [(set   I8:$res, undef)], "undef.i8  \t$res", 0x11>;
  def UNDEF_I16  : I< (outs  I16:$res), (ins), [(set  I16:$res, undef)], "undef.i16 \t$res", 0x11>;
  def UNDEF_I32  : I< (outs  I32:$res), (ins), [(set  I32:$res, undef)], "undef.i32 \t$res", 0x11>;
  def UNDEF_I64  : I< (outs  I64:$res), (ins), [(set  I64:$res, undef)], "undef.i64 \t$res", 0x12>;
  def UNDEF_F32  : I< (outs  F32:$res), (ins), [(set  F32:$res, undef)], "undef.f32 \t$res", 0x13>;
  def UNDEF_F64  : I< (outs  F64:$res), (ins), [(set  F64:$res, undef)], "undef.f64 \t$res", 0x14>;
  def UNDEF_F80  : I< (outs  F80:$res), (ins), [(set  F80:$res, undef)], "undef.f80 \t$res", 0x14>;
  def UNDEF_F128 : I< (outs F128:$res), (ins), [(set F128:$res, undef)], "undef.f128\t$res", 0x14>;
}
