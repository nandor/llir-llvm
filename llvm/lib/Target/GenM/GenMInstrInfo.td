//===-- GenMInstrInfo.td - Target Description for GenM Target -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the GenM instructions in TableGen format.
//
//===----------------------------------------------------------------------===//


//===----------------------------------------------------------------------===//
// GenM-specific DAG Node Types.
//===----------------------------------------------------------------------===//

def SDT_GenMargument : SDTypeProfile<1,  1, [SDTCisVT<1, i32>]>;
def SDT_GenMreturn   : SDTypeProfile<0, -1, []>;
def SDT_GenMcall     : SDTypeProfile<1, -2, [SDTCisPtrTy<1>, SDTCisVT<2, i32>]>;
def SDT_GenMcall_va  : SDTypeProfile<1, -3, [SDTCisPtrTy<1>, SDTCisVT<2, i32>, SDTCisVT<3, i32>]>;
def SDT_GenMvoid     : SDTypeProfile<0, -1, [SDTCisPtrTy<0>, SDTCisVT<1, i32>]>;
def SDT_GenMvoid_va  : SDTypeProfile<0, -2, [SDTCisPtrTy<0>, SDTCisVT<1, i32>, SDTCisVT<2, i32>]>;
def SDT_GenMtcall    : SDTypeProfile<0, -1, [SDTCisPtrTy<0>, SDTCisVT<1, i32>]>;
def SDT_GenMtcall_va : SDTypeProfile<0, -2, [SDTCisPtrTy<0>, SDTCisVT<1, i32>, SDTCisVT<2, i32>]>;
def SDT_GenMbrjt     : SDTypeProfile<0,  2, [SDTCisPtrTy<0>, SDTCisVT<1, i32>]>;
def SDT_GenMsymbol   : SDTypeProfile<1,  1, [SDTCisSameAs<0, 1>, SDTCisPtrTy<0>]>;
def SDT_GenMswitch   : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;

//===----------------------------------------------------------------------===//
// GenM-specific DAG Nodes.
//===----------------------------------------------------------------------===//

def GenMargument : SDNode
  < "GenMISD::ARGUMENT"
  , SDT_GenMargument
  >;
def GenMreturn : SDNode
  < "GenMISD::RETURN"
  , SDT_GenMreturn
  , [SDNPHasChain]
  >;
def GenMcall : SDNode
  < "GenMISD::CALL"
  , SDT_GenMcall
  , [SDNPHasChain, SDNPVariadic]
  >;
def GenMcall_va : SDNode
  < "GenMISD::CALL"
  , SDT_GenMcall_va
  , [SDNPHasChain, SDNPVariadic]
  >;
def GenMtcall : SDNode
  < "GenMISD::TCALL"
  , SDT_GenMtcall
  , [SDNPHasChain, SDNPVariadic]
  >;
def GenMtcall_va : SDNode
  < "GenMISD::TCALL"
  , SDT_GenMtcall_va
  , [SDNPHasChain, SDNPVariadic]
  >;
def GenMvoid : SDNode
  < "GenMISD::VOID"
  , SDT_GenMvoid
  , [SDNPHasChain, SDNPVariadic]
  >;
def GenMvoid_va : SDNode
  < "GenMISD::VOID"
  , SDT_GenMvoid_va
  , [SDNPHasChain, SDNPVariadic]
  >;
def GenMsymbol : SDNode
  < "GenMISD::SYMBOL"
  , SDT_GenMsymbol
  >;
def GenMswitch : SDNode
  < "GenMISD::SWITCH"
  , SDT_GenMswitch
  , [SDNPHasChain, SDNPVariadic]
  >;

//===----------------------------------------------------------------------===//
// WebAssembly-specific Operands.
//===----------------------------------------------------------------------===//

class GenMOperand<ValueType ty, string type> : Operand<ty>
{
  let OperandNamespace = "GenM";
  let OperandType = type;
}


def bb_op      : GenMOperand<OtherVT, "OPERAND_BASIC_BLOCK">;
def i8imm_op   : GenMOperand<i8,  "OPERAND_I8IMM">;
def i32imm_op  : GenMOperand<i32, "OPERAND_I32IMM">;
def i64imm_op  : GenMOperand<i64, "OPERAND_I64IMM">;
def i32off_op  : GenMOperand<i32, "OPERAND_I32OFF">;
def i64addr_op : GenMOperand<i64, "OPERAND_I64FUNC">;
def f32imm_op  : GenMOperand<f32, "OPERAND_F32IMM">;
def f64imm_op  : GenMOperand<f64, "OPERAND_F64IMM">;

