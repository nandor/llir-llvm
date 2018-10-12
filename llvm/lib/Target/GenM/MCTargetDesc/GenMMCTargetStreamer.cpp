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

GenMMCTargetELFStreamer::GenMMCTargetELFStreamer(MCStreamer &S)
  : GenMMCTargetStreamer(S)
{
}

void GenMMCTargetELFStreamer::emitStackSize(int Size)
{
  llvm_unreachable("not implemented");
}

MCELFStreamer &GenMMCTargetELFStreamer::getStreamer()
{
  return static_cast<MCELFStreamer &>(Streamer);
}
