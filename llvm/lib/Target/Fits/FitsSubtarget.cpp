#include "FitsSubtarget.h"
#include "Fits.h"
#include "FitsRegisterInfo.h"
#include "FitsTargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "fits-subtarget"

#define GET_SUBTARGETINFO_CTOR
#define GET_SUBTARGETINFO_TARGET_DESC
#include "FitsGenSubtargetInfo.inc"