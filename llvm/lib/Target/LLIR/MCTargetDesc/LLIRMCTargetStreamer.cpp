//===-- LLIRMCTargetStreamer.cpp - LLIR Target Streamer Methods -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides LLIR specific target streamer methods.
//
//===----------------------------------------------------------------------===//

#include "LLIRMCTargetStreamer.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

// pin vtable to this file
LLIRMCTargetStreamer::LLIRMCTargetStreamer(MCStreamer &S)
  : MCTargetStreamer(S)
{

}

LLIRMCTargetAsmStreamer::LLIRMCTargetAsmStreamer(
    MCStreamer &S,
    formatted_raw_ostream &OS)
  : LLIRMCTargetStreamer(S)
  , OS(OS)
{
}

void LLIRMCTargetAsmStreamer::emitStackObject(int Offset, int Size, int Align)
{
  OS << "\t.stack_object\t" << Offset << ", " << Size << ", " << Align << "\n";
}

void LLIRMCTargetAsmStreamer::emitParams(ArrayRef<MVT> params, bool IsVA)
{
  OS << "\t.args\t" << IsVA;
  for (const auto &VT : params) {
    OS << ", ";
    switch (VT.SimpleTy) {
      case MVT::i8:  OS << "i8";  break;
      case MVT::i16: OS << "i16"; break;
      case MVT::i32: OS << "i32"; break;
      case MVT::i64: OS << "i64"; break;
      case MVT::f32: OS << "f32"; break;
      case MVT::f64: OS << "f64"; break;
      case MVT::f80: OS << "f80"; break;
      default: llvm_unreachable("not implemented");
    }
  }
  OS << "\n";
}

void LLIRMCTargetAsmStreamer::emitCallingConv(CallingConv::ID CallConv)
{
  switch (CallConv) {
    case CallingConv::C:    OS << "\t.call c\n"; break;
    case CallingConv::Fast: OS << "\t.call c\n"; break;
    default: llvm_unreachable("not implemented");
  }
}

void LLIRMCTargetAsmStreamer::emitEnd()
{
  OS << "\t.end\n";
}

void LLIRMCTargetAsmStreamer::emitNoInline()
{
  OS << "\t.noinline\n";
}

LLIRMCTargetLLIRStreamer::LLIRMCTargetLLIRStreamer(MCStreamer &S)
  : LLIRMCTargetStreamer(S)
{
}

void LLIRMCTargetLLIRStreamer::emitStackObject(int Offset, int Size, int Align)
{
  llvm::errs() << "LLIRMCTargetLLIRStreamer::emitStackSize\n";
}

void LLIRMCTargetLLIRStreamer::emitParams(ArrayRef<MVT> params, bool IsVA)
{
  llvm::errs() << "LLIRMCTargetLLIRStreamer::emitParams\n";
}

void LLIRMCTargetLLIRStreamer::emitCallingConv(CallingConv::ID CallConv)
{
  llvm::errs() << "LLIRMCTargetLLIRStreamer::emitCallingConv\n";
}

void LLIRMCTargetLLIRStreamer::emitEnd()
{
  llvm::errs() << "LLIRMCTargetLLIRStreamer::emitEnd\n";
}

void LLIRMCTargetLLIRStreamer::emitNoInline()
{
  llvm::errs() << "LLIRMCTargetLLIRStreamer::emitNoInline\n";
}
