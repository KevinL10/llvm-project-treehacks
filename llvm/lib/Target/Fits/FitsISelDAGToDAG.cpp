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

INITIALIZE_PASS(FitsDAGToDAGISelLegacy, DEBUG_TYPE, "fits-isel", false, false);

FunctionPass *llvm::createFitsISelDagLegacy(FitsTargetMachine &TM,
                                            CodeGenOptLevel OptLevel) {
  return new FitsDAGToDAGISelLegacy(TM, OptLevel);
}

bool FitsDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  Subtarget =
      &static_cast<const FitsSubtarget &>(MF.getSubtarget<FitsSubtarget>());
  return SelectionDAGISel::runOnMachineFunction(MF);
}

void FitsDAGToDAGISel::Select(SDNode *Node) {
  // Implement the selection logic here.
  // This is where you would match the SelectionDAG nodes to the target
  // instructions. For example, you might want to match a specific node type and
  // then create a corresponding machine instruction.

  // Example: if (Node->getOpcode() == ISD::ADD) { ... }
  // This is just a placeholder for the actual implementation.
  SelectCode(Node);
}