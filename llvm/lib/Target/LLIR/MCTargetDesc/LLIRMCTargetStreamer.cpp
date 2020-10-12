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



LLIRMCTargetStreamer::LLIRMCTargetStreamer(MCStreamer &S)
  : MCTargetStreamer(S)
{

}

//===----------------------------------------------------------------------===//
// LLIRMCTargetAsmStreamer
//===----------------------------------------------------------------------===//

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

static StringRef LLIRTypeName(MVT VT)
{
  switch (VT.SimpleTy) {
    case MVT::i8:  return "i8";
    case MVT::i16: return "i16";
    case MVT::i32: return "i32";
    case MVT::i64: return "i64";
    case MVT::f32: return "f32";
    case MVT::f64: return "f64";
    case MVT::f80: return "f80";
    default: llvm_unreachable("not implemented");
  }
}

void LLIRMCTargetAsmStreamer::emitVarArg()
{
  OS << "\t.vararg\n";
}

void LLIRMCTargetAsmStreamer::emitParams(ArrayRef<MVT> params)
{
  OS << "\t.args\t";
  for (unsigned i = 0, n = params.size(); i < n; ++i) {
    if (i != 0)
      OS << ", ";
    OS << LLIRTypeName(params[i]);
  }
  OS << "\n";
}

void LLIRMCTargetAsmStreamer::emitCallingConv(CallingConv::ID CallConv)
{
  switch (CallConv) {
    case CallingConv::C:     OS << "\t.call c\n"; break;
    case CallingConv::Fast:  OS << "\t.call c\n"; break;
    case CallingConv::Win64: OS << "\t.call win64\n"; break;
    default: llvm_unreachable("not implemented");
  }
}

void LLIRMCTargetAsmStreamer::emitCtor(int Priority, const MCSymbol *Sym)
{
  OS << ".ctor " << Priority << ", " << Sym->getName() << "\n";
}

void LLIRMCTargetAsmStreamer::emitDtor(int Priority, const MCSymbol *Sym)
{
  OS << ".dtor " << Priority << ", " << Sym->getName() << "\n";
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
  llvm_unreachable("LLIRMCTargetLLIRStreamer::emitStackSize");
}

void LLIRMCTargetLLIRStreamer::emitVarArg()
{
  llvm_unreachable("LLIRMCTargetLLIRStreamer::emitVarArg");
}

void LLIRMCTargetLLIRStreamer::emitParams(ArrayRef<MVT> params)
{
  llvm_unreachable("LLIRMCTargetLLIRStreamer::emitParams");
}

void LLIRMCTargetLLIRStreamer::emitCallingConv(CallingConv::ID CallConv)
{
  llvm_unreachable("LLIRMCTargetLLIRStreamer::emitCallingConv");
}

void LLIRMCTargetLLIRStreamer::emitCtor(int Priority, const MCSymbol *Sym)
{
  llvm_unreachable("not implemented");
}

void LLIRMCTargetLLIRStreamer::emitDtor(int Priority, const MCSymbol *Sym)
{
  llvm_unreachable("not implemented");
}

void LLIRMCTargetLLIRStreamer::emitEnd()
{
  llvm_unreachable("LLIRMCTargetLLIRStreamer::emitEnd");
}

void LLIRMCTargetLLIRStreamer::emitNoInline()
{
  llvm_unreachable("LLIRMCTargetLLIRStreamer::emitNoInline");
}
