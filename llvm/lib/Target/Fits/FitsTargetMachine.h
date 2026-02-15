#ifndef LLVM_LIB_TARGET_FITS_FITSTARGETMACHINE_H
#define LLVM_LIB_TARGET_FITS_FITSTARGETMACHINE_H

#include "FitsSubtarget.h"
#include "FitsTargetObjectFile.h"
#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

static const char *FitsDataLayoutString = "E-p:32:32-i8:32-i16:32-n32-S32-Fi32";

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  if (!RM)
    return Reloc::Static;
  return *RM;
}

namespace llvm {

class FitsTargetMachine final : public CodeGenTargetMachineImpl {
  FitsSubtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:
  FitsTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                    StringRef FS, const TargetOptions &Options,
                    std::optional<Reloc::Model> RM,
                    std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                    bool JIT)
      : CodeGenTargetMachineImpl(T, FitsDataLayoutString, TT, CPU, FS, Options,
                                 getEffectiveRelocModel(RM),
                                 getEffectiveCodeModel(CM, CodeModel::Medium),
                                 OL),
        Subtarget(TT, CPU, FS, *this), TLOF(new FitsTargetObjectFile()) {
    initAsmInfo();
  }

  // ~FitsTargetMachine() override = default;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  const FitsSubtarget *getSubtargetImpl(const Function &F) const override {
    return &Subtarget;
  }

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
};
} // namespace llvm

#endif
