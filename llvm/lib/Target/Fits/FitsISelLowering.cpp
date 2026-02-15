#include "FitsISelLowering.h"
#include "FitsSubtarget.h"
#include "MCTargetDesc/FitsMCTargetDesc.h"

using namespace llvm;

#define DEBUG_TYPE "fits-isel"

FitsTargetLowering::FitsTargetLowering(const TargetMachine &TM,
                                       const FitsSubtarget &STI)
    : TargetLowering(TM) {
  addRegisterClass(MVT::i32, &Fits::GPRAddrRegClass);
  setOperationAction(ISD::SETCC, MVT::i32, Legal);
  setBooleanContents(ZeroOrOneBooleanContent);

  computeRegisterProperties(STI.getRegisterInfo());
}

SDValue
FitsTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                bool isVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                const SDLoc &dl, SelectionDAG &DAG) const {
  // Handle only integer return values
  // we need to copy the value to the r0 register.
  if (Outs.size() > 1) {
    report_fatal_error(
        "Multiple return values not supported\n"
        "This could be because the return type is a struct or a large integer "
        "that got split into multiple registers",
        false);
  }

  if (Outs.size() == 0) {
    return DAG.getNode(FitsISD::Ret, dl, MVT::Other, Chain);
  }

  // Otherwise, copy the outs to registers.
  SDValue Glue;
  SmallVector<SDValue, 3> RetOps(1, Chain);
  for (unsigned i = 0, e = Outs.size(); i != e; ++i) {
    const ISD::OutputArg &Out = Outs[i];
    const SDValue &OutVal = OutVals[i];
    if (!Out.ArgVT.isScalarInteger() || Out.ArgVT.getScalarSizeInBits() > 32) {
      report_fatal_error("Only i32 return values are supported", false);
    }
    Chain = DAG.getCopyToReg(Chain, dl, Fits::R0, OutVal, Glue);
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(Fits::R0, Out.VT));
  }
  RetOps[0] = Chain;
  RetOps.push_back(Glue);

  return DAG.getNode(FitsISD::Ret, dl, MVT::Other, RetOps);
}

SDValue FitsTargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                      SmallVectorImpl<SDValue> &InVals) const {
  return SDValue();
}

bool FitsTargetLowering::CanLowerReturn(
    CallingConv::ID CallConv, MachineFunction &MF, bool IsVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs, LLVMContext &Context,
    const Type *RetTy) const {
  return true;
}

const char *FitsTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case FitsISD::Ret:
    return "FitsISD::Ret";
  default:
    return "Unknown FitsISD::Node";
  }
}
