class FitsInst<dag outs, dag ins, string asmString> : Instruction {
  let Namespace = "Fits";
  let OutOperandList = outs;
  let InOperandList = ins;
  let AsmString = asmString;
}

class PseudoFitsInst<dag outs, dag ins, string asmString> : FitsInst<outs, ins, asmString> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}