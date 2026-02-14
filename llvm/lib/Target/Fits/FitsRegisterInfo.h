#ifndef LLVM_LIB_TARGET_FITS_FITSREGISTERINFO_H
#define LLVM_LIB_TARGET_FITS_FITSREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "FitsGenRegisterInfo.inc"


namespace llvm {
class FitsRegisterInfo final : public FitsGenRegisterInfo {
public:
    FitsRegisterInfo();
    const MCPhysReg* getCalleeSavedRegs(const MachineFunction *MF) const override;
  
    BitVector getReservedRegs(const MachineFunction &MF) const override;
  
    bool eliminateFrameIndex(MachineBasicBlock::iterator II,
                              int SPAdj, unsigned FIOperandNum,
                              RegScavenger *RS = nullptr) const override;
  
    Register getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm

#endif
