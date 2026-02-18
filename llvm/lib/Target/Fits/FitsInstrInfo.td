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

def JMP : FitsInst<(outs), (ins unknown:$dst), "jmp $dst"> {
  let isBranch = 1;
  let isTerminator = 1;
  let isBarrier = 1;
}

def JMP0 : FitsInst<(outs), (ins GPR:$cond, unknown:$dst), "jmp0 $cond $dst"> {
  let isBranch = 1;
  let isTerminator = 1;
}

def JMPA : FitsInst<(outs), (ins GPRAddr:$dst), "jmp_a $dst"> {
  let isBranch = 1;
  let isTerminator = 1;
  let isBarrier = 1;
}

def JMP0A : FitsInst<(outs), (ins GPR:$cond, GPRAddr:$dst),
                     "jmp0_a $cond $dst"> {
  let isBranch = 1;
  let isTerminator = 1;
}

def ADDrr : FitsInst<(outs GPR:$dst), (ins GPR:$lhs, GPR:$rhs),
                     "add $dst $lhs $rhs">;

def SUBrr : FitsInst<(outs GPR:$dst), (ins GPR:$lhs, GPR:$rhs),
                     "sub $dst $lhs $rhs">;

def MULrr : FitsInst<(outs GPR:$dst), (ins GPR:$lhs, GPR:$rhs),
                     "mul $dst $lhs $rhs">;

def LTErr : FitsInst<(outs GPR:$dst), (ins GPR:$lhs, GPR:$rhs),
                     "lte $dst $lhs $rhs">;

def LTrr : FitsInst<(outs GPR:$dst), (ins GPR:$lhs, GPR:$rhs),
                     "lt $dst $lhs $rhs">;

def SETi : FitsInst<(outs GPR:$dst), (ins unknown:$imm),
                    "set $dst $imm">;

def MEMAddr : Operand<iPTR>, ComplexPattern<iPTR, 2, "SelectAddrDirect", [], []> {
  let MIOperandInfo = (ops unknown, GPRAddr);
  let PrintMethod = "printAddrOperand";
}

def MEMAddrA : Operand<iPTR>, ComplexPattern<iPTR, 2, "SelectAddr", [frameindex, add, sub], []> {
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

def : Pat<(br bb:$dst), (JMP bb:$dst)>;
def : Pat<(brind GPRAddr:$dst), (JMPA $dst)>;
def : Pat<(brind tblockaddress:$dst), (JMP $dst)>;

// LLVM brcond branches on cond != 0, while Fits jmp0 branches on cond == 0.
// For i1-style conditions (0/1), lte cond, 0 yields !cond.
def : Pat<(brcond i32:$cond, bb:$dst), (JMP0 (LTErr $cond, ZERO), bb:$dst)>;

def : Pat<(add i32:$lhs, i32:$rhs), (ADDrr $lhs, $rhs)>;
def : Pat<(add i32:$lhs, imm:$rhs), (ADDrr $lhs, (SETi imm:$rhs))>;
def : Pat<(add imm:$lhs, imm:$rhs), (ADDrr (SETi imm:$lhs), (SETi imm:$rhs))>;

def : Pat<(sub i32:$lhs, i32:$rhs), (SUBrr $lhs, $rhs)>;
def : Pat<(sub i32:$lhs, imm:$rhs), (SUBrr $lhs, (SETi imm:$rhs))>;
def : Pat<(sub imm:$lhs, imm:$rhs), (SUBrr (SETi imm:$lhs), (SETi imm:$rhs))>;

def : Pat<(mul i32:$lhs, i32:$rhs), (MULrr $lhs, $rhs)>;
def : Pat<(mul i32:$lhs, imm:$rhs), (MULrr $lhs, (SETi imm:$rhs))>;
def : Pat<(mul imm:$lhs, imm:$rhs), (MULrr (SETi imm:$lhs), (SETi imm:$rhs))>;

def : Pat<(setle i32:$lhs, i32:$rhs), (LTErr $lhs, $rhs)>;
def : Pat<(setle i32:$lhs, imm:$rhs), (LTErr $lhs, (SETi imm:$rhs))>;
def : Pat<(setle imm:$lhs, i32:$rhs), (LTErr (SETi imm:$lhs), $rhs)>;
def : Pat<(setle imm:$lhs, imm:$rhs), (LTErr (SETi imm:$lhs), (SETi imm:$rhs))>;

def : Pat<(setlt i32:$lhs, i32:$rhs), (LTrr $lhs, $rhs)>;
def : Pat<(setlt i32:$lhs, imm:$rhs), (LTrr $lhs, (SETi imm:$rhs))>;
def : Pat<(setlt imm:$lhs, i32:$rhs), (LTrr (SETi imm:$lhs), $rhs)>;
def : Pat<(setlt imm:$lhs, imm:$rhs), (LTrr (SETi imm:$lhs), (SETi imm:$rhs))>;

def : Pat<(setgt i32:$lhs, i32:$rhs), (LTrr $rhs, $lhs)>;
def : Pat<(setgt i32:$lhs, imm:$rhs), (LTrr (SETi imm:$rhs), $lhs)>;
def : Pat<(setgt imm:$lhs, i32:$rhs), (LTrr $rhs, (SETi imm:$lhs))>;
def : Pat<(setgt imm:$lhs, imm:$rhs), (LTrr (SETi imm:$rhs), (SETi imm:$lhs))>;

def : Pat<(setge i32:$lhs, i32:$rhs), (LTErr $rhs, $lhs)>;
def : Pat<(setge i32:$lhs, imm:$rhs), (LTErr (SETi imm:$rhs), $lhs)>;
def : Pat<(setge imm:$lhs, i32:$rhs), (LTErr $rhs, (SETi imm:$lhs))>;
def : Pat<(setge imm:$lhs, imm:$rhs), (LTErr (SETi imm:$rhs), (SETi imm:$lhs))>;

def : Pat<(i32 (load MEMAddr:$src)), (LOAD MEMAddr:$src)>;
def : Pat<(i32 (load MEMAddrA:$src)), (LOADA MEMAddrA:$src)>;

def : Pat<(store i32:$val, MEMAddr:$dst), (STORE MEMAddr:$dst, $val)>;
def : Pat<(store i32:$val, MEMAddrA:$dst), (STOREA MEMAddrA:$dst, $val)>;

def : Pat<(i32 imm:$imm), (SETi imm:$imm)>;
