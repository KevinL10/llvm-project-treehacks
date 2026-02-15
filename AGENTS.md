I want to implement an LLVM backend from a subset of C to a simple custom assembly language. Both are open to modifications in order to simplify the compiler design.

The LLVM backend should be developed in llvm/lib/Target/Fits.

In particular, the subset of C should be restricted to the following:
- while/for/if statements
- no functions
- int64_t locals and globals
- arrays of int64_t (word addressing)
- no structs/unions, no floats, no varargs, no malloc, no function pointers

The assemby language is described below. Note that there is no explicit concept of registers - all operations are performed on memory addresses.
```
Run `bun asm` to assemble src/assembler.js into out.sheet.

Parameters starting with & means they are memory addresses rather than the concrete value.
Parameters starting with % means they should be treated as the address itself in the normal
(constant variant), and as an &address in the "_a" (address) variant.

The prefix * implies reading the value at the memory address (dereferencing).
Memory addresses are in the form of row[col] since memory is 2D.

Unless otherwise specified, the program counter (PC) increments after every instruction.

lte &out &a &b
    If *a <= *b, stores 1 at out[0], otherwise stores 0.

add &out &a &b
    Stores the result of adding *a + *b at out[0].

mul &out &a &b
    Stores the result of multiplying *a * *b at out[0].

sub &out &a &b
    Stores the result of *a - *b at out[0].

set &dest data
    Stores data at dest[0].

load(_a) &out %row &col
    Read the value at (*row)[*col] and store it at out[0].
    Note that this reads the values at &row and &col and uses those values as the addresses.
    In the future it might be better for &row to be a concrete address instead of a reference.

store(_a) %row &col &in
    Store *in at (*row)[*col]. See load for further notes.

jmp0(_a) &data %dest
    If *data == 0, sets the PC to *dest, otherwise increments it as usual.

jmp(_a) %dest
    Sets the PC to *dest.

halt
    Stops the PC from increasing, halting the program's execution.
```

Example program:
```
n = 50
out = 0

terms = 0 1
terms_len = 2
i = 2

temp1 = 0
temp2 = 0

neg1 = -1
neg2 = -2
pos1 = 1

_start:
loop:
    lte temp1 i n
    jmp0 temp1 after_loop
    add temp1 i neg1
    add temp2 i neg2
    load temp1 terms temp1
    load temp2 terms temp2
    add temp1 temp1 temp2
    store terms terms_len temp1
    add terms_len terms_len pos1
    add i i pos1
    jmp loop
after_loop:
    load out terms n
    halt
```


Notes:
- you should install any necessary tools and libraries


Docs:
- https://llvm.org/docs/CodeGenerator.html
- https://llvm.org/docs/WritingAnLLVMBackend.html


Your goal is to have an LLVM backend (fits) that can take the fib.c program and return the compiled assembly - make sure to actually run your code and verify the outputs.

When making changes, do not include any unnecessary or extra changes to the codebase.


