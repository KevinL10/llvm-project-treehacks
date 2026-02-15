#ifndef LLVM_LIB_TARGET_FITS_FITSISELDAGTODAG_H
#define LLVM_LIB_TARGET_FITS_FITSISELDAGTODAG_H

#include "FitsSubtarget.h"
#include "FitsTargetMachine.h"

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

namespace llvm {
class FitsDAGToDAGISel final : public SelectionDAGISel {
  const FitsSubtarget *Subtarget;

public:
  explicit FitsDAGToDAGISel(FitsTargetMachine &TM, CodeGenOptLevel OptLevel)
      : SelectionDAGISel(TM, OptLevel) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

private:
#include "FitsGenDAGISel.inc"

  bool SelectAddr(SDValue Addr, SDValue &Row, SDValue &Col);
  void Select(SDNode *Node) override;
};
} // namespace llvm

#endif
