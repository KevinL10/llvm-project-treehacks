/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Target Register Enum Values                                                *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/


#ifdef GET_REGINFO_ENUM
#undef GET_REGINFO_ENUM

namespace llvm {

class MCRegisterClass;
extern const MCRegisterClass FitsMCRegisterClasses[];

namespace Fits {
enum : unsigned {
  NoRegister,
  FP = 1,
  PC = 2,
  RA = 3,
  SP = 4,
  ZERO = 5,
  R0 = 6,
  R1 = 7,
  R2 = 8,
  R3 = 9,
  R4 = 10,
  R5 = 11,
  R6 = 12,
  R7 = 13,
  R8 = 14,
  R9 = 15,
  R10 = 16,
  R11 = 17,
  R12 = 18,
  R13 = 19,
  R14 = 20,
  R15 = 21,
  R16 = 22,
  R17 = 23,
  R18 = 24,
  R19 = 25,
  R20 = 26,
  R21 = 27,
  R22 = 28,
  R23 = 29,
  R24 = 30,
  R25 = 31,
  R26 = 32,
  R27 = 33,
  R28 = 34,
  R29 = 35,
  R30 = 36,
  R31 = 37,
  NUM_TARGET_REGS // 38
};
} // end namespace Fits

// Register classes

namespace Fits {
enum {
  GPRAllRegClassID = 0,
  GPRAddrRegClassID = 1,
  GPRRegClassID = 2,
  PCRegRegClassID = 3,
  SPRegRegClassID = 4,

};
} // end namespace Fits

// Register pressure sets enum.
namespace Fits {
enum RegisterPressureSets {
  GPRAll = 0,
};
} // end namespace Fits

} // end namespace llvm

#endif // GET_REGINFO_ENUM

/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* MC Register Information                                                    *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/


#ifdef GET_REGINFO_MC_DESC
#undef GET_REGINFO_MC_DESC

namespace llvm {

extern const int16_t FitsRegDiffLists[] = {
  /* 0 */ 0,
};

extern const LaneBitmask FitsLaneMaskLists[] = {
  /* 0 */ LaneBitmask(0xFFFFFFFFFFFFFFFF), 
};

extern const uint16_t FitsSubRegIdxLists[] = {
  /* 0 */ 
  /* dummy */ 0
};


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverlength-strings"
#endif
extern const char FitsRegStrings[] = {
  /* 0 */ "R10\000"
  /* 4 */ "R20\000"
  /* 8 */ "R30\000"
  /* 12 */ "R0\000"
  /* 15 */ "R11\000"
  /* 19 */ "R21\000"
  /* 23 */ "R31\000"
  /* 27 */ "R1\000"
  /* 30 */ "R12\000"
  /* 34 */ "R22\000"
  /* 38 */ "R2\000"
  /* 41 */ "R13\000"
  /* 45 */ "R23\000"
  /* 49 */ "R3\000"
  /* 52 */ "R14\000"
  /* 56 */ "R24\000"
  /* 60 */ "R4\000"
  /* 63 */ "R15\000"
  /* 67 */ "R25\000"
  /* 71 */ "R5\000"
  /* 74 */ "R16\000"
  /* 78 */ "R26\000"
  /* 82 */ "R6\000"
  /* 85 */ "R17\000"
  /* 89 */ "R27\000"
  /* 93 */ "R7\000"
  /* 96 */ "R18\000"
  /* 100 */ "R28\000"
  /* 104 */ "R8\000"
  /* 107 */ "R19\000"
  /* 111 */ "R29\000"
  /* 115 */ "R9\000"
  /* 118 */ "RA\000"
  /* 121 */ "PC\000"
  /* 124 */ "ZERO\000"
  /* 129 */ "FP\000"
  /* 132 */ "SP\000"
};
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

extern const MCRegisterDesc FitsRegDesc[] = { // Descriptors
  { 3, 0, 0, 0, 0, 0, 0, 0 },
  { 129, 0, 0, 0, 0, 0, 0, 0 },
  { 121, 0, 0, 0, 1, 0, 0, 0 },
  { 118, 0, 0, 0, 2, 0, 0, 0 },
  { 132, 0, 0, 0, 3, 0, 0, 0 },
  { 124, 0, 0, 0, 4, 0, 1, 0 },
  { 12, 0, 0, 0, 5, 0, 0, 0 },
  { 27, 0, 0, 0, 6, 0, 0, 0 },
  { 38, 0, 0, 0, 7, 0, 0, 0 },
  { 49, 0, 0, 0, 8, 0, 0, 0 },
  { 60, 0, 0, 0, 9, 0, 0, 0 },
  { 71, 0, 0, 0, 10, 0, 0, 0 },
  { 82, 0, 0, 0, 11, 0, 0, 0 },
  { 93, 0, 0, 0, 12, 0, 0, 0 },
  { 104, 0, 0, 0, 13, 0, 0, 0 },
  { 115, 0, 0, 0, 14, 0, 0, 0 },
  { 0, 0, 0, 0, 15, 0, 0, 0 },
  { 15, 0, 0, 0, 16, 0, 0, 0 },
  { 30, 0, 0, 0, 17, 0, 0, 0 },
  { 41, 0, 0, 0, 18, 0, 0, 0 },
  { 52, 0, 0, 0, 19, 0, 0, 0 },
  { 63, 0, 0, 0, 20, 0, 0, 0 },
  { 74, 0, 0, 0, 21, 0, 0, 0 },
  { 85, 0, 0, 0, 22, 0, 0, 0 },
  { 96, 0, 0, 0, 23, 0, 0, 0 },
  { 107, 0, 0, 0, 24, 0, 0, 0 },
  { 4, 0, 0, 0, 25, 0, 0, 0 },
  { 19, 0, 0, 0, 26, 0, 0, 0 },
  { 34, 0, 0, 0, 27, 0, 0, 0 },
  { 45, 0, 0, 0, 28, 0, 0, 0 },
  { 56, 0, 0, 0, 29, 0, 0, 0 },
  { 67, 0, 0, 0, 30, 0, 0, 0 },
  { 78, 0, 0, 0, 31, 0, 0, 0 },
  { 89, 0, 0, 0, 32, 0, 0, 0 },
  { 100, 0, 0, 0, 33, 0, 0, 0 },
  { 111, 0, 0, 0, 34, 0, 0, 0 },
  { 8, 0, 0, 0, 35, 0, 0, 0 },
  { 23, 0, 0, 0, 36, 0, 0, 0 },
};

extern const MCPhysReg FitsRegUnitRoots[][2] = {
  { Fits::FP },
  { Fits::PC },
  { Fits::RA },
  { Fits::SP },
  { Fits::ZERO },
  { Fits::R0 },
  { Fits::R1 },
  { Fits::R2 },
  { Fits::R3 },
  { Fits::R4 },
  { Fits::R5 },
  { Fits::R6 },
  { Fits::R7 },
  { Fits::R8 },
  { Fits::R9 },
  { Fits::R10 },
  { Fits::R11 },
  { Fits::R12 },
  { Fits::R13 },
  { Fits::R14 },
  { Fits::R15 },
  { Fits::R16 },
  { Fits::R17 },
  { Fits::R18 },
  { Fits::R19 },
  { Fits::R20 },
  { Fits::R21 },
  { Fits::R22 },
  { Fits::R23 },
  { Fits::R24 },
  { Fits::R25 },
  { Fits::R26 },
  { Fits::R27 },
  { Fits::R28 },
  { Fits::R29 },
  { Fits::R30 },
  { Fits::R31 },
};

namespace {     // Register classes...
  // GPRAll Register Class...
  const MCPhysReg GPRAll[] = {
    Fits::ZERO, Fits::R0, Fits::R1, Fits::R2, Fits::R3, Fits::R4, Fits::R5, Fits::R6, Fits::R7, Fits::R8, Fits::R9, Fits::R10, Fits::R11, Fits::R12, Fits::R13, Fits::R14, Fits::R15, Fits::R16, Fits::R17, Fits::R18, Fits::R19, Fits::R20, Fits::R21, Fits::R22, Fits::R23, Fits::R24, Fits::R25, Fits::R26, Fits::R27, Fits::R28, Fits::R29, Fits::R30, Fits::R31, Fits::SP, Fits::FP, Fits::RA, 
  };

