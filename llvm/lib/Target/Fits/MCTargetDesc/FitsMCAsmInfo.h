#ifndef LLVM_LIB_TARGET_FITS_MCTARGETDESC_FITSMCASMINFO_H
#define LLVM_LIB_TARGET_FITS_MCTARGETDESC_FITSMCASMINFO_H

#include "llvm/MC/MCAsmInfo.h"

namespace llvm {
class Triple;
class FitsMCAsmInfo final : public MCAsmInfo {
public:
  explicit FitsMCAsmInfo(const Triple &TheTriple);
};
} // end namespace llvm

#endif