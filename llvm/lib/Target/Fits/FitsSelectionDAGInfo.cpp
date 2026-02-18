#include "FitsSelectionDAGInfo.h"

#define GET_SDNODE_DESC
#include "FitsGenSDNodeInfo.inc"

using namespace llvm;

FitsSelectionDAGInfo::FitsSelectionDAGInfo()
    : SelectionDAGGenTargetInfo(FitsGenSDNodeInfo) {}

FitsSelectionDAGInfo::~FitsSelectionDAGInfo() = default;