  // GPRAll Bit set.
  const uint8_t GPRAllBits[] = {
    0xfa, 0xff, 0xff, 0xff, 0x3f, 
  };

  // GPRAddr Register Class...
  const MCPhysReg GPRAddr[] = {
    Fits::R0, Fits::R1, Fits::R2, Fits::R3, Fits::R4, Fits::R5, Fits::R6, Fits::R7, Fits::R8, Fits::R9, Fits::R10, Fits::R11, Fits::R12, Fits::R13, Fits::R14, Fits::R15, Fits::R16, Fits::R17, Fits::R18, Fits::R19, Fits::R20, Fits::R21, Fits::R22, Fits::R23, Fits::R24, Fits::R25, Fits::R26, Fits::R27, Fits::R28, Fits::R29, Fits::R30, Fits::R31, Fits::SP, Fits::FP, 
  };

  // GPRAddr Bit set.
  const uint8_t GPRAddrBits[] = {
    0xd2, 0xff, 0xff, 0xff, 0x3f, 
  };

  // GPR Register Class...
  const MCPhysReg GPR[] = {
    Fits::R0, Fits::R1, Fits::R2, Fits::R3, Fits::R4, Fits::R5, Fits::R6, Fits::R7, Fits::R8, Fits::R9, Fits::R10, Fits::R11, Fits::R12, Fits::R13, Fits::R14, Fits::R15, Fits::R16, Fits::R17, Fits::R18, Fits::R19, Fits::R20, Fits::R21, Fits::R22, Fits::R23, Fits::R24, Fits::R25, Fits::R26, Fits::R27, Fits::R28, Fits::R29, Fits::R30, Fits::R31, 
  };

  // GPR Bit set.
  const uint8_t GPRBits[] = {
    0xc0, 0xff, 0xff, 0xff, 0x3f, 
  };

  // PCReg Register Class...
  const MCPhysReg PCReg[] = {
    Fits::PC, 
  };

  // PCReg Bit set.
  const uint8_t PCRegBits[] = {
    0x04, 
  };

  // SPReg Register Class...
  const MCPhysReg SPReg[] = {
    Fits::SP, 
  };

  // SPReg Bit set.
  const uint8_t SPRegBits[] = {
    0x10, 
  };

} // end anonymous namespace


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverlength-strings"
#endif
extern const char FitsRegClassStrings[] = {
  /* 0 */ "GPR\000"
  /* 4 */ "PCReg\000"
  /* 10 */ "SPReg\000"
  /* 16 */ "GPRAll\000"
  /* 23 */ "GPRAddr\000"
};
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

extern const MCRegisterClass FitsMCRegisterClasses[] = {
  { GPRAll, GPRAllBits, 16, 36, sizeof(GPRAllBits), Fits::GPRAllRegClassID, 32, 1, true, false },
  { GPRAddr, GPRAddrBits, 23, 34, sizeof(GPRAddrBits), Fits::GPRAddrRegClassID, 32, 1, true, false },
  { GPR, GPRBits, 0, 32, sizeof(GPRBits), Fits::GPRRegClassID, 32, 1, true, false },
  { PCReg, PCRegBits, 4, 1, sizeof(PCRegBits), Fits::PCRegRegClassID, 32, 1, false, false },
  { SPReg, SPRegBits, 10, 1, sizeof(SPRegBits), Fits::SPRegRegClassID, 32, 1, false, false },
};

extern const uint16_t FitsRegEncodingTable[] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
};
static inline void InitFitsMCRegisterInfo(MCRegisterInfo *RI, unsigned RA, unsigned DwarfFlavour = 0, unsigned EHFlavour = 0, unsigned PC = 0) {
  RI->InitMCRegisterInfo(FitsRegDesc, 38, RA, PC, FitsMCRegisterClasses, 5, FitsRegUnitRoots, 37, FitsRegDiffLists, FitsLaneMaskLists, FitsRegStrings, FitsRegClassStrings, FitsSubRegIdxLists, 1,
FitsRegEncodingTable);

}

} // end namespace llvm

