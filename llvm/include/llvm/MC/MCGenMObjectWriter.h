//===-- llvm/MC/MCGenMObjectWriter.h - GenM Object Writer -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_MC_MCGENMOBJECTWRITER_H
#define LLVM_MC_MCGENMOBJECTWRITER_H

#include "llvm/MC/MCObjectWriter.h"
#include <memory>

namespace llvm {

class MCFixup;
class MCValue;
class raw_pwrite_stream;

class MCGenMObjectTargetWriter : public MCObjectTargetWriter {
protected:
  explicit MCGenMObjectTargetWriter();

public:
  virtual ~MCGenMObjectTargetWriter();

  virtual Triple::ObjectFormatType getFormat() const { return Triple::GenM; }
  static bool classof(const MCObjectTargetWriter *W) {
    return W->getFormat() == Triple::GenM;
  }

  virtual unsigned getRelocType(
      const MCValue &Target,
      const MCFixup &Fixup
  ) const = 0;
};

/// Construct a new GenM writer instance.
///
/// \param MOTW - The target specific GenM writer subclass.
/// \param OS - The stream to write to.
/// \returns The constructed object writer.
std::unique_ptr<MCObjectWriter>
createGenMObjectWriter(std::unique_ptr<MCGenMObjectTargetWriter> MOTW,
                       raw_pwrite_stream &OS);

} // namespace llvm

#endif
