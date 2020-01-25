//===-- GenMMCTargetStreamer.cpp - GenM Target Streamer Methods -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides GenM specific target streamer methods.
//
//===----------------------------------------------------------------------===//

#include "GenMMCTargetStreamer.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

// pin vtable to this file
GenMMCTargetStreamer::GenMMCTargetStreamer(MCStreamer &S)
  : MCTargetStreamer(S)
{

}

GenMMCTargetAsmStreamer::GenMMCTargetAsmStreamer(
    MCStreamer &S,
    formatted_raw_ostream &OS)
  : GenMMCTargetStreamer(S)
  , OS(OS)
{
}

void GenMMCTargetAsmStreamer::emitStackObject(int Offset, int Size, int Align)
{
  OS << "\t.stack_object\t" << Offset << ", " << Size << ", " << Align << "\n";
}

void GenMMCTargetAsmStreamer::emitParams(ArrayRef<MVT> params, bool IsVA)
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

void GenMMCTargetAsmStreamer::emitCallingConv(CallingConv::ID CallConv)
{
  switch (CallConv) {
    case CallingConv::C:    OS << "\t.call c\n";    break;
    case CallingConv::Fast: OS << "\t.call fast\n"; break;
    default: llvm_unreachable("not implemented");
  }
}

void GenMMCTargetAsmStreamer::emitEnd()
{
  OS << "\t.end\n";
}

void GenMMCTargetAsmStreamer::emitNoInline()
{
  OS << "\t.noinline\n";
}

GenMMCTargetGenMStreamer::GenMMCTargetGenMStreamer(MCStreamer &S)
  : GenMMCTargetStreamer(S)
{
}

void GenMMCTargetGenMStreamer::emitStackObject(int Offset, int Size, int Align)
{
  llvm::errs() << "GenMMCTargetGenMStreamer::emitStackSize\n";
}

void GenMMCTargetGenMStreamer::emitParams(ArrayRef<MVT> params, bool IsVA)
{
  llvm::errs() << "GenMMCTargetGenMStreamer::emitParams\n";
}

void GenMMCTargetGenMStreamer::emitCallingConv(CallingConv::ID CallConv)
{
  llvm::errs() << "GenMMCTargetGenMStreamer::emitCallingConv\n";
}

void GenMMCTargetGenMStreamer::emitEnd()
{
  llvm::errs() << "GenMMCTargetGenMStreamer::emitEnd\n";
}

void GenMMCTargetGenMStreamer::emitNoInline()
{
  llvm::errs() << "GenMMCTargetGenMStreamer::emitNoInline\n";
}
