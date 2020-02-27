//===-- LLIR.td - Describe the LLIR Target Machine ---------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"



//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "LLIRRegisterInfo.td"
include "LLIRInstrInfo.td"

def LLIRInstrInfo : InstrInfo;

def : Processor<"penryn",  NoItineraries, []>;
def : Processor<"skylake", NoItineraries, []>;
def : Processor<"x86-64",  NoItineraries, []>;


//===----------------------------------------------------------------------===//
// LLIR underlying processors supported.
//===----------------------------------------------------------------------===//


//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def LLIRAsmWriter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  int PassSubtarget = 1;
  int Variant = 0;
}

def LLIR : Target {
  // Pull in Instruction Info:
  let InstructionSet = LLIRInstrInfo;
  let AssemblyParsers  = [];
  let AssemblyWriters = [LLIRAsmWriter];
  let AllowRegisterRenaming = 1;
}
