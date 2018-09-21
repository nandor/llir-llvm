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


// GenM Instruction Format
class GenMInst<string cstr> : Instruction {
  field bits<32> Inst;
  let Namespace = "GenM";
}

class I<dag oops, dag iops, string asm, string operands, string cstr, list<dag> pattern>
  : GenMInst<cstr> {
  let Size = 4;
  let Pattern = pattern;
  dag OutOperandList = oops;
  dag InOperandList  = iops;
  let AsmString      = !strconcat(asm, operands);
}

class AsmImmRange<int Low, int High> : AsmOperandClass {
  let Name = "Imm" # Low # "_" # High;
  let DiagnosticType = "InvalidImm" # Low # "_" # High;
  let RenderMethod = "addImmOperands";
  let PredicateMethod = "isImmInRange<" # Low # "," # High # ">";
}

def Imm0_65535Operand : AsmImmRange<0, 65535>;

def movimm32_imm : Operand<i32> {
  let ParserMatchClass = Imm0_65535Operand;
  let EncoderMethod = "getMoveWideImmOpValue";
}

class BaseInsertImmediate<bits<2> opc, RegisterClass regtype, string asm>
  : I<(outs regtype:$Rd),
      (ins regtype:$src, movimm32_imm:$imm),
       asm, "\t$Rd, $imm", "$src = $Rd", []> {
  bits<5> Rd;
  bits<16> imm;
  bits<6> shift;

  let Inst{30-29} = opc;
  let Inst{28-23} = 0b100101;
  let Inst{22-21} = shift{5-4};
  let Inst{20-5}  = imm;
  let Inst{4-0}   = Rd;

  let DecoderMethod = "DecodeMoveImmInstruction";
}

def MOVK : BaseInsertImmediate<0b11, GPR, "movk">;
