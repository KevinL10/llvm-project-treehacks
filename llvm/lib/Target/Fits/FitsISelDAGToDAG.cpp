#include "FitsISelDAGToDAG.h"
#include "FitsISelLowering.h"
#include "FitsSubtarget.h"

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Pass.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

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

bool FitsDAGToDAGISel::canSelectWithPatternsOrGeneric(SDNode *Node) const {
  switch (Node->getOpcode()) {
  case FitsISD::Ret:
    return true;
  case ISD::EntryToken:
  case ISD::TokenFactor:
  case ISD::FrameIndex:
  case ISD::Register:
  case ISD::CopyToReg:
  case ISD::CopyFromReg:
    return true;
  case ISD::ADD:
    return Node->getSimpleValueType(0) == MVT::i32;
  case ISD::SETCC:
    return true;
  case ISD::LOAD: {
    auto *LD = cast<LoadSDNode>(Node);
    return LD->getMemoryVT() == MVT::i32 &&
           LD->getAddressingMode() == ISD::UNINDEXED &&
           LD->getExtensionType() == ISD::NON_EXTLOAD;
  }
  case ISD::STORE: {
    auto *ST = cast<StoreSDNode>(Node);
    return ST->getMemoryVT() == MVT::i32 &&
           ST->getAddressingMode() == ISD::UNINDEXED &&
           !ST->isTruncatingStore();
  }
  case ISD::Constant:
    return cast<ConstantSDNode>(Node)->getValueType(0) == MVT::i32;
  default:
    return false;
  }
}

void FitsDAGToDAGISel::ignoreUnsupportedNode(SDNode *Node) {
  if (ReportedUnsupportedOpcodes.insert(Node->getOpcode()).second) {
    errs() << "fits-isel: unsupported DAG node ignored: "
           << Node->getOperationName(CurDAG) << '\n';
  }

  SDLoc DL(Node);
  SDValue ChainIn;
  SDValue GlueIn;
  for (const SDValue &Op : Node->ops()) {
    if (!ChainIn && Op.getValueType() == MVT::Other)
      ChainIn = Op;
    if (!GlueIn && Op.getValueType() == MVT::Glue)
      GlueIn = Op;
  }

  for (unsigned I = 0, E = Node->getNumValues(); I != E; ++I) {
    EVT VT = Node->getValueType(I);
    SDValue Replacement;
    if (VT == MVT::Other) {
      Replacement = ChainIn ? ChainIn : CurDAG->getEntryNode();
    } else if (VT == MVT::Glue) {
      Replacement = GlueIn;
    } else if (VT == MVT::i32) {
      SDValue Zero = CurDAG->getTargetConstant(0, DL, MVT::i32);
      SDNode *SetZero = CurDAG->getMachineNode(Fits::SETi, DL, MVT::i32, Zero);
      Replacement = SDValue(SetZero, 0);
    } else {
      Replacement = CurDAG->getUNDEF(VT);
    }

    if (!Replacement)
      Replacement = CurDAG->getUNDEF(VT);

    ReplaceUses(SDValue(Node, I), Replacement);
  }

  CurDAG->RemoveDeadNode(Node);
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

bool FitsDAGToDAGISel::SelectAddrDirect(SDValue Addr, SDValue &Row,
                                        SDValue &Col) {
  SDLoc DL(Addr);
  if (auto *GA = dyn_cast<GlobalAddressSDNode>(Addr)) {
    Row = CurDAG->getTargetGlobalAddress(
        GA->getGlobal(), DL, MVT::i32, GA->getOffset(), GA->getTargetFlags());
  } else if (auto *C = dyn_cast<ConstantSDNode>(Addr)) {
    Row = CurDAG->getTargetConstant(C->getSExtValue(), DL, MVT::i32);
  } else {
    return false;
  }

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

  if (Node->getOpcode() == ISD::SETCC) {
    const auto *CC = cast<CondCodeSDNode>(Node->getOperand(2));
    if (CC->get() != ISD::SETLE && CC->get() != ISD::SETLT) {
      report_fatal_error(
          "fits-isel: unsupported icmp predicate (only signed < and <= are currently "
          "supported)",
          false);
    }
  }

  // if (!canSelectWithPatternsOrGeneric(Node)) {
  //   ignoreUnsupportedNode(Node);
  //   return;
  // }

  SelectCode(Node);
}
