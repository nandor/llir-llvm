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
/// LLIR address-generating or address-specific instructions.
///
//===----------------------------------------------------------------------===//

let isAsCheapAsAMove = 1, isReMaterializable = 1 in
{
  let Predicates = [Is32Bit] in
  {
    def ADDR_I32 : I
      < (outs I32:$dst)
      , (ins i32addr_op:$addr)
      , []
      , "mov.i32\t$dst, $addr"
      , 0x0D
      >;

    def FRAME_I32 : I
      < (outs I32:$res)
      , (ins i32imm:$index, i32imm:$offset)
      , []
      , "frame.i32\t$res, $index, $offset"
      , 0x0E
      >;
  }

  let Predicates = [Is64Bit] in
  {
    def ADDR_I64 : I
      < (outs I64:$dst)
      , (ins i64addr_op:$addr)
      , []
      , "mov.i64\t$dst, $addr"
      , 0x0D
      >;

    def FRAME_I64 : I
      < (outs I64:$res)
      , (ins i32imm:$index, i64imm:$offset)
      , []
      , "frame.i64\t$res, $index, $offset"
      , 0x0E
      >;
  }
}

let Predicates = [Is32Bit] in
{
  def : Pat<(LLIRsymbol tglobaladdr:$addr),    (ADDR_I32 i32addr_op:$addr)>;
  def : Pat<(LLIRsymbol texternalsym:$addr),   (ADDR_I32 i32addr_op:$addr)>;
  def : Pat<(LLIRsymbol tconstpool:$addr),     (ADDR_I32 i32addr_op:$addr)>;
  def : Pat<(LLIRsymbol tglobaltlsaddr:$addr), (ADDR_I32 i32addr_op:$addr)>;
}

let Predicates = [Is64Bit] in
{
  def : Pat<(LLIRsymbol tglobaladdr:$addr),    (ADDR_I64 i64addr_op:$addr)>;
  def : Pat<(LLIRsymbol texternalsym:$addr),   (ADDR_I64 i64addr_op:$addr)>;
  def : Pat<(LLIRsymbol tconstpool:$addr),     (ADDR_I64 i64addr_op:$addr)>;
  def : Pat<(LLIRsymbol tglobaltlsaddr:$addr), (ADDR_I64 i64addr_op:$addr)>;
}

//===----------------------------------------------------------------------===//
// Instructions with side effects that depend on pointer size.
//===----------------------------------------------------------------------===//

let hasSideEffects = 1 in
{
  let Predicates = [Is32Bit] in
  {
    def VASTART_I32 : I
      < (outs)
      , (ins I32:$frame)
      , [(LLIRvastart I32:$frame)]
      , "vastart\t$frame"
      , 0x00
      >;

    def ALLOCA_I32 : I
      < (outs I32:$res)
      , (ins I32:$size, i64imm:$align)
      , [(set I32:$res, (LLIRalloca I32:$size, imm:$align))]
      , "alloca.i32\t$res, $size, $align"
      , 0x0F
      >;

    def GET_I32 : I
      < (outs I32:$dst)
      , (ins R32:$src)
      , []
      , "get.i32\t$dst, $src"
      , 0x0A
      >;

    def SET_I32 : I
      < (outs)
      , (ins R32:$dst, I32:$src)
      , []
      , "set\t$dst, $src"
      , 0x0A
      >;
  }

  let Predicates = [Is64Bit] in
  {
    def VASTART_I64 : I
      < (outs)
      , (ins I64:$frame)
      , [(LLIRvastart I64:$frame)]
      , "vastart\t$frame"
      , 0x00
      >;

    def ALLOCA_I64 : I
      < (outs I64:$res)
      , (ins I64:$size, i64imm:$align)
      , [(set I64:$res, (LLIRalloca I64:$size, imm:$align))]
      , "alloca.i64\t$res, $size, $align"
      , 0x0F
      >;

    def GET_I64 : I
      < (outs I64:$dst)
      , (ins R64:$src)
      , []
      , "get.i64\t$dst, $src"
      , 0x0A
      >;

    def SET_I64 : I
      < (outs)
      , (ins R64:$dst, I64:$src)
      , []
      , "set\t$dst, $src"
      , 0x0A
      >;
  }
}
