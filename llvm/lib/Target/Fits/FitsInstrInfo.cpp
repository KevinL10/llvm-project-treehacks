#include "FitsInstrInfo.h"
#include "FitsTargetMachine.h"
#include "MCTargetDesc/FitsMCTargetDesc.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

using namespace llvm;

#define DEBUG_TYPE "Fits Instruction Info"

#define GET_INSTRINFO_CTOR_DTOR
#include "FitsGenInstrInfo.inc"

FitsInstrInfo::FitsInstrInfo(const FitsSubtarget &STI)
    : FitsGenInstrInfo(-1, -1), Subtarget(STI) {}