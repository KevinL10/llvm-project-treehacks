#include "Fits.h"
#include "FitsSubtarget.h"
#include "FitsTargetInfo.h"
#include "FitsTargetMachine.h"
#include "MCTargetDesc/FitsMCInstPrinter.h"

#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetOpcodes.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"

#define DEBUG_TYPE "nova-asm-printer"

using namespace llvm;

namespace {
static bool collectFitsInitWords(const Constant *C,
                                 SmallVectorImpl<int64_t> &Words) {
  if (const auto *CI = dyn_cast<ConstantInt>(C)) {
    Words.push_back(CI->getSExtValue());
    return true;
  }

  if (isa<UndefValue>(C) || isa<ConstantPointerNull>(C)) {
    Words.push_back(0);
    return true;
  }

  if (const auto *CAZ = dyn_cast<ConstantAggregateZero>(C)) {
    for (unsigned I = 0, E = CAZ->getElementCount().getKnownMinValue(); I != E;
         ++I) {
      if (!collectFitsInitWords(CAZ->getElementValue(I), Words))
        return false;
    }
    return true;
  }

  if (const auto *CA = dyn_cast<ConstantArray>(C)) {
    for (const auto &Op : CA->operands()) {
      if (!collectFitsInitWords(cast<Constant>(Op), Words))
        return false;
    }
    return true;
  }

  if (const auto *CDS = dyn_cast<ConstantDataSequential>(C)) {
    if (!CDS->getElementType()->isIntegerTy())
      return false;
    for (unsigned I = 0, E = CDS->getNumElements(); I != E; ++I)
      Words.push_back(static_cast<int64_t>(CDS->getElementAsInteger(I)));
    return true;
  }

  return false;
}

class FitsAsmPrinter : public AsmPrinter {
public:
  FitsAsmPrinter(TargetMachine &TM, std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "Fits Assembly Printer"; }

  void emitStartOfAsmFile(Module &M) override;
  void emitGlobalVariable(const GlobalVariable *GV) override;
  void emitInstruction(const MachineInstr *MI) override;

  // Lower the MachineInstr to MCInst
  void lowerInstruction(const MachineInstr &MI, MCInst &Inst);
private:
  DenseSet<const GlobalVariable *> GlobalsPrintedAsDecls;
  MCOperand lowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym);
  MCOperand lowerGlobalAddressOperand(const MachineOperand &MO);
};

void FitsAsmPrinter::emitStartOfAsmFile(Module &M) {
  OutStreamer->emitRawText("__zero = 0");
  for (unsigned I = 0; I != 32; ++I) {
    SmallString<16> Line;
    raw_svector_ostream OS(Line);
    OS << "__r" << I << " = 0";
    OutStreamer->emitRawText(OS.str());
  }
  OutStreamer->emitRawText("__sp = 0");
  OutStreamer->emitRawText("__fp = 0");
  OutStreamer->emitRawText("__ra = 0");

  OutStreamer->addBlankLine();

  for (const GlobalVariable &GV : M.globals()) {
    if (GV.isDeclaration() || !GV.hasInitializer())
      continue;

    SmallVector<int64_t, 8> Words;
    if (!collectFitsInitWords(GV.getInitializer(), Words))
      continue;

    SmallString<64> Line;
    raw_svector_ostream OS(Line);
    OS << getSymbol(&GV)->getName() << " = ";
    for (unsigned I = 0, E = Words.size(); I != E; ++I) {
      if (I)
        OS << ' ';
      OS << Words[I];
    }
    OutStreamer->emitRawText(OS.str());
    GlobalsPrintedAsDecls.insert(&GV);
  }

  if (!GlobalsPrintedAsDecls.empty())
    OutStreamer->addBlankLine();
}

void FitsAsmPrinter::emitGlobalVariable(const GlobalVariable *GV) {
  if (GlobalsPrintedAsDecls.contains(GV))
    return;
  AsmPrinter::emitGlobalVariable(GV);
}

MCOperand FitsAsmPrinter::lowerSymbolOperand(const MachineOperand &MO,
                                             MCSymbol *Sym) {
  auto &Ctx = OutContext;
  const MCExpr *Expr = MCSymbolRefExpr::create(Sym, Ctx);
  assert(MO.isMBB() && "Only basic block symbols are supported");
  return MCOperand::createExpr(Expr);
}

MCOperand FitsAsmPrinter::lowerGlobalAddressOperand(const MachineOperand &MO) {
  const GlobalValue *GV = MO.getGlobal();
  const MCExpr *Expr = MCSymbolRefExpr::create(getSymbol(GV), OutContext);
  if (MO.getOffset())
    Expr = MCBinaryExpr::createAdd(
        Expr, MCConstantExpr::create(MO.getOffset(), OutContext), OutContext);
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
    case MachineOperand::MO_GlobalAddress:
      MCOp = lowerGlobalAddressOperand(Op);
      break;
    case MachineOperand::MO_ExternalSymbol:
      MCOp = MCOperand::createExpr(
          MCSymbolRefExpr::create(GetExternalSymbolSymbol(Op.getSymbolName()),
                                  OutContext));
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
