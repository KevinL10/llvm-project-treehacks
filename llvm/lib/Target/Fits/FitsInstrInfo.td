include "FitsInstrFormats.td"

// Nodes
def FitsRetSDN : SDNode<"FitsISD::Ret",
                        SDTNone, // 0 results and 0 operands
                        [SDNPHasChain, SDNPVariadic, SDNPOptInGlue]>;

// Instructions
def PseudoRet : PseudoFitsInst<(outs), (ins), "halt"> {
  let isReturn = 1;
  let isTerminator = 1;
}

def ADDrr : FitsInst<(outs GPR:$dst), (ins GPR:$lhs, GPR:$rhs),
                     "add $dst $lhs $rhs">;

def SETi : FitsInst<(outs GPR:$dst), (ins unknown:$imm),
                    "set $dst $imm">;

def MEMAddr : Operand<iPTR>, ComplexPattern<iPTR, 2, "SelectAddrDirect", [], []> {
  let MIOperandInfo = (ops unknown, GPRAddr);
  let PrintMethod = "printAddrOperand";
}

def MEMAddrA : Operand<iPTR>, ComplexPattern<iPTR, 2, "SelectAddr", [frameindex], []> {
  let MIOperandInfo = (ops GPRAddr, GPRAddr);
  let PrintMethod = "printAddrOperand";
}

def LOAD : FitsInst<(outs GPR:$dst), (ins MEMAddr:$addr),
                    "load $dst $addr"> {
  let mayLoad = 1;
}

def LOADA : FitsInst<(outs GPR:$dst), (ins MEMAddrA:$addr),
                     "load_a $dst $addr"> {
  let mayLoad = 1;
}

def STORE : FitsInst<(outs), (ins MEMAddr:$addr, GPR:$in),
                     "store $addr $in"> {
  let mayStore = 1;
}

def STOREA : FitsInst<(outs), (ins MEMAddrA:$addr, GPR:$in),
                      "store_a $addr $in"> {
  let mayStore = 1;
}

// Nodes -> Instructions
def : Pat<(FitsRetSDN), (PseudoRet)>;
def : Pat<(add i32:$lhs, i32:$rhs), (ADDrr $lhs, $rhs)>;
def : Pat<(add i32:$lhs, imm:$rhs), (ADDrr $lhs, (SETi imm:$rhs))>;
def : Pat<(add imm:$lhs, imm:$rhs), (ADDrr (SETi imm:$lhs), (SETi imm:$rhs))>;
def : Pat<(i32 imm:$imm), (SETi imm:$imm)>;
def : Pat<(i32 (load MEMAddr:$src)), (LOAD MEMAddr:$src)>;
def : Pat<(store i32:$val, MEMAddr:$dst), (STORE MEMAddr:$dst, $val)>;
def : Pat<(i32 (load MEMAddrA:$src)), (LOADA MEMAddrA:$src)>;
def : Pat<(store i32:$val, MEMAddrA:$dst), (STOREA MEMAddrA:$dst, $val)>;
