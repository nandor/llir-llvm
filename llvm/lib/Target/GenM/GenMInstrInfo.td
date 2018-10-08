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
def SDT_GenMcall     : SDTypeProfile<1, -1, [SDTCisPtrTy<0>]>;
def SDT_GenMvoid     : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;


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
def GenMvoid : SDNode
  < "GenMISD::VOID"
  , SDT_GenMvoid
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
def i32imm_op  : GenMOperand<i32, "OPERAND_I32IMM">;
def i64imm_op  : GenMOperand<i64, "OPERAND_I64IMM">;
def i32off_op  : GenMOperand<i32, "OPERAND_I32OFF">;
def i64func_op : GenMOperand<i64, "OPERAND_I64FUNC">;

//===----------------------------------------------------------------------===//
// GenM Instruction Format Definitions.
//===----------------------------------------------------------------------===//

include "GenMInstrFormats.td"

//===----------------------------------------------------------------------===//
// Additional instructions.
//===----------------------------------------------------------------------===//

class ARGUMENT<GenMRegClass vt>
  : I
    < (outs vt:$res)
    , (ins i32imm:$argno)
    , [(set vt:$res, (GenMargument timm:$argno))]
    >
{
  let hasSideEffects = 1;
  let Uses = [ARGS];
  let isCodeGenOnly = 1;
}

def ARG_I32 : ARGUMENT<I32>;
def ARG_I64 : ARGUMENT<I64>;

let isMoveImm = 1, isAsCheapAsAMove = 1, isReMaterializable = 1 in
{
  def CONST_I32 : I
    < (outs I32:$res)
    , (ins i32imm_op:$imm)
    , [(set I32:$res, imm:$imm)]
    , "imm.i32 \t$res, $imm"
    , 0x00
    >;

  def CONST_I64 : I
    < (outs I64:$res)
    , (ins i64imm_op:$imm)
    , [(set I64:$res, imm:$imm)]
    , "imm.i64 \t$res, $imm"
    , 0x01
    >;
}

//===----------------------------------------------------------------------===//
// Additional sets of instructions.
//===----------------------------------------------------------------------===//

include "GenMInstrCall.td"
include "GenMInstrMemory.td"
include "GenMInstrControl.td"
include "GenMInstrInteger.td"

