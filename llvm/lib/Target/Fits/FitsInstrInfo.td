include "FitsInstrFormats.td"

// Nodes
def FitsRetSDN : SDNode<"FitsISD::Ret",
                        SDTNone, // 0 results and 0 operands
                        [SDNPHasChain, SDNPVariadic, SDNPOptInGlue]>;

// Instructions
def PseudoRet : PseudoFitsInst<(outs), (ins), "ret"> {
  let isReturn = 1;
  let isTerminator = 1;
}

// Nodes -> Instructions
def : Pat<(FitsRetSDN), (PseudoRet)>;