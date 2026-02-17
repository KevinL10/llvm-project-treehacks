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
static bool isRawAddressBaseExpr(SDValue V) {
  return isa<GlobalAddressSDNode>(V) || isa<FrameIndexSDNode>(V) ||
         isa<ExternalSymbolSDNode>(V);
}

static bool isAddressBaseExpr(SDValue V) {
  if (isRawAddressBaseExpr(V))
    return true;

  if (V->isMachineOpcode() && V.getMachineOpcode() == Fits::SETi &&
      V->getNumOperands() == 1)
    return isRawAddressBaseExpr(V.getOperand(0));

  return false;
}

static bool isAddOrSubOpcode(unsigned Opcode) {
  return Opcode == ISD::ADD || Opcode == ISD::SUB || Opcode == Fits::ADDrr ||
         Opcode == Fits::SUBrr;
}

static unsigned getCanonicalAddSubOpcode(unsigned Opcode) {
  return (Opcode == ISD::ADD || Opcode == Fits::ADDrr) ? ISD::ADD : ISD::SUB;
}

static bool splitAddressBaseAndByteOffset(SelectionDAG *DAG, SDValue Addr,
                                          SDValue &Base, SDValue &ByteOff) {
  SDLoc DL(Addr);
  SDValue Zero = DAG->getConstant(0, DL, MVT::i32);

  if (isAddressBaseExpr(Addr)) {
    Base = Addr;
    ByteOff = Zero;
    return true;
  }

  if (isAddOrSubOpcode(Addr.getOpcode())) {
    unsigned CanonicalOpcode = getCanonicalAddSubOpcode(Addr.getOpcode());
    SDValue L = Addr.getOperand(0);
    SDValue R = Addr.getOperand(1);

    SDValue LBase, LOff;
    if (splitAddressBaseAndByteOffset(DAG, L, LBase, LOff)) {
      Base = LBase;
      ByteOff = DAG->getNode(CanonicalOpcode, DL, MVT::i32, LOff, R);
      return true;
    }

    if (CanonicalOpcode == ISD::ADD) {
      SDValue RBase, ROff;
      if (splitAddressBaseAndByteOffset(DAG, R, RBase, ROff)) {
        Base = RBase;
        ByteOff = DAG->getNode(ISD::ADD, DL, MVT::i32, ROff, L);
        return true;
      }
    }
  }

  return false;
}

class FitsDAGToDAGISelLegacy : public SelectionDAGISelLegacy {
public:
  static char ID;
  FitsDAGToDAGISelLegacy(FitsTargetMachine &TM, CodeGenOptLevel OptLevel)
      : SelectionDAGISelLegacy(
            ID, std::make_unique<FitsDAGToDAGISel>(TM, OptLevel)) {}
};
} // namespace

char FitsDAGToDAGISelLegacy::ID = 0;

