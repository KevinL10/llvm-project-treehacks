#ifndef LLVM_LIB_TARGET_FITS_FITSTARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_FITS_FITSTARGETOBJECTFILE_H

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {
class TargetMachine;
class MCContext;
class FitsTargetObjectFile final : public TargetLoweringObjectFileELF {
public:
  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;
};
} // namespace llvm

#endif