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
