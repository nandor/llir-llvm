//===-- GenM.td - Describe the GenM Target Machine ---------*- tablegen -*-===//
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

include "GenMRegisterInfo.td"
include "GenMInstrInfo.td"

def GenMInstrInfo : InstrInfo;

def : Processor<"penryn", NoItineraries, []>;
def : Processor<"x86-64", NoItineraries, []>;


//===----------------------------------------------------------------------===//
// GenM underlying processors supported.
//===----------------------------------------------------------------------===//


//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def GenMAsmWriter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  int PassSubtarget = 1;
  int Variant = 0;
}

def GenM : Target {
  // Pull in Instruction Info:
  let InstructionSet = GenMInstrInfo;
  let AssemblyParsers  = [];
  let AssemblyWriters = [GenMAsmWriter];
  let AllowRegisterRenaming = 1;
}
