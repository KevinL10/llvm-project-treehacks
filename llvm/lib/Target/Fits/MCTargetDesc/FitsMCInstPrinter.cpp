#include "FitsMCInstPrinter.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"

#define DEBUG_TYPE "fits-mcinst-printer"

using namespace llvm;

#define PRINT_ALIAS_INSTR
#include "FitsGenAsmWriter.inc"

void FitsInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                StringRef Annot, const MCSubtargetInfo &STI,
                                raw_ostream &O) {
  // Check if we have an alias
  if (!printAliasInstr(MI, Address, O)) {
    printInstruction(MI, Address, O);
  }
  printAnnotation(O, Annot);
}

void FitsInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) {
  OS << "__" << StringRef(getRegisterName(Reg)).lower();
}

void FitsInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                   raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  assert(Op.isExpr() && "Unknown operand type");
  MAI.printExpr(O, *Op.getExpr());
}

void FitsInstPrinter::printAddrOperand(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O) {
  printOperand(MI, OpNo, O);
  O << ' ';
  printOperand(MI, OpNo + 1, O);
}