#endif // GET_REGINFO_MC_DESC

/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Register Information Header Fragment                                       *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/


#ifdef GET_REGINFO_HEADER
#undef GET_REGINFO_HEADER

#include "llvm/CodeGen/TargetRegisterInfo.h"

namespace llvm {

class FitsFrameLowering;

struct FitsGenRegisterInfo : public TargetRegisterInfo {
  explicit FitsGenRegisterInfo(unsigned RA, unsigned D = 0, unsigned E = 0,
      unsigned PC = 0, unsigned HwMode = 0);
  const RegClassWeight &getRegClassWeight(const TargetRegisterClass *RC) const override;
  unsigned getRegUnitWeight(unsigned RegUnit) const override;
  unsigned getNumRegPressureSets() const override;
  const char *getRegPressureSetName(unsigned Idx) const override;
  unsigned getRegPressureSetLimit(const MachineFunction &MF, unsigned Idx) const override;
  const int *getRegClassPressureSets(const TargetRegisterClass *RC) const override;
  const int *getRegUnitPressureSets(unsigned RegUnit) const override;
  ArrayRef<const char *> getRegMaskNames() const override;
  ArrayRef<const uint32_t *> getRegMasks() const override;
  bool isGeneralPurposeRegister(const MachineFunction &, MCRegister) const override;
  bool isGeneralPurposeRegisterClass(const TargetRegisterClass *RC) const override;
  bool isFixedRegister(const MachineFunction &, MCRegister) const override;
  bool isArgumentRegister(const MachineFunction &, MCRegister) const override;
  bool isConstantPhysReg(MCRegister PhysReg) const override final;
  /// Devirtualized TargetFrameLowering.
  static const FitsFrameLowering *getFrameLowering(
      const MachineFunction &MF);
};

namespace Fits { // Register classes
  extern const TargetRegisterClass GPRAllRegClass;
  extern const TargetRegisterClass GPRAddrRegClass;
  extern const TargetRegisterClass GPRRegClass;
  extern const TargetRegisterClass PCRegRegClass;
  extern const TargetRegisterClass SPRegRegClass;
} // end namespace Fits

} // end namespace llvm

#endif // GET_REGINFO_HEADER

/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Target Register and Register Classes Information                           *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/


#ifdef GET_REGINFO_TARGET_DESC
#undef GET_REGINFO_TARGET_DESC

namespace llvm {

extern const MCRegisterClass FitsMCRegisterClasses[];

static const MVT::SimpleValueType VTLists[] = {
  /* 0 */ MVT::i32, MVT::Other,
};

static const char *SubRegIndexNameTable[] = { "" };

static const TargetRegisterInfo::SubRegCoveredBits SubRegIdxRangeTable[] = {
  { 65535, 65535 },
};


static const LaneBitmask SubRegIndexLaneMaskTable[] = {
  LaneBitmask::getAll(),
 };



static const TargetRegisterInfo::RegClassInfo RegClassInfos[] = {
  // Mode = 0 (Default)
  { 32, 32, 32, /*VTLists+*/0 },    // GPRAll
  { 32, 32, 32, /*VTLists+*/0 },    // GPRAddr
  { 32, 32, 32, /*VTLists+*/0 },    // GPR
  { 32, 32, 32, /*VTLists+*/0 },    // PCReg
  { 32, 32, 32, /*VTLists+*/0 },    // SPReg
};
static const uint32_t GPRAllSubClassMask[] = {
  0x00000017, 
};

static const uint32_t GPRAddrSubClassMask[] = {
  0x00000016, 
};

static const uint32_t GPRSubClassMask[] = {
  0x00000004, 
};

static const uint32_t PCRegSubClassMask[] = {
  0x00000008, 
};

static const uint32_t SPRegSubClassMask[] = {
  0x00000010, 
};

static const uint16_t SuperRegIdxSeqs[] = {
  /* 0 */ 0,
};

static unsigned const GPRAddrSuperclasses[] = {
  Fits::GPRAllRegClassID,
};

static unsigned const GPRSuperclasses[] = {
  Fits::GPRAllRegClassID,
  Fits::GPRAddrRegClassID,
};

static unsigned const SPRegSuperclasses[] = {
  Fits::GPRAllRegClassID,
  Fits::GPRAddrRegClassID,
};


namespace Fits {   // Register class instances
  extern const TargetRegisterClass GPRAllRegClass = {
    &FitsMCRegisterClasses[GPRAllRegClassID],
    GPRAllSubClassMask,
    SuperRegIdxSeqs + 0,
    LaneBitmask(0x0000000000000001),
    0,
    false,
    0x00, /* TSFlags */
    false, /* HasDisjunctSubRegs */
    false, /* CoveredBySubRegs */
    nullptr, 0,
    nullptr
  };

