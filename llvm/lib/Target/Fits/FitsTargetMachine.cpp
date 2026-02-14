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
  RegisterTarget<Triple::fits> X(getTheFitsTarget(),
      "fits", "Fits (32-bit big endian)", "Fits");
}

extern "C" void LLVMInitializeFitsTarget() {
  // TODO: Register target machine, passes, etc.
}
