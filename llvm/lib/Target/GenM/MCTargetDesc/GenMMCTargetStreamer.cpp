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

void GenMMCTargetAsmStreamer::emitVAIndex(int Index)
{
  OS << "\t.va_idx\t" << Index << '\n';
}

void GenMMCTargetAsmStreamer::emitCallingConv(CallingConv::ID CallConv)
{
  switch (CallConv) {
    case CallingConv::C:    OS << "\t.call c\n";    break;
    case CallingConv::Fast: OS << "\t.call fast\n"; break;
    default: llvm_unreachable("not implemented");
  }
}

GenMMCTargetELFStreamer::GenMMCTargetELFStreamer(MCStreamer &S)
  : GenMMCTargetStreamer(S)
{
}

void GenMMCTargetELFStreamer::emitStackSize(int Size)
{
  llvm_unreachable("not implemented");
}

void GenMMCTargetELFStreamer::emitVAIndex(int Index)
{
  llvm_unreachable("not implemented");
}

void GenMMCTargetELFStreamer::emitCallingConv(CallingConv::ID CallConv)
{
  llvm_unreachable("not implemented");
}

MCELFStreamer &GenMMCTargetELFStreamer::getStreamer()
{
  return static_cast<MCELFStreamer &>(Streamer);
}
