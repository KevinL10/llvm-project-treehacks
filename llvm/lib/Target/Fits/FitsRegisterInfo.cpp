#include "FitsRegisterInfo.h"
#include "MCTargetDesc/FitsMCTargetDesc.h"
#include "FitsFrameLowering.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "fits-reginfo"

#define GET_REGINFO_TARGET_DESC
#include "FitsGenRegisterInfo.inc"

FitsRegisterInfo::FitsRegisterInfo() : FitsGenRegisterInfo(Fits::RA) {}

const MCPhysReg *
FitsRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  static const MCPhysReg CSRList[] = {
      Fits::SP, // Stack Pointer
      Fits::FP // Frame Pointer
  };
  return CSRList;
}

BitVector FitsRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  static const MCPhysReg ReservedRegs[] = {
      Fits::ZERO, Fits::SP, Fits::PC, Fits::RA,
      Fits::FP,
  };

  BitVector Reserved(getNumRegs());
  for (MCPhysReg Reg : ReservedRegs) {
    Reserved.set(Reg);
  }
  return Reserved;
}

bool FitsRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();

  const int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  int64_t Offset = MFI.getObjectOffset(FrameIndex);

  if ((Offset % 4) != 0 || (SPAdj % 4) != 0) {
    report_fatal_error("fits-reginfo: unaligned frame index offset", false);
  }

  // Frame indices are materialized as column offsets inside a shared stack
  // backing row (selected in FitsISelDAGToDAG). Keep column 0 untouched and
  // keep all stack slots word-aligned.
  constexpr int64_t FitsStackBaseCol = 4;
  Offset = FitsStackBaseCol + Offset + SPAdj;

  MI.getOperand(FIOperandNum).ChangeToImmediate(Offset);
  return false;
}

Register FitsRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return Fits::FP;
}
