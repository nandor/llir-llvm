//===-- LLIRInstrInfo.td - Target Description for LLIR Target -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the LLIR instructions in TableGen format.
//
//===----------------------------------------------------------------------===//


//===----------------------------------------------------------------------===//
// LLIR-specific DAG Node Types.
//===----------------------------------------------------------------------===//

def SDT_LLIRargument : SDTypeProfile<1,  1, [SDTCisVT<1, i32>]>;
def SDT_LLIRreturn   : SDTypeProfile<0, -1, []>;
def SDT_LLIRsymbol   : SDTypeProfile<1,  1, [SDTCisSameAs<0, 1>, SDTCisPtrTy<0>]>;
def SDT_LLIRswitch   : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;
def SDT_LLIRvastart  : SDTypeProfile<0,  1, [SDTCisPtrTy<0>]>;
def SDT_LLIRvalue    : SDTypeProfile<1,  0, []>;

def SDT_LLIRcall     : SDTypeProfile<1, -2, [SDTCisPtrTy<1>, SDTCisVT<2, i32>]>;
def SDT_LLIRcall_va  : SDTypeProfile<1, -3, [SDTCisPtrTy<1>, SDTCisVT<2, i32>, SDTCisVT<3, i32>]>;
def SDT_LLIRtcall    : SDTypeProfile<1, -2, [SDTCisPtrTy<1>, SDTCisVT<2, i32>]>;
def SDT_LLIRtcall_va : SDTypeProfile<1, -3, [SDTCisPtrTy<1>, SDTCisVT<2, i32>, SDTCisVT<3, i32>]>;

def SDT_LLIRvoid     : SDTypeProfile<0, -1, [SDTCisPtrTy<0>, SDTCisVT<1, i32>]>;
def SDT_LLIRvoid_va  : SDTypeProfile<0, -2, [SDTCisPtrTy<0>, SDTCisVT<1, i32>, SDTCisVT<2, i32>]>;
def SDT_LLIRtvoid    : SDTypeProfile<0, -1, [SDTCisPtrTy<0>, SDTCisVT<1, i32>]>;
def SDT_LLIRtvoid_va : SDTypeProfile<0, -2, [SDTCisPtrTy<0>, SDTCisVT<1, i32>, SDTCisVT<2, i32>]>;

def SDT_LLIRover     : SDTypeProfile<1,  2, [SDTCisSameAs<1, 2>]>;

def SDT_LLIRalloca   : SDTypeProfile<1,  2, [SDTCisInt<1>, SDTCisVT<2, i64>]>;

//===----------------------------------------------------------------------===//
// LLIR-specific DAG Nodes.
//===----------------------------------------------------------------------===//

def LLIRargument  : SDNode<"LLIRISD::ARGUMENT", SDT_LLIRargument>;
def LLIRreturn    : SDNode<"LLIRISD::RETURN",   SDT_LLIRreturn, [SDNPHasChain]>;
def LLIRsymbol    : SDNode<"LLIRISD::SYMBOL",   SDT_LLIRsymbol>;
def LLIRswitch    : SDNode<"LLIRISD::SWITCH",   SDT_LLIRswitch, [SDNPHasChain, SDNPVariadic]>;
def LLIRvastart   : SDNode<"LLIRISD::VASTART",  SDT_LLIRvastart, [SDNPHasChain]>;

def LLIRcall      : SDNode<"LLIRISD::CALL",     SDT_LLIRcall,     [SDNPHasChain, SDNPVariadic]>;
def LLIRcall_va   : SDNode<"LLIRISD::CALL",     SDT_LLIRcall_va,  [SDNPHasChain, SDNPVariadic]>;
def LLIRtcall     : SDNode<"LLIRISD::TCALL",    SDT_LLIRtcall,    [SDNPHasChain, SDNPVariadic]>;
def LLIRtcall_va  : SDNode<"LLIRISD::TCALL",    SDT_LLIRtcall_va, [SDNPHasChain, SDNPVariadic]>;
def LLIRvoid      : SDNode<"LLIRISD::VOID",     SDT_LLIRvoid,     [SDNPHasChain, SDNPVariadic]>;
def LLIRvoid_va   : SDNode<"LLIRISD::VOID",     SDT_LLIRvoid_va,  [SDNPHasChain, SDNPVariadic]>;
def LLIRtvoid     : SDNode<"LLIRISD::TVOID",    SDT_LLIRtvoid,    [SDNPHasChain, SDNPVariadic]>;
def LLIRtvoid_va  : SDNode<"LLIRISD::TVOID",    SDT_LLIRtvoid_va, [SDNPHasChain, SDNPVariadic]>;

def LLIRsaddo     : SDNode<"LLIRISD::SADDO",    SDT_LLIRover>;
def LLIRuaddo     : SDNode<"LLIRISD::UADDO",    SDT_LLIRover>;
def LLIRssubo     : SDNode<"LLIRISD::SSUBO",    SDT_LLIRover>;
def LLIRusubo     : SDNode<"LLIRISD::USUBO",    SDT_LLIRover>;
def LLIRsmulo     : SDNode<"LLIRISD::SMULO",    SDT_LLIRover>;
def LLIRumulo     : SDNode<"LLIRISD::UMULO",    SDT_LLIRover>;

def LLIRrdtsc     : SDNode<"LLIRISD::RDTSC",    SDT_LLIRvalue, [SDNPHasChain]>;

