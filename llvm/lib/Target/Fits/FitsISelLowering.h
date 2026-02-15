#ifndef LLVM_LIB_TARGET_FITS_FITSISELLOWERING_H
#define LLVM_LIB_TARGET_FITS_FITSISELLOWERING_H

#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

namespace FitsISD {
enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

  // Return
  Ret,
};
} // end namespace FitsISD

class FitsSubtarget;

class FitsTargetLowering : public TargetLowering {
public:
  explicit FitsTargetLowering(const TargetMachine &TM,
                              const FitsSubtarget &STI);

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &dl,
                      SelectionDAG &DAG) const override;

  SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;

  bool CanLowerReturn(CallingConv::ID CallConv, MachineFunction &MF,
                      bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      LLVMContext &Context, const Type *RetTy) const override;
  SDValue
  LowerFormalArguments(SDValue Chain, CallingConv::ID /* CallConv */,
                       bool /* isVarArg */,
                       const SmallVectorImpl<ISD::InputArg> & /* Ins */,
                       const SDLoc & /* dl */, SelectionDAG & /* DAG */,
                       SmallVectorImpl<SDValue> & /* InVals */) const override {
    return Chain;
  }

  /// getTargetNodeName - This method returns the name of a target specific
  //  DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;
};

} // namespace llvm

#endif