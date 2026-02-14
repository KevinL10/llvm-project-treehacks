// This is the top tablegen file that pulls in other code
// specific to the Fits target.

include "llvm/Target/Target.td"

include "FitsRegisterInfo.td"

def : ProcessorModel<"generic", NoSchedModel, []>;

def Fits : Target {
}
