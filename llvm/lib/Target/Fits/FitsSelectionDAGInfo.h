#ifndef LLVM_LIB_TARGET_FITS_FITSSELECTIONDAGINFO_H
#define LLVM_LIB_TARGET_FITS_FITSSELECTIONDAGINFO_H

#include "llvm/CodeGen/SelectionDAGTargetInfo.h"

#define GET_SDNODE_ENUM
#include "FitsGenSDNodeInfo.inc"

namespace llvm {

class FitsSelectionDAGInfo : public SelectionDAGGenTargetInfo {
public:
  FitsSelectionDAGInfo();

  ~FitsSelectionDAGInfo() override;
};

} // namespace llvm

#endif
