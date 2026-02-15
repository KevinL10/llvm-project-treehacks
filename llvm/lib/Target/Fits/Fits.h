#ifndef LLVM_LIB_TARGET_FITS_FITS_H
#define LLVM_LIB_TARGET_FITS_FITS_H

#include "MCTargetDesc/FitsMCTargetDesc.h"

#include "llvm/Pass.h"
#include "llvm/Support/CodeGen.h"

namespace llvm {
class FunctionPass;
class FitsTargetMachine;

FunctionPass *createFitsISelDagLegacy(FitsTargetMachine &TM,
                                      CodeGenOptLevel OptLevel);

void initializeFitsDAGToDAGISelLegacyPass(PassRegistry &);
} // namespace llvm
#endif