  extern const TargetRegisterClass GPRAddrRegClass = {
    &FitsMCRegisterClasses[GPRAddrRegClassID],
    GPRAddrSubClassMask,
    SuperRegIdxSeqs + 0,
    LaneBitmask(0x0000000000000001),
    0,
    false,
    0x00, /* TSFlags */
    false, /* HasDisjunctSubRegs */
    false, /* CoveredBySubRegs */
    GPRAddrSuperclasses,  1,
    nullptr
  };

  extern const TargetRegisterClass GPRRegClass = {
    &FitsMCRegisterClasses[GPRRegClassID],
    GPRSubClassMask,
    SuperRegIdxSeqs + 0,
    LaneBitmask(0x0000000000000001),
    0,
    false,
    0x00, /* TSFlags */
    false, /* HasDisjunctSubRegs */
    false, /* CoveredBySubRegs */
    GPRSuperclasses,  2,
    nullptr
  };

  extern const TargetRegisterClass PCRegRegClass = {
    &FitsMCRegisterClasses[PCRegRegClassID],
    PCRegSubClassMask,
    SuperRegIdxSeqs + 0,
    LaneBitmask(0x0000000000000001),
    0,
    false,
    0x00, /* TSFlags */
    false, /* HasDisjunctSubRegs */
    false, /* CoveredBySubRegs */
    nullptr, 0,
    nullptr
  };

