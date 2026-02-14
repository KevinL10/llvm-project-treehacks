set(LLVM_TARGET_DEFINITIONS Fits.td)

tablegen(LLVM FitsGenRegisterInfo.inc -gen-register-info)
add_public_tablegen_target(FitsCommonTableGen)

add_llvm_component_group(Fits)

add_llvm_target(FitsCodeGen
  FitsTargetMachine.cpp
  FitsRegisterInfo.cpp
  MCTargetDesc/FitsMCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  TargetParser
  Target

  ADD_TO_COMPONENT
  Fits
)

# tablegen(LLVM FitsGenRegisterInfo.inc -gen-register-info)

# cd llvm/lib/Target/Fits  && ../../../../build/bin/llvm-tblgen Fits.td -I=../../../include -gen-register-info > reg.h
