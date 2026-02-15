#include "FitsISelDAGToDAG.h"
#include "FitsSubtarget.h"

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Pass.h"
#include "llvm/Support/CodeGen.h"

using namespace llvm;

#define DEBUG_TYPE "fits-isel"

namespace {
class FitsDAGToDAGISelLegacy : public SelectionDAGISelLegacy {
public:
  static char ID;
  FitsDAGToDAGISelLegacy(FitsTargetMachine &TM, CodeGenOptLevel OptLevel)
      : SelectionDAGISelLegacy(
            ID, std::make_unique<FitsDAGToDAGISel>(TM, OptLevel)) {}
};
} // namespace

char FitsDAGToDAGISelLegacy::ID = 0;

INITIALIZE_PASS(FitsDAGToDAGISelLegacy, DEBUG_TYPE, "fits-isel", false, false)

FunctionPass *llvm::createFitsISelDagLegacy(FitsTargetMachine &TM,
                                            CodeGenOptLevel OptLevel) {
  return new FitsDAGToDAGISelLegacy(TM, OptLevel);
}

bool FitsDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  Subtarget =
      &static_cast<const FitsSubtarget &>(MF.getSubtarget<FitsSubtarget>());
  return SelectionDAGISel::runOnMachineFunction(MF);
}

bool FitsDAGToDAGISel::SelectAddr(SDValue Addr, SDValue &Row, SDValue &Col) {
  SDLoc DL(Addr);
  auto MaterializeInReg = [&](SDValue V) -> SDValue {
    if (auto *GA = dyn_cast<GlobalAddressSDNode>(V)) {
      SDValue TargetGA = CurDAG->getTargetGlobalAddress(
          GA->getGlobal(), DL, MVT::i32, GA->getOffset(), GA->getTargetFlags());
      SDNode *Set = CurDAG->getMachineNode(Fits::SETi, DL, MVT::i32, TargetGA);
      return SDValue(Set, 0);
    }
    if (auto *FI = dyn_cast<FrameIndexSDNode>(V)) {
      SDValue TargetFI = CurDAG->getTargetFrameIndex(FI->getIndex(), MVT::i32);
      SDNode *Set = CurDAG->getMachineNode(Fits::SETi, DL, MVT::i32, TargetFI);
      return SDValue(Set, 0);
    }
    if (auto *C = dyn_cast<ConstantSDNode>(V)) {
      SDValue Imm = CurDAG->getTargetConstant(C->getSExtValue(), DL, MVT::i32);
      SDNode *Set = CurDAG->getMachineNode(Fits::SETi, DL, MVT::i32, Imm);
      return SDValue(Set, 0);
    }
    return V;
  };

  Row = MaterializeInReg(Addr);
  SDValue Zero = CurDAG->getTargetConstant(0, DL, MVT::i32);
  SDNode *SetZero = CurDAG->getMachineNode(Fits::SETi, DL, MVT::i32, Zero);
  Col = SDValue(SetZero, 0);
  return true;
}

void FitsDAGToDAGISel::Select(SDNode *Node) {
  if (Node->isMachineOpcode()) {
    Node->setNodeId(-1);
    return;
  }

  SelectCode(Node);
}