def LLIRalloca    : SDNode<"LLIRISD::ALLOCA",   SDT_LLIRalloca, [SDNPHasChain]>;

//===----------------------------------------------------------------------===//
// WebAssembly-specific Operands.
//===----------------------------------------------------------------------===//

class LLIROperand<ValueType ty, string type> : Operand<ty>
{
  let OperandNamespace = "LLIR";
  let OperandType = type;
}


def bb_op      : LLIROperand<OtherVT, "OPERAND_BASIC_BLOCK">;
def i8imm_op   : LLIROperand<i8,  "OPERAND_I8IMM">;
def i16imm_op  : LLIROperand<i16, "OPERAND_I16IMM">;
def i32imm_op  : LLIROperand<i32, "OPERAND_I32IMM">;
def i64imm_op  : LLIROperand<i64, "OPERAND_I64IMM">;
def i32off_op  : LLIROperand<i32, "OPERAND_I32OFF">;
def i64addr_op : LLIROperand<i64, "OPERAND_I64FUNC">;
def f32imm_op  : LLIROperand<f32, "OPERAND_F32IMM">;
def f64imm_op  : LLIROperand<f64, "OPERAND_F64IMM">;
def f80imm_op  : LLIROperand<f80, "OPERAND_F80IMM">;

//===----------------------------------------------------------------------===//
// LLIR Instruction Format Definitions.
//===----------------------------------------------------------------------===//

include "LLIRInstrFormats.td"

//===----------------------------------------------------------------------===//
// Additional instructions.
//===----------------------------------------------------------------------===//

let isAsCheapAsAMove = 1, isReMaterializable = 1 in
{
  def MOV_I8 : I
    < (outs I8:$dst)
    , (ins I8:$src)
    , []
    , "mov.i8\t$dst, $src"
    , 0x09
    >;

  def MOV_I16 : I
    < (outs I16:$dst)
    , (ins I16:$src)
    , []
    , "mov.i16\t$dst, $src"
    , 0x09
    >;

  def MOV_I32 : I
    < (outs I32:$dst)
    , (ins I32:$src)
    , []
    , "mov.i32\t$dst, $src"
    , 0x09
    >;

  def MOV_I64 : I
    < (outs I64:$dst)
    , (ins GI64:$src)
    , []
    , "mov.i64\t$dst, $src"
    , 0x0A
    >;

  def MOV_F32 : I
    < (outs F32:$dst)
    , (ins F32:$src)
    , []
    , "mov.f32\t$dst, $src"
    , 0x0B
    >;

  def MOV_F64 : I
    < (outs F64:$dst)
    , (ins F64:$src)
    , []
    , "mov.f64\t$dst, $src"
    , 0x0C
    >;

  def MOV_F80 : I
    < (outs F80:$dst)
    , (ins F80:$src)
    , []
    , "mov.f80\t$dst, $src"
    , 0x0C
    >;

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

  def UNDEF_I8 : I
      < (outs I8:$res)
      , (ins)
      , [(set I8:$res, undef)]
      , "undef.i8\t$res"
      , 0x11
      >;

  def UNDEF_I16 : I
      < (outs I16:$res)
      , (ins)
      , [(set I16:$res, undef)]
      , "undef.i16\t$res"
      , 0x11
      >;

  def UNDEF_I32 : I
      < (outs I32:$res)
      , (ins)
      , [(set I32:$res, undef)]
      , "undef.i32\t$res"
      , 0x11
      >;

  def UNDEF_I64 : I
      < (outs I64:$res)
      , (ins)
      , [(set I64:$res, undef)]
      , "undef.i64\t$res"
      , 0x12
      >;

  def UNDEF_F32 : I
      < (outs F32:$res)
      , (ins)
      , [(set F32:$res, undef)]
      , "undef.f32\t$res"
      , 0x13
      >;

  def UNDEF_F64 : I
      < (outs F64:$res)
      , (ins)
      , [(set F64:$res, undef)]
      , "undef.f64\t$res"
      , 0x14
      >;

  def UNDEF_F80 : I
      < (outs F80:$res)
      , (ins)
      , [(set F80:$res, undef)]
      , "undef.f80\t$res"
      , 0x14
      >;
}

def : Pat
  < (LLIRsymbol tglobaladdr:$addr)
  , (ADDR_I64 i64addr_op:$addr)
  >;

def : Pat
  < (LLIRsymbol texternalsym:$addr)
  , (ADDR_I64 i64addr_op:$addr)
  >;

//===----------------------------------------------------------------------===//
// Special instructions with side effects.
//===----------------------------------------------------------------------===//

let hasSideEffects = 1 in
{
  def VASTART : I
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

  def RDTSC_I64 : I
      < (outs I64:$res)
      , (ins)
      , [(set I64:$res, LLIRrdtsc)]
      , "rdtsc.i64\t$res"
      , 0x15
    >;

  def GET_I64 : I
    < (outs I64:$dst)
    , (ins R64:$src)
    , []
    , "mov.i64\t$dst, $src"
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

//===----------------------------------------------------------------------===//
// Additional sets of instructions.
//===----------------------------------------------------------------------===//

include "LLIRInstrArg.td"
include "LLIRInstrAtomic.td"
include "LLIRInstrCall.td"
include "LLIRInstrConst.td"
include "LLIRInstrConv.td"
include "LLIRInstrMemory.td"
include "LLIRInstrControl.td"
include "LLIRInstrInteger.td"
include "LLIRInstrFloat.td"

