#include "Fits.h"
#include "FitsSubtarget.h"
#include "FitsTargetInfo.h"
#include "FitsTargetMachine.h"
#include "MCTargetDesc/FitsMCInstPrinter.h"

#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"

#define DEBUG_TYPE "nova-asm-printer"

using namespace llvm;

namespace {
class FitsAsmPrinter : public AsmPrinter {
public:
  FitsAsmPrinter(TargetMachine &TM, std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "Fits Assembly Printer"; }

  void emitInstruction(const MachineInstr *MI) override;

  // Lower the MachineInstr to MCInst
  void lowerInstruction(const MachineInstr &MI, MCInst &Inst);

  // bool lowerPseudoInstExpansion(const MachineInstr *MI, MCInst &Inst);
private:
  MCOperand lowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym);
};

MCOperand FitsAsmPrinter::lowerSymbolOperand(const MachineOperand &MO,
                                             MCSymbol *Sym) {
  auto &Ctx = OutContext;
  const MCExpr *Expr =
      MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, Ctx);
  assert(MO.isMBB() && "Only basic block symbols are supported");
  return MCOperand::createExpr(Expr);
}

void FitsAsmPrinter::lowerInstruction(const MachineInstr &MI, MCInst &Inst) {
  // This function should convert the MachineInstr to MCInst
  // The implementation will depend on the specific instruction set
  // and how you want to represent it in the MCInst format.
  // For now, we will just print the opcode and operands.

  Inst.setOpcode(MI.getOpcode());
  for (const auto &Op : MI.operands()) {
    MCOperand MCOp;
    switch (Op.getType()) {
    case MachineOperand::MO_Register:
      MCOp = MCOperand::createReg(Op.getReg());
      break;
    case MachineOperand::MO_Immediate:
      MCOp = MCOperand::createImm(Op.getImm());
      break;
    case MachineOperand::MO_MachineBasicBlock:
      MCOp = lowerSymbolOperand(Op, Op.getMBB()->getSymbol());
      break;
    // Add other operand types as needed
    default:
      llvm_unreachable("Unsupported operand type");
    }
    Inst.addOperand(MCOp);
  }
}

} // end anonymous namespace

void FitsAsmPrinter::emitInstruction(const MachineInstr *MI) {
  // Lower the instruction to MCInst
  MCInst Inst;
  lowerInstruction(*MI, Inst);
  EmitToStreamer(*OutStreamer, Inst);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeFitsAsmPrinter() {
  RegisterAsmPrinter<FitsAsmPrinter> X(getTheFitsTarget());
}