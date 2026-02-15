#!/usr/bin/env bash

cd /Users/kevin/dev/llvm-project-treehacks

echo "--- compiling c to llvm ir"
clang -S -emit-llvm -m32 fib.c

echo "--- building llc"
cmake --build build --target llc

echo "--- compiling llvm ir to asm"
./build/bin/llc -mtriple=fits fib.ll 

cd /Users/kevin/dev/freak-in-the-sheets

echo "--- compiling asm to sheets"
bun asm /Users/kevin/dev/llvm-project-treehacks/fib.s ./out.sheet

echo "--- copying sheets code to clipboard"
pbcopy < /Users/kevin/dev/freak-in-the-sheets/out.sheet