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
/// LLIR vreg-to-vreg moves
///
//===----------------------------------------------------------------------===//

let isAsCheapAsAMove = 1, isReMaterializable = 1 in
{
  def MOV_I8   : I< (outs   I8:$dst), (ins   I8:$src), [], "mov.i8  \t$dst, $src", 0x09>;
  def MOV_I16  : I< (outs  I16:$dst), (ins  I16:$src), [], "mov.i16 \t$dst, $src", 0x09>;
  def MOV_I32  : I< (outs  I32:$dst), (ins  I32:$src), [], "mov.i32 \t$dst, $src", 0x09>;
  def MOV_I64  : I< (outs  I64:$dst), (ins  I64:$src), [], "mov.i64 \t$dst, $src", 0x0A>;
  def MOV_I128 : I< (outs I128:$dst), (ins I128:$src), [], "mov.i128\t$dst, $src", 0x09>;
  def MOV_F32  : I< (outs  F32:$dst), (ins  F32:$src), [], "mov.f32 \t$dst, $src", 0x0B>;
  def MOV_F64  : I< (outs  F64:$dst), (ins  F64:$src), [], "mov.f64 \t$dst, $src", 0x0C>;
  def MOV_F80  : I< (outs  F80:$dst), (ins  F80:$src), [], "mov.f80 \t$dst, $src", 0x0C>;
  def MOV_F128 : I< (outs F128:$dst), (ins F128:$src), [], "mov.f128\t$dst, $src", 0x0C>;
}
