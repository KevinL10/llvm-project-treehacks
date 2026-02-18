set(LLVM_TARGET_DEFINITIONS Fits.td)

tablegen(LLVM FitsGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM FitsGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM FitsGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM FitsGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM FitsGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM FitsGenSDNodeInfo.inc -gen-sd-node-info)

add_public_tablegen_target(FitsCommonTableGen)

add_llvm_component_group(Fits)

add_llvm_target(FitsCodeGen
  FitsTargetMachine.cpp
  FitsRegisterInfo.cpp
  FitsTargetObjectFile.cpp
  FitsSubtarget.cpp
  FitsInstrInfo.cpp
  FitsISelLowering.cpp
  FitsISelDAGToDag.cpp
  FitsSelectionDAGInfo.cpp
  FitsAsmPrinter.cpp
  MCTargetDesc/FitsMCTargetDesc.cpp
  MCTargetDesc/FitsMCAsmInfo.cpp
  MCTargetDesc/FitsMCInstPrinter.cpp

  LINK_COMPONENTS
  MC
  Target
  TargetParser
  AsmPrinter

  ADD_TO_COMPONENT
  Fits
)
