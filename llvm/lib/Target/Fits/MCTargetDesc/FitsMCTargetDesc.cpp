#include "FitsMCTargetDesc.h"
#include "FitsTargetInfo.h"
#include "MCTargetDesc/FitsMCAsmInfo.h"
#include "MCTargetDesc/FitsMCInstPrinter.h"

#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "FitsGenInstrInfo.inc"

#define GET_REGINFO_MC_DESC
#include "FitsGenRegisterInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "FitsGenSubtargetInfo.inc"

namespace {
class FitsAsmStreamer final : public MCStreamer {
  std::unique_ptr<formatted_raw_ostream> FOSOwner;
  formatted_raw_ostream &OS;
  const MCAsmInfo *MAI;
  std::unique_ptr<MCInstPrinter> InstPrinter;

public:
  FitsAsmStreamer(MCContext &Context,
                  std::unique_ptr<formatted_raw_ostream> Out,
                  std::unique_ptr<MCInstPrinter> Printer,
                  std::unique_ptr<MCCodeEmitter> CE,
                  std::unique_ptr<MCAsmBackend> TAB)
      : MCStreamer(Context), FOSOwner(std::move(Out)), OS(*FOSOwner),
        MAI(Context.getAsmInfo()), InstPrinter(std::move(Printer)) {
    (void)CE;
    (void)TAB;
    assert(InstPrinter && "Fits streamer requires an instruction printer");
    Context.setUseNamesOnTempLabels(true);
  }

  bool hasRawTextSupport() const override { return true; }

  void changeSection(MCSection *Section, uint32_t Subsection) override {
    // Keep MCStreamer state in sync but intentionally suppress textual section
    // directives like ".text".
    MCStreamer::changeSection(Section, Subsection);
  }

  void emitInstruction(const MCInst &Inst,
                       const MCSubtargetInfo &STI) override {
    MCStreamer::emitInstruction(Inst, STI);
    InstPrinter->printInst(&Inst, /*Address=*/0, /*Annot=*/"", STI, OS);
    OS << '\n';
  }

  void emitLabel(MCSymbol *Symbol, SMLoc Loc = SMLoc()) override {
    MCStreamer::emitLabel(Symbol, Loc);
    Symbol->print(OS, MAI);
    OS << ":\n";
  }

  bool emitSymbolAttribute(MCSymbol *Symbol,
                           MCSymbolAttr Attribute) override {
    (void)Symbol;
    (void)Attribute;
    return true;
  }

  void emitCommonSymbol(MCSymbol *Symbol, uint64_t Size,
                        Align ByteAlignment) override {
    (void)Symbol;
    (void)Size;
    (void)ByteAlignment;
  }

  void emitRawTextImpl(StringRef String) override {
    OS << String;
    if (!String.empty() && !String.ends_with('\n'))
      OS << '\n';
  }
};
} // namespace

static MCRegisterInfo *createFitsMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitFitsMCRegisterInfo(X, Fits::RA);
  return X;
}


static MCSubtargetInfo *createFitsSubtargetInfo(const Triple &TT, StringRef CPU,
                                                StringRef FS) {
  if (CPU.empty())
    CPU = "generic";
  return createFitsMCSubtargetInfoImpl(TT, CPU, CPU, FS);
}

static MCAsmInfo *createFitsMCAsmInfo(const MCRegisterInfo &MRI,
                                      const Triple &TT,
                                      const MCTargetOptions &Options) {
  MCAsmInfo *X = new FitsMCAsmInfo(TT);
  return X;
}

static MCInstrInfo *createFitsMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitFitsMCInstrInfo(X);
  return X;
}

static MCInstPrinter *createFitsMCInstPrinter(const Triple &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new FitsInstPrinter(MAI, MII, MRI);
}

static MCStreamer *createFitsAsmStreamer(
    MCContext &Ctx, std::unique_ptr<formatted_raw_ostream> OS,
    std::unique_ptr<MCInstPrinter> IP, std::unique_ptr<MCCodeEmitter> CE,
    std::unique_ptr<MCAsmBackend> TAB) {
  return new FitsAsmStreamer(Ctx, std::move(OS), std::move(IP), std::move(CE),
                             std::move(TAB));
}

extern "C" void LLVMInitializeFitsTargetMC() {
  Target *T = &getTheFitsTarget();
  TargetRegistry::RegisterMCRegInfo(*T, createFitsMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(*T, createFitsSubtargetInfo);
  TargetRegistry::RegisterMCAsmInfo(*T, createFitsMCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(*T, createFitsMCInstrInfo);
  TargetRegistry::RegisterMCInstPrinter(*T, createFitsMCInstPrinter);
  TargetRegistry::RegisterAsmStreamer(*T, createFitsAsmStreamer);
}
