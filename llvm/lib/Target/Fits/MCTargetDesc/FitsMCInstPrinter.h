#ifndef LLVM_LIB_TARGET_FITS_MCTARGETDESC_FITSMCINSTPRINTER_H
#define LLVM_LIB_TARGET_FITS_MCTARGETDESC_FITSMCINSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCRegister.h"

namespace llvm {
class FitsInstPrinter : public MCInstPrinter {
public:
  FitsInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                  const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

  void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
                 const MCSubtargetInfo &STI, raw_ostream &O) override;

  bool printAliasInstr(const MCInst *MI, uint64_t Address, raw_ostream &OS);

  void printInstruction(const MCInst *MI, uint64_t Address, raw_ostream &O);

  void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);

  void printRegName(raw_ostream &OS, MCRegister RegNo) override;

  const char *getRegisterName(MCRegister Reg);

  std::pair<const char *, uint64_t>
  getMnemonic(const MCInst &MI) const override;
};
} // end namespace llvm

#endif