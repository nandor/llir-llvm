//===-- llvm/MC/MCLLIRObjectWriter.h - LLIR Object Writer -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_MC_MCLLIROBJECTWRITER_H
#define LLVM_MC_MCLLIROBJECTWRITER_H

#include "llvm/MC/MCObjectWriter.h"
#include <memory>

namespace llvm {

class MCFixup;
class MCValue;
class raw_pwrite_stream;

class MCLLIRObjectTargetWriter : public MCObjectTargetWriter {
protected:
  explicit MCLLIRObjectTargetWriter();

public:
  virtual ~MCLLIRObjectTargetWriter();

  virtual Triple::ObjectFormatType getFormat() const { return Triple::LLIR; }
  static bool classof(const MCObjectTargetWriter *W) {
    return W->getFormat() == Triple::LLIR;
  }

  virtual unsigned getRelocType(
      const MCValue &Target,
      const MCFixup &Fixup
  ) const = 0;
};

/// Construct a new LLIR writer instance.
///
/// \param MOTW - The target specific LLIR writer subclass.
/// \param OS - The stream to write to.
/// \returns The constructed object writer.
std::unique_ptr<MCObjectWriter>
createLLIRObjectWriter(std::unique_ptr<MCLLIRObjectTargetWriter> MOTW,
                       raw_pwrite_stream &OS);

} // namespace llvm

#endif
