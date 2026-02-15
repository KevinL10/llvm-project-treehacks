#include "FitsMCTargetDesc.h"
#include "FitsTargetInfo.h"
#include "MCTargetDesc/FitsMCAsmInfo.h"
#include "MCTargetDesc/FitsMCInstPrinter.h"

#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "FitsGenInstrInfo.inc"

#define GET_REGINFO_MC_DESC
#include "FitsGenRegisterInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "FitsGenSubtargetInfo.inc"

static MCRegisterInfo *createFitsMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitFitsMCRegisterInfo(X, Fits::RA);
  return X;
}


static MCSubtargetInfo *createFitsSubtargetInfo(const Triple &TT, StringRef CPU,
                                                StringRef FS) {
  if (CPU.empty())
    CPU = "generic";
  return createFitsMCSubtargetInfoImpl(TT, CPU, CPU, FS);
}

static MCAsmInfo *createFitsMCAsmInfo(const MCRegisterInfo &MRI,
                                      const Triple &TT,
                                      const MCTargetOptions &Options) {
  MCAsmInfo *X = new FitsMCAsmInfo(TT);
  return X;
}

static MCInstrInfo *createFitsMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitFitsMCInstrInfo(X);
  return X;
}

static MCInstPrinter *createFitsMCInstPrinter(const Triple &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new FitsInstPrinter(MAI, MII, MRI);
}

extern "C" void LLVMInitializeFitsTargetMC() {
  Target *T = &getTheFitsTarget();
  TargetRegistry::RegisterMCRegInfo(*T, createFitsMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(*T, createFitsSubtargetInfo);
  TargetRegistry::RegisterMCAsmInfo(*T, createFitsMCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(*T, createFitsMCInstrInfo);
  TargetRegistry::RegisterMCInstPrinter(*T, createFitsMCInstPrinter);
}
