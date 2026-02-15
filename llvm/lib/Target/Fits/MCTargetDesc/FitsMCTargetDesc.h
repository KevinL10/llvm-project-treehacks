#ifndef LLVM_LIB_TARGET_FITS_MCTARGETDESC_H
#define LLVM_LIB_TARGET_FITS_MCTARGETDESC_H

// Include symbolic names for registers. This includes the enum
// for register to register number mapping. (Fits::RA etc) and
// the register classes.
#define GET_REGINFO_ENUM
#include "FitsGenRegisterInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "FitsGenSubtargetInfo.inc"

#define GET_INSTRINFO_ENUM
#include "FitsGenInstrInfo.inc"

#endif
