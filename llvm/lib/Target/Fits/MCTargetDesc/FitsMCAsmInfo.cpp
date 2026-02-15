#include "FitsMCAsmInfo.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

FitsMCAsmInfo::FitsMCAsmInfo(const Triple &TT) {
  IsLittleEndian = false;
  CommentString = ";";
  AlignmentIsInBytes = true;
  HasSingleParameterDotFile = false;
  HasDotTypeDotSizeDirective = false;
}
