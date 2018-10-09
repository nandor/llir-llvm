//=- GenMInstrFormats.td - GenM Instr. Formats ---------------*- tablegen -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// GenM instruction format definitions.
///
//===----------------------------------------------------------------------===//

class GenMInst<bits<32> inst, string asmstr> : Instruction
{
  field bits<32> Inst = inst;
  let Namespace = "GenM";
  let AsmString = asmstr;
}

class I
  < dag oops
  , dag iops
  , list<dag> pattern
  , string asmstr = ""
  , bits<32> inst = -1
  >
  : GenMInst<inst, asmstr>
{
  let Size           = 4;
  dag OutOperandList = oops;
  dag InOperandList  = iops;
  let Pattern        = pattern;
}

class TerminatorInst
  < dag iops
  , list<dag> pattern
  , string asmstr
  , bits<32> inst
  >
  : I<(outs), iops, pattern, asmstr, inst>
{
  let isTerminator = 1;
  let hasCtrlDep = 1;
  let isBarrier = 1;
}

class BranchInst
  < dag iops
  , list<dag> pattern
  , string asmstr
  , bits<32> inst
  >
  : TerminatorInst<iops, pattern, asmstr, inst>
{
  let isBranch = 1;
}

class CallInst
  < dag oops
  , dag iops
  , list<dag> pattern
  , string asmstr
  , bits<32> inst
  >
  : I<oops, iops, pattern, asmstr, inst>
{
  let isCall = 1;
}
