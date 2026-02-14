#include "FitsMCTargetDesc.h"
#include "FitsTargetInfo.h"

#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "FitsGenRegisterInfo.inc"

static MCRegisterInfo* createFitsMCRegisterInfo(const Triple &TT) {
    MCRegisterInfo *X = new MCRegisterInfo();
    InitFitsMCRegisterInfo(X, Fits::RA);
    return X;
}

extern "C" void LLVMInitializeFitsTargetMC() {
  Target *T =  &getTheFitsTarget();
  TargetRegistry::RegisterMCRegInfo(*T, createFitsMCRegisterInfo);
}
