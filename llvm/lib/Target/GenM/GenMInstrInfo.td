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
def SDT_GenMsymbol   : SDTypeProfile<1,  1, [SDTCisSameAs<0, 1>, SDTCisPtrTy<0>]>;
def SDT_GenMswitch   : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;
def SDT_GenMvastart  : SDTypeProfile<0,  1, [SDTCisPtrTy<0>]>;
def SDT_GenMvalue    : SDTypeProfile<1,  0, []>;

def SDT_GenMcall     : SDTypeProfile<1, -2, [SDTCisPtrTy<1>, SDTCisVT<2, i32>]>;
def SDT_GenMcall_va  : SDTypeProfile<1, -3, [SDTCisPtrTy<1>, SDTCisVT<2, i32>, SDTCisVT<3, i32>]>;
def SDT_GenMtcall    : SDTypeProfile<1, -2, [SDTCisPtrTy<1>, SDTCisVT<2, i32>]>;
def SDT_GenMtcall_va : SDTypeProfile<1, -3, [SDTCisPtrTy<1>, SDTCisVT<2, i32>, SDTCisVT<3, i32>]>;

def SDT_GenMvoid     : SDTypeProfile<0, -1, [SDTCisPtrTy<0>, SDTCisVT<1, i32>]>;
def SDT_GenMvoid_va  : SDTypeProfile<0, -2, [SDTCisPtrTy<0>, SDTCisVT<1, i32>, SDTCisVT<2, i32>]>;
def SDT_GenMtvoid    : SDTypeProfile<0, -1, [SDTCisPtrTy<0>, SDTCisVT<1, i32>]>;
def SDT_GenMtvoid_va : SDTypeProfile<0, -2, [SDTCisPtrTy<0>, SDTCisVT<1, i32>, SDTCisVT<2, i32>]>;

def SDT_GenMover     : SDTypeProfile<1,  2, [SDTCisSameAs<1, 2>]>;

def SDT_GenMalloca   : SDTypeProfile<1,  2, [SDTCisInt<1>, SDTCisVT<2, i64>]>;

//===----------------------------------------------------------------------===//
// GenM-specific DAG Nodes.
//===----------------------------------------------------------------------===//

def GenMargument  : SDNode<"GenMISD::ARGUMENT", SDT_GenMargument>;
def GenMreturn    : SDNode<"GenMISD::RETURN",   SDT_GenMreturn, [SDNPHasChain]>;
def GenMsymbol    : SDNode<"GenMISD::SYMBOL",   SDT_GenMsymbol>;
def GenMswitch    : SDNode<"GenMISD::SWITCH",   SDT_GenMswitch, [SDNPHasChain, SDNPVariadic]>;
def GenMvastart   : SDNode<"GenMISD::VASTART",  SDT_GenMvastart, [SDNPHasChain]>;

def GenMcall      : SDNode<"GenMISD::CALL",     SDT_GenMcall,     [SDNPHasChain, SDNPVariadic]>;
def GenMcall_va   : SDNode<"GenMISD::CALL",     SDT_GenMcall_va,  [SDNPHasChain, SDNPVariadic]>;
def GenMtcall     : SDNode<"GenMISD::TCALL",    SDT_GenMtcall,    [SDNPHasChain, SDNPVariadic]>;
def GenMtcall_va  : SDNode<"GenMISD::TCALL",    SDT_GenMtcall_va, [SDNPHasChain, SDNPVariadic]>;
def GenMvoid      : SDNode<"GenMISD::VOID",     SDT_GenMvoid,     [SDNPHasChain, SDNPVariadic]>;
def GenMvoid_va   : SDNode<"GenMISD::VOID",     SDT_GenMvoid_va,  [SDNPHasChain, SDNPVariadic]>;
def GenMtvoid     : SDNode<"GenMISD::TVOID",    SDT_GenMtvoid,    [SDNPHasChain, SDNPVariadic]>;
def GenMtvoid_va  : SDNode<"GenMISD::TVOID",    SDT_GenMtvoid_va, [SDNPHasChain, SDNPVariadic]>;

def GenMsaddo     : SDNode<"GenMISD::SADDO",    SDT_GenMover>;
def GenMuaddo     : SDNode<"GenMISD::UADDO",    SDT_GenMover>;
def GenMssubo     : SDNode<"GenMISD::SSUBO",    SDT_GenMover>;
def GenMusubo     : SDNode<"GenMISD::USUBO",    SDT_GenMover>;
def GenMsmulo     : SDNode<"GenMISD::SMULO",    SDT_GenMover>;
def GenMumulo     : SDNode<"GenMISD::UMULO",    SDT_GenMover>;

def GenMrdtsc     : SDNode<"GenMISD::RDTSC",    SDT_GenMvalue, [SDNPHasChain]>;

def GenMalloca    : SDNode<"GenMISD::ALLOCA",   SDT_GenMalloca, [SDNPHasChain]>;

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
def f80imm_op  : GenMOperand<f80, "OPERAND_F80IMM">;

//===----------------------------------------------------------------------===//
// GenM Instruction Format Definitions.
//===----------------------------------------------------------------------===//

include "GenMInstrFormats.td"

//===----------------------------------------------------------------------===//
// Additional instructions.
//===----------------------------------------------------------------------===//

def VASTART : I
  < (outs)
  , (ins I64:$frame)
  , [(GenMvastart I64:$frame)]
  , "vastart\t$frame"
  , 0x00
  >
{
  let hasSideEffects = 1;
}

let isAsCheapAsAMove = 1, isReMaterializable = 1 in
{
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

  def ALLOCA_I64 : I
    < (outs I64:$res)
    , (ins I64:$size, i64imm:$align)
    , [(set I64:$res, (GenMalloca I64:$size, imm:$align))]
    , "alloca.i64\t$res, $size, $align"
    , 0x0F
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

  def RDTSC_I64 : I
      < (outs I64:$res)
      , (ins)
      , [(set I64:$res, GenMrdtsc)]
      , "rdtsc.i64\t$res"
      , 0x15
      >;
}

def : Pat
  < (GenMsymbol tglobaladdr:$addr)
  , (ADDR_I64 i64addr_op:$addr)
  >;

def : Pat
  < (GenMsymbol texternalsym:$addr)
  , (ADDR_I64 i64addr_op:$addr)
  >;

//===----------------------------------------------------------------------===//
// Additional sets of instructions.
//===----------------------------------------------------------------------===//

include "GenMInstrArg.td"
include "GenMInstrAtomic.td"
include "GenMInstrCall.td"
include "GenMInstrConst.td"
include "GenMInstrConv.td"
include "GenMInstrMemory.td"
include "GenMInstrControl.td"
include "GenMInstrInteger.td"
include "GenMInstrFloat.td"