//===----------------------------------------------------------------------===//
// GenM Instruction Format Definitions.
//===----------------------------------------------------------------------===//

include "GenMInstrFormats.td"

//===----------------------------------------------------------------------===//
// Additional instructions.
//===----------------------------------------------------------------------===//

let isAsCheapAsAMove = 1, isReMaterializable = 1 in
{
  def ARG_I32 : I
      < (outs I32:$res)
      , (ins i32imm:$argno)
      , [(set I32:$res, (GenMargument timm:$argno))]
      , "arg.i32\t$res, $argno"
      , 0x01
      >;

  def ARG_I64 : I
      < (outs I64:$res)
      , (ins i32imm:$argno)
      , [(set I64:$res, (GenMargument timm:$argno))]
      , "arg.i64\t$res, $argno"
      , 0x02
      >;

  def ARG_F32 : I
      < (outs F32:$res)
      , (ins i32imm:$argno)
      , [(set F32:$res, (GenMargument timm:$argno))]
      , "arg.f32\t$res, $argno"
      , 0x03
      >;

  def ARG_F64 : I
      < (outs F64:$res)
      , (ins i32imm:$argno)
      , [(set F64:$res, (GenMargument timm:$argno))]
      , "arg.f64\t$res, $argno"
      , 0x04
      >;

  let isMoveImm = 1 in
  {
    def CONST_I32 : I
      < (outs I32:$res)
      , (ins i32imm_op:$imm)
      , [(set I32:$res, imm:$imm)]
      , "imm.i32\t$res, $imm"
      , 0x05
      >;

    def CONST_I64 : I
      < (outs I64:$res)
      , (ins i64imm_op:$imm)
      , [(set I64:$res, imm:$imm)]
      , "imm.i64\t$res, $imm"
      , 0x06
      >;

    def CONST_F32 : I
      < (outs F32:$res)
      , (ins f32imm_op:$imm)
      , [(set F32:$res, fpimm:$imm)]
      , "imm.f32\t$res, $imm"
      , 0x07
      >;

    def CONST_F64 : I
      < (outs F64:$res)
      , (ins f64imm_op:$imm)
      , [(set F64:$res, fpimm:$imm)]
      , "imm.f64\t$res, $imm"
      , 0x08
      >;
  }

  def COPY_I32 : I
    < (outs I32:$dst)
    , (ins G32:$src)
    , []
    , "mov.i32\t$dst, $src"
    , 0x09
    >;

  def COPY_I64 : I
    < (outs I64:$dst)
    , (ins G64:$src)
    , []
    , "mov.i64\t$dst, $src"
    , 0x0A
    >;

  def COPY_F32 : I
    < (outs F32:$dst)
    , (ins F32:$src)
    , []
    , "mov.f32\t$dst, $src"
    , 0x0B
    >;

  def COPY_F64 : I
    < (outs F64:$dst)
    , (ins F64:$src)
    , []
    , "mov.f64\t$dst, $src"
    , 0x0C
    >;

  def ADDR : I
    < (outs I64:$dst)
    , (ins i64addr_op:$addr)
    , []
    , "addr.i64\t$dst, $addr"
    , 0x0D
    >;

  def FRAME_I64 : I
    < (outs I64:$res)
    , (ins i32imm:$offset)
    , []
    , "frame.i64\t$res, $offset"
    , 0x0E
    >;
}


def : Pat
  < (GenMsymbol tglobaladdr:$addr)
  , (ADDR i64addr_op:$addr)
  >;

def : Pat
  < (GenMsymbol texternalsym:$addr)
  , (ADDR i64addr_op:$addr)
  >;

//===----------------------------------------------------------------------===//
// Additional sets of instructions.
//===----------------------------------------------------------------------===//

include "GenMInstrAtomic.td"
include "GenMInstrCall.td"
include "GenMInstrConv.td"
include "GenMInstrMemory.td"
include "GenMInstrControl.td"
include "GenMInstrInteger.td"
include "GenMInstrFloat.td"

