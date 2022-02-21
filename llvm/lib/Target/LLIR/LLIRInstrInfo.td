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
// LLIR subtarget predicates.
//===----------------------------------------------------------------------===//

def IsX86         : Predicate<"Subtarget->isX86()">;
def IsX86_32      : Predicate<"Subtarget->isX86_32()">;
def IsX86_64      : Predicate<"Subtarget->isX86_64()">;
def IsAArch64     : Predicate<"Subtarget->isAArch64()">;
def IsRISCV       : Predicate<"Subtarget->isRISCV()">;
def IsPPC64le     : Predicate<"Subtarget->isPPC64le()">;
def Is32Bit       : Predicate<"Subtarget->is32Bit()">;
def Is64Bit       : Predicate<"Subtarget->is64Bit()">;


//===----------------------------------------------------------------------===//
// LLIR-specific DAG Node Types.
//===----------------------------------------------------------------------===//

def SDT_LLIRargument     : SDTypeProfile<1,  1, [SDTCisVT<1, i32>]>;
def SDT_LLIRsymbol       : SDTypeProfile<1,  1, [SDTCisSameAs<0, 1>, SDTCisPtrTy<0>]>;
def SDT_LLIRswitch       : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;
def SDT_LLIRvastart      : SDTypeProfile<0,  1, [SDTCisPtrTy<0>]>;
def SDT_LLIRvalue        : SDTypeProfile<1,  0, []>;
def SDT_LLIRalloca       : SDTypeProfile<1,  2, [SDTCisInt<1>, SDTCisVT<2, i64>]>;
def SDT_LLIRbarrier      : SDTypeProfile<0,  0, []>;

def LLIRreturn      : SDNode<"LLIRISD::RETURN",          SDTNone,             [SDNPHasChain, SDNPVariadic]>;
def LLIRargument    : SDNode<"LLIRISD::ARGUMENT",        SDT_LLIRargument>;
def LLIRsymbol      : SDNode<"LLIRISD::SYMBOL",          SDT_LLIRsymbol>;
def LLIRswitch      : SDNode<"LLIRISD::SWITCH",          SDT_LLIRswitch,      [SDNPHasChain, SDNPVariadic]>;
def LLIRvastart     : SDNode<"LLIRISD::VASTART",         SDT_LLIRvastart,     [SDNPHasChain]>;
def LLIRrdtsc       : SDNode<"LLIRISD::RDTSC",           SDT_LLIRvalue,       [SDNPHasChain]>;
def LLIRalloca      : SDNode<"LLIRISD::ALLOCA",          SDT_LLIRalloca,      [SDNPHasChain]>;
def LLIRBarrier     : SDNode<"LLIRISD::BARRIER",         SDT_LLIRbarrier,     [SDNPHasChain, SDNPSideEffect]>;
def LLIRMFence      : SDNode<"LLIRISD::MFENCE",          SDT_LLIRbarrier,     [SDNPHasChain]>;

//===----------------------------------------------------------------------===//
// Overflow-specific DAG Nodes.
//===----------------------------------------------------------------------===//

def SDT_LLIRover     : SDTypeProfile<1,  2, [SDTCisSameAs<1, 2>]>;

def LLIRsaddo : SDNode<"LLIRISD::SADDO", SDT_LLIRover>;
def LLIRuaddo : SDNode<"LLIRISD::UADDO", SDT_LLIRover>;
def LLIRssubo : SDNode<"LLIRISD::SSUBO", SDT_LLIRover>;
def LLIRusubo : SDNode<"LLIRISD::USUBO", SDT_LLIRover>;
def LLIRsmulo : SDNode<"LLIRISD::SMULO", SDT_LLIRover>;
def LLIRumulo : SDNode<"LLIRISD::UMULO", SDT_LLIRover>;

//===----------------------------------------------------------------------===//
// Call-specific DAG Nodes.
//===----------------------------------------------------------------------===//

def SDT_LLIRcall      : SDTypeProfile<1, -1, [SDTCisVT<1, i32>]>;
def SDT_LLIRcall_va   : SDTypeProfile<1, -2, [SDTCisVT<1, i32>, SDTCisVT<2, i32>]>;
def SDT_LLIRvoid      : SDTypeProfile<0, -1, [SDTCisVT<0, i32>]>;
def SDT_LLIRvoid_va   : SDTypeProfile<0, -2, [SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_LLIRtcall     : SDTypeProfile<1, -1, [SDTCisVT<1, i32>]>;
def SDT_LLIRtcall_va  : SDTypeProfile<1, -2, [SDTCisVT<1, i32>, SDTCisVT<2, i32>]>;
def SDT_LLIRtvoid     : SDTypeProfile<0, -1, [SDTCisVT<0, i32>]>;
def SDT_LLIRtvoid_va  : SDTypeProfile<0, -2, [SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_LLIRinvoke    : SDTypeProfile<1, -2, [SDTCisVT<1, i32>, SDTCisVT<2, OtherVT>]>;
def SDT_LLIRinvoke_va : SDTypeProfile<1, -3, [SDTCisVT<1, i32>, SDTCisVT<2, i32>, SDTCisVT<3, OtherVT>]>;
def SDT_LLIRivoid     : SDTypeProfile<0, -2, [SDTCisVT<0, i32>, SDTCisVT<1, OtherVT>]>;
def SDT_LLIRivoid_va  : SDTypeProfile<0, -3, [SDTCisVT<0, i32>, SDTCisVT<1, i32>, SDTCisVT<2, OtherVT>]>;

def LLIRcall      : SDNode<"LLIRISD::CALL",            SDT_LLIRcall,      [SDNPHasChain, SDNPVariadic]>;
def LLIRcall_va   : SDNode<"LLIRISD::CALL_VA",         SDT_LLIRcall_va,   [SDNPHasChain, SDNPVariadic]>;
def LLIRtcall     : SDNode<"LLIRISD::TCALL",           SDT_LLIRtcall,     [SDNPHasChain, SDNPVariadic]>;
def LLIRtcall_va  : SDNode<"LLIRISD::TCALL_VA",        SDT_LLIRtcall_va,  [SDNPHasChain, SDNPVariadic]>;
def LLIRinvoke    : SDNode<"LLIRISD::INVOKE",          SDT_LLIRinvoke,    [SDNPHasChain, SDNPVariadic]>;
def LLIRinvoke_va : SDNode<"LLIRISD::INVOKE_VA",       SDT_LLIRinvoke_va, [SDNPHasChain, SDNPVariadic]>;
def LLIRvoid      : SDNode<"LLIRISD::CALL_VOID",       SDT_LLIRvoid,      [SDNPHasChain, SDNPVariadic]>;
def LLIRvoid_va   : SDNode<"LLIRISD::CALL_VOID_VA",    SDT_LLIRvoid_va,   [SDNPHasChain, SDNPVariadic]>;
def LLIRtvoid     : SDNode<"LLIRISD::TCALL_VOID",      SDT_LLIRtvoid,     [SDNPHasChain, SDNPVariadic]>;
def LLIRtvoid_va  : SDNode<"LLIRISD::TCALL_VOID_VA",   SDT_LLIRtvoid_va,  [SDNPHasChain, SDNPVariadic]>;
def LLIRivoid     : SDNode<"LLIRISD::INVOKE_VOID",     SDT_LLIRivoid,     [SDNPHasChain, SDNPVariadic]>;
def LLIRivoid_va  : SDNode<"LLIRISD::INVOKE_VOID_VA",  SDT_LLIRivoid_va,  [SDNPHasChain, SDNPVariadic]>;


//===----------------------------------------------------------------------===//
// WebAssembly-specific Operands.
//===----------------------------------------------------------------------===//

class LLIROperand<ValueType ty, string type> : Operand<ty>
{
  let OperandNamespace = "LLIR";
  let OperandType = type;
}


def bb_op      : LLIROperand<OtherVT, "OPERAND_BASIC_BLOCK">;

def i8imm_op   : LLIROperand<i8,   "OPERAND_I8IMM">;
def i16imm_op  : LLIROperand<i16,  "OPERAND_I16IMM">;
def i32imm_op  : LLIROperand<i32,  "OPERAND_I32IMM">;
def i64imm_op  : LLIROperand<i64,  "OPERAND_I64IMM">;
def i128imm_op : LLIROperand<i128, "OPERAND_I128IMM">;

def i32off_op  : LLIROperand<i32,  "OPERAND_I32OFF">;
def i64off_op  : LLIROperand<i64,  "OPERAND_I64OFF">;

def i32addr_op : LLIROperand<i32,  "OPERAND_I32ADDR">;
def i64addr_op : LLIROperand<i64,  "OPERAND_I64ADDR">;

def f32imm_op  : LLIROperand<f32,  "OPERAND_F32IMM">;
def f64imm_op  : LLIROperand<f64,  "OPERAND_F64IMM">;
def f80imm_op  : LLIROperand<f80,  "OPERAND_F80IMM">;
def f128imm_op : LLIROperand<f128, "OPERAND_F128IMM">;

//===----------------------------------------------------------------------===//
// LLIR Instruction Format Definitions.
//===----------------------------------------------------------------------===//

include "LLIRInstrFormats.td"

//===----------------------------------------------------------------------===//
// Atomic Helpers
//===----------------------------------------------------------------------===//

class monotonic_store<PatFrag base>
  : PatFrag<(ops node:$ptr, node:$val), (base node:$ptr, node:$val)> {
  let IsAtomic = 1;
  let IsAtomicOrderingMonotonic = 1;
}

class releasing_store<PatFrag base>
  : PatFrag<(ops node:$ptr, node:$val), (base node:$ptr, node:$val)> {
  let IsAtomic = 1;
  let IsAtomicOrderingReleaseOrStronger = 1;
}

class acquiring_load<PatFrag base>
  : PatFrag<(ops node:$ptr), (base node:$ptr)> {
  let IsAtomic = 1;
  let IsAtomicOrderingAcquireOrStronger = 1;
}

class seq_cst_store<PatFrag base>
  : PatFrag<(ops node:$ptr, node:$val), (base node:$ptr, node:$val)> {
  let IsAtomic = 1;
  let IsAtomicOrderingSequentiallyConsistent = 1;
}

//===----------------------------------------------------------------------===//
// Additional sets of instructions.
//===----------------------------------------------------------------------===//

include "LLIRInstrArg.td"
include "LLIRInstrMov.td"
include "LLIRInstrUndef.td"
include "LLIRInstrAddr.td"
include "LLIRInstrCall.td"
include "LLIRInstrConst.td"
include "LLIRInstrConv.td"
include "LLIRInstrMemory.td"
include "LLIRInstrControl.td"
include "LLIRInstrInteger.td"
include "LLIRInstrFloat.td"
include "LLIRInstrX86.td"
include "LLIRInstrAArch64.td"
include "LLIRInstrRISCV.td"
include "LLIRInstrPPC.td"

