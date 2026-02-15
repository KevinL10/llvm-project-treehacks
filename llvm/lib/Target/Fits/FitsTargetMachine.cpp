#include "FitsTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

Target &llvm::getTheFitsTarget() {
  static Target TheFitsTarget;
  return TheFitsTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeFitsTargetInfo() {
  RegisterTarget<Triple::fits> X(getTheFitsTarget(), "fits",
                                 "Fits (32-bit big endian)", "Fits");
}

extern "C" void LLVMInitializeFitsTarget() {
  RegisterTargetMachine<FitsTargetMachine> X(getTheFitsTarget());

  initializeFitsDAGToDAGISelLegacyPass(*PassRegistry::getPassRegistry());
}

namespace {
class FitsPassConfig : public TargetPassConfig {
public:
  FitsPassConfig(FitsTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  FitsTargetMachine &getFitsTargetMachine() const {
    return getTM<FitsTargetMachine>();
  }
  bool addInstSelector() override {
    addPass(createFitsISelDagLegacy(getFitsTargetMachine(), getOptLevel()));
    return false;
  }
  void addPreEmitPass() override {}
};
} // namespace

TargetPassConfig *FitsTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new FitsPassConfig(*this, PM);
}