  extern const TargetRegisterClass SPRegRegClass = {
    &FitsMCRegisterClasses[SPRegRegClassID],
    SPRegSubClassMask,
    SuperRegIdxSeqs + 0,
    LaneBitmask(0x0000000000000001),
    0,
    false,
    0x00, /* TSFlags */
    false, /* HasDisjunctSubRegs */
    false, /* CoveredBySubRegs */
    SPRegSuperclasses,  2,
    nullptr
  };

} // end namespace Fits

namespace {
  const TargetRegisterClass *const RegisterClasses[] = {
    &Fits::GPRAllRegClass,
    &Fits::GPRAddrRegClass,
    &Fits::GPRRegClass,
    &Fits::PCRegRegClass,
    &Fits::SPRegRegClass,
  };
} // end anonymous namespace

static const uint8_t CostPerUseTable[] = { 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };


static const bool InAllocatableClassTable[] = { 
false, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, };


static const TargetRegisterInfoDesc FitsRegInfoDesc = { // Extra Descriptors
CostPerUseTable, 1, InAllocatableClassTable};

/// Get the weight in units of pressure for this register class.
const RegClassWeight &FitsGenRegisterInfo::
getRegClassWeight(const TargetRegisterClass *RC) const {
  static const RegClassWeight RCWeightTable[] = {
    {1, 36},  	// GPRAll
    {1, 34},  	// GPRAddr
    {1, 32},  	// GPR
    {0, 0},  	// PCReg
    {1, 1},  	// SPReg
  };
  return RCWeightTable[RC->getID()];
}

/// Get the weight in units of pressure for this register unit.
unsigned FitsGenRegisterInfo::
getRegUnitWeight(unsigned RegUnit) const {
  assert(RegUnit < 37 && "invalid register unit");
  // All register units have unit weight.
  return 1;
}


// Get the number of dimensions of register pressure.
unsigned FitsGenRegisterInfo::getNumRegPressureSets() const {
  return 1;
}

// Get the name of this register unit pressure set.
const char *FitsGenRegisterInfo::
getRegPressureSetName(unsigned Idx) const {
  static const char *PressureNameTable[] = {
    "GPRAll",
  };
  return PressureNameTable[Idx];
}

// Get the register unit pressure limit for this dimension.
// This limit must be adjusted dynamically for reserved registers.
unsigned FitsGenRegisterInfo::
getRegPressureSetLimit(const MachineFunction &MF, unsigned Idx) const {
  static const uint8_t PressureLimitTable[] = {
    36,  	// 0: GPRAll
  };
  return PressureLimitTable[Idx];
}

/// Table of pressure sets per register class or unit.
static const int RCSetsTable[] = {
  /* 0 */ 0, -1,
};

/// Get the dimensions of register pressure impacted by this register class.
/// Returns a -1 terminated array of pressure set IDs
const int *FitsGenRegisterInfo::
getRegClassPressureSets(const TargetRegisterClass *RC) const {
  static const uint8_t RCSetStartTable[] = {
    0,0,0,1,1,};
  return &RCSetsTable[RCSetStartTable[RC->getID()]];
}

/// Get the dimensions of register pressure impacted by this register unit.
/// Returns a -1 terminated array of pressure set IDs
const int *FitsGenRegisterInfo::
getRegUnitPressureSets(unsigned RegUnit) const {
  assert(RegUnit < 37 && "invalid register unit");
  static const uint8_t RUSetStartTable[] = {
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
  return &RCSetsTable[RUSetStartTable[RegUnit]];
}

extern const MCRegisterDesc FitsRegDesc[];
extern const int16_t FitsRegDiffLists[];
extern const LaneBitmask FitsLaneMaskLists[];
extern const char FitsRegStrings[];
extern const char FitsRegClassStrings[];
extern const MCPhysReg FitsRegUnitRoots[][2];
extern const uint16_t FitsSubRegIdxLists[];
extern const uint16_t FitsRegEncodingTable[];
FitsGenRegisterInfo::
FitsGenRegisterInfo(unsigned RA, unsigned DwarfFlavour, unsigned EHFlavour,
      unsigned PC, unsigned HwMode)
  : TargetRegisterInfo(&FitsRegInfoDesc, RegisterClasses, RegisterClasses+5,
             SubRegIndexNameTable, SubRegIdxRangeTable, SubRegIndexLaneMaskTable,
             LaneBitmask(0xFFFFFFFFFFFFFFFF), RegClassInfos, VTLists, HwMode) {
  InitMCRegisterInfo(FitsRegDesc, 38, RA, PC,
                     FitsMCRegisterClasses, 5,
                     FitsRegUnitRoots,
                     37,
                     FitsRegDiffLists,
                     FitsLaneMaskLists,
                     FitsRegStrings,
                     FitsRegClassStrings,
                     FitsSubRegIdxLists,
                     1,
                     FitsRegEncodingable);

}



ArrayRef<const uint32_t *> FitsGenRegisterInfo::getRegMasks() const {
  return {};
}

bool FitsGenRegisterInfo::
isGeneralPurposeRegister(const MachineFunction &MF, MCRegister PhysReg) const {
  return
      false;
}

bool FitsGenRegisterInfo::
isGeneralPurposeRegisterClass(const TargetRegisterClass *RC) const {
  return
      false;
}

bool FitsGenRegisterInfo::
isFixedRegister(const MachineFunction &MF, MCRegister PhysReg) const {
  return
      false;
}

bool FitsGenRegisterInfo::
isArgumentRegister(const MachineFunction &MF, MCRegister PhysReg) const {
  return
      false;
}

bool FitsGenRegisterInfo::
isConstantPhysReg(MCRegister PhysReg) const {
  return
      PhysReg == Fits::ZERO ||
      false;
}

ArrayRef<const char *> FitsGenRegisterInfo::getRegMaskNames() const {
  return {};
}

const FitsFrameLowering *
FitsGenRegisterInfo::getFrameLowering(const MachineFunction &MF) {
  return static_cast<const FitsFrameLowering *>(
      MF.getSubtarget().getFrameLowering());
}

} // end namespace llvm

#endif // GET_REGINFO_TARGET_DESC

