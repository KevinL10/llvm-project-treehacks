#ifndef LLVM_LIB_TARGET_FITS_FITSINSTRINFO_H
#define LLVM_LIB_TARGET_FITS_FITSINSTRINFO_H

#include "Fits.h"
#include "FitsRegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "FitsGenInstrInfo.inc"

namespace llvm {

class FitsSubtarget;

class FitsInstrInfo : public FitsGenInstrInfo {
public:
  explicit FitsInstrInfo(const FitsSubtarget &STI);

protected:
  const FitsSubtarget &Subtarget;
};

} // end namespace llvm

#endif