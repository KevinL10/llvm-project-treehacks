#include "Fits.h"
#include "FitsSubtarget.h"
#include "FitsTargetInfo.h"
#include "FitsTargetMachine.h"
#include "MCTargetDesc/FitsMCInstPrinter.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetOpcodes.h"
#include "llvm/ADT/SmallVector.h"
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

  bool runOnMachineFunction(MachineFunction &MF) override;
  void emitStartOfAsmFile(Module &M) override;
  void emitGlobalVariable(const GlobalVariable *GV) override;
  void emitInstruction(const MachineInstr *MI) override;

  // Lower the MachineInstr to MCInst
  void lowerInstruction(const MachineInstr &MI, MCInst &Inst);
private:
  void emitRegisterDecl(StringRef Name);
  void collectUsedRegisters(const MachineFunction &MF, BitVector &UsedGPR,
                            bool &NeedZero, bool &NeedSP, bool &NeedFP,
                            bool &NeedRA) const;
  void emitUsedRegisterDecls(const MachineFunction &MF);

  DenseSet<const GlobalVariable *> GlobalsPrintedAsDecls;
  BitVector EmittedGPRDecls = BitVector(32);
  bool EmittedZeroDecl = false;
  bool EmittedSPDecl = false;
  bool EmittedFPDecl = false;
  bool EmittedRADecl = false;
  MCOperand lowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym);
  MCOperand lowerGlobalAddressOperand(const MachineOperand &MO);
};

void FitsAsmPrinter::emitStartOfAsmFile(Module &M) {
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

void FitsAsmPrinter::emitRegisterDecl(StringRef Name) {
  SmallString<16> Line;
  raw_svector_ostream OS(Line);
  OS << Name << " = 0";
  OutStreamer->emitRawText(OS.str());
}

void FitsAsmPrinter::collectUsedRegisters(const MachineFunction &MF,
                                          BitVector &UsedGPR, bool &NeedZero,
                                          bool &NeedSP, bool &NeedFP,
                                          bool &NeedRA) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  if (MFI.getObjectIndexEnd() != 0)
    NeedSP = true;

  for (const MachineBasicBlock &MBB : MF) {
    for (const MachineInstr &MI : MBB) {
      for (const MachineOperand &MO : MI.operands()) {
        if (MO.isReg()) {
          Register Reg = MO.getReg();
          if (!Reg || !Reg.isPhysical())
            continue;

          if (Reg == Fits::ZERO) {
            NeedZero = true;
          } else if (Reg >= Fits::R0 && Reg <= Fits::R31) {
            UsedGPR.set(Reg - Fits::R0);
          } else if (Reg == Fits::SP) {
            NeedSP = true;
          } else if (Reg == Fits::FP) {
            NeedFP = true;
          } else if (Reg == Fits::RA) {
            NeedRA = true;
          }
          continue;
        }

        if (MO.getType() == MachineOperand::MO_ExternalSymbol) {
          StringRef Sym = MO.getSymbolName();
          if (Sym == "__zero")
            NeedZero = true;
          else if (Sym == "__sp")
            NeedSP = true;
          else if (Sym == "__fp")
            NeedFP = true;
          else if (Sym == "__ra")
            NeedRA = true;
        }
      }
    }
  }
}

void FitsAsmPrinter::emitUsedRegisterDecls(const MachineFunction &MF) {
  BitVector UsedGPR(32);
  bool NeedZero = false;
  bool NeedSP = false;
  bool NeedFP = false;
  bool NeedRA = false;
  collectUsedRegisters(MF, UsedGPR, NeedZero, NeedSP, NeedFP, NeedRA);

  bool EmittedAny = false;
  if (NeedZero && !EmittedZeroDecl) {
    emitRegisterDecl("__zero");
    EmittedZeroDecl = true;
    EmittedAny = true;
  }

  for (unsigned I = 0; I != 32; ++I) {
    if (!UsedGPR.test(I) || EmittedGPRDecls.test(I))
      continue;

    SmallString<16> Name;
    raw_svector_ostream OS(Name);
    OS << "__r" << I;
    emitRegisterDecl(OS.str());
    EmittedGPRDecls.set(I);
    EmittedAny = true;
  }

  if (NeedSP && !EmittedSPDecl) {
    emitRegisterDecl("__sp");
    EmittedSPDecl = true;
    EmittedAny = true;
  }
  if (NeedFP && !EmittedFPDecl) {
    emitRegisterDecl("__fp");
    EmittedFPDecl = true;
    EmittedAny = true;
  }
  if (NeedRA && !EmittedRADecl) {
    emitRegisterDecl("__ra");
    EmittedRADecl = true;
    EmittedAny = true;
  }

  if (EmittedAny)
    OutStreamer->addBlankLine();
}

bool FitsAsmPrinter::runOnMachineFunction(MachineFunction &MF) {
  SetupMachineFunction(MF);
  emitUsedRegisterDecls(MF);
  emitFunctionBody();
  return false;
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
