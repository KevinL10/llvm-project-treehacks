class FitsReg<string n> : Register<n> {
  let Namespace = "Fits";
}

class FitsGPRReg<string n> : FitsReg<n>;

// equivalent of x0
def ZERO : FitsGPRReg<"zero"> {
  let isConstant = 1;
}

foreach i = 0-31 in
  def R#i : FitsGPRReg<"r"#i>;

def SP : FitsGPRReg<"sp">;
def FP : FitsGPRReg<"fp">;
def RA : FitsGPRReg<"ra">;
def PC : FitsReg<"pc">;


// register classes

def GPR : RegisterClass<"Fits", [i32], 32,
  (add (sequence "R%u", 0, 31))> {
  let AllocationPriority = 0;
}

def GPRAddr : RegisterClass<"Fits", [i32], 32,
  (add (sequence "R%u", 0, 31), SP, FP)>;

def GPRAll : RegisterClass<"Fits", [i32], 32,
  (add ZERO, (sequence "R%u", 0, 31), SP, FP, RA)>;

def SPReg : RegisterClass<"Fits", [i32], 32, (add SP)> {
  let isAllocatable = false;
}

def PCReg : RegisterClass<"Fits", [i32], 32, (add PC)> {
  let isAllocatable = false;
}



