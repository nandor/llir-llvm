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

void GenMMCTargetAsmStreamer::emitStackSize(int Size)
{
  OS << "\t.stack\t" << Size << '\n';
}

void GenMMCTargetAsmStreamer::emitParams(ArrayRef<MVT> params, bool IsVA)
{
  OS << "\t.args\t" << IsVA;
  for (const auto &VT : params) {
    OS << ", ";
    switch (VT.SimpleTy) {
      case MVT::i32: OS << "i32"; break;
      case MVT::i64: OS << "i64"; break;
      case MVT::f32: OS << "f32"; break;
      case MVT::f64: OS << "f64"; break;
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

GenMMCTargetELFStreamer::GenMMCTargetELFStreamer(MCStreamer &S)
  : GenMMCTargetStreamer(S)
{
}

void GenMMCTargetELFStreamer::emitStackSize(int Size)
{
  llvm_unreachable("not implemented");
}

void GenMMCTargetELFStreamer::emitParams(ArrayRef<MVT> params, bool IsVA)
{
  llvm_unreachable("not implemented");
}

void GenMMCTargetELFStreamer::emitCallingConv(CallingConv::ID CallConv)
{
  llvm_unreachable("not implemented");
}

void GenMMCTargetELFStreamer::emitEnd()
{
  llvm_unreachable("not implemented");
}

MCELFStreamer &GenMMCTargetELFStreamer::getStreamer()
{
  return static_cast<MCELFStreamer &>(Streamer);
}