INITIALIZE_PASS(FitsDAGToDAGISelLegacy, DEBUG_TYPE, "Fits Instruction Selection", false, false)

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
  case ISD::SUB:
  case ISD::SETCC:
    return Node->getSimpleValueType(0) == MVT::i32;
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
  auto MaterializeInReg = [&](auto &&Self, SDValue V) -> SDValue {
    SDLoc DL(V);
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
      if (C->isZero())
        return CurDAG->getRegister(Fits::ZERO, MVT::i32);
      SDValue Imm =
          CurDAG->getSignedTargetConstant(C->getSExtValue(), DL, MVT::i32);
      SDNode *Set = CurDAG->getMachineNode(Fits::SETi, DL, MVT::i32, Imm);
      return SDValue(Set, 0);
    }
    if (auto *ES = dyn_cast<ExternalSymbolSDNode>(V)) {
      SDValue TargetES = CurDAG->getTargetExternalSymbol(
          ES->getSymbol(), MVT::i32, ES->getTargetFlags());
      SDNode *Set = CurDAG->getMachineNode(Fits::SETi, DL, MVT::i32, TargetES);
      return SDValue(Set, 0);
    }
    if (V.getOpcode() == ISD::ADD || V.getOpcode() == ISD::SUB) {
      SDValue LHS = Self(Self, V.getOperand(0));
      SDValue RHS = Self(Self, V.getOperand(1));
      unsigned Opcode = V.getOpcode() == ISD::ADD ? Fits::ADDrr : Fits::SUBrr;
      SDNode *Op = CurDAG->getMachineNode(Opcode, DL, MVT::i32, LHS, RHS);
      return SDValue(Op, 0);
    }
    return V;
  };

  SDValue Base;
  SDValue ByteOff;
  if (splitAddressBaseAndByteOffset(CurDAG, Addr, Base, ByteOff)) {
    if (auto *FI = dyn_cast<FrameIndexSDNode>(Base)) {
      SDLoc DL(Addr);
      SDValue StackRow = CurDAG->getTargetExternalSymbol("__sp", MVT::i32);
      SDValue FullByteOff =
          CurDAG->getNode(ISD::ADD, DL, MVT::i32, SDValue(FI, 0), ByteOff);

      Row = MaterializeInReg(MaterializeInReg, StackRow);
      Col = MaterializeInReg(MaterializeInReg, FullByteOff);
      return true;
    }

    Row = MaterializeInReg(MaterializeInReg, Base);
    Col = MaterializeInReg(MaterializeInReg, ByteOff);
    return true;
  }

  SDLoc DL(Addr);
  Row = MaterializeInReg(MaterializeInReg, Addr);
  Col = CurDAG->getRegister(Fits::ZERO, MVT::i32);
  return true;
}

bool FitsDAGToDAGISel::SelectAddrDirect(SDValue Addr, SDValue &Row,
                                        SDValue &Col) {
  SDLoc DL(Addr);
  if (auto *GA = dyn_cast<GlobalAddressSDNode>(Addr)) {
    Row = CurDAG->getTargetGlobalAddress(
        GA->getGlobal(), DL, MVT::i32, GA->getOffset(), GA->getTargetFlags());
  } else if (auto *C = dyn_cast<ConstantSDNode>(Addr)) {
    Row = CurDAG->getSignedTargetConstant(C->getSExtValue(), DL, MVT::i32);
  } else {
    return false;
  }

  Col = CurDAG->getRegister(Fits::ZERO, MVT::i32);
  return true;
}

void FitsDAGToDAGISel::Select(SDNode *Node) {
  if (Node->isMachineOpcode()) {
    Node->setNodeId(-1);
    return;
  }

  if (auto *C = dyn_cast<ConstantSDNode>(Node)) {
    if (Node->getSimpleValueType(0) == MVT::i32 && C->isZero()) {
      ReplaceNode(Node, CurDAG->getRegister(Fits::ZERO, MVT::i32).getNode());
      return;
    }
  }

  if (Node->getOpcode() == ISD::SHL && Node->getSimpleValueType(0) == MVT::i32) {
    auto *ShiftAmt = dyn_cast<ConstantSDNode>(Node->getOperand(1));
    if (!ShiftAmt) {
      report_fatal_error(
          "fits-isel: unsupported shl form (only constant shift amounts are supported)",
          false);
    }

    uint64_t Amt = ShiftAmt->getZExtValue();
    if (Amt >= 32) {
      report_fatal_error("fits-isel: shl amount out of range for i32", false);
    }

    SDLoc DL(Node);
    SDValue ScaleImm = CurDAG->getTargetConstant(1ULL << Amt, DL, MVT::i32);
    SDNode *Scale = CurDAG->getMachineNode(Fits::SETi, DL, MVT::i32, ScaleImm);
    SDNode *Mul =
        CurDAG->getMachineNode(Fits::MULrr, DL, MVT::i32, Node->getOperand(0),
                               SDValue(Scale, 0));
    ReplaceNode(Node, Mul);
    return;
  }

  if (Node->getOpcode() == ISD::SETCC) {
    const auto *CC = cast<CondCodeSDNode>(Node->getOperand(2));
    if (CC->get() != ISD::SETLE && CC->get() != ISD::SETLT &&
        CC->get() != ISD::SETGE && CC->get() != ISD::SETGT) {
      report_fatal_error(
          "fits-isel: unsupported icmp predicate (only signed <, <=, >, and >= are currently "
          "supported)",
          false);
    }
  }

  SelectCode(Node);
}
