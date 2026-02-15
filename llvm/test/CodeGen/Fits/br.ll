; RUN: llc -mtriple=fits -O0 %s -o - | FileCheck %s

@x = global i32 5

define void @_start() {
entry:
  %v = load volatile i32, ptr @x
  %cmp = icmp slt i32 %v, 10
  br i1 %cmp, label %then, label %else

then:
  store volatile i32 1, ptr @x
  br label %exit

else:
  store volatile i32 2, ptr @x
  br label %exit

exit:
  ret void
}

; CHECK-LABEL: _start:
; CHECK: jmp0 {{.*}} LBB0_2
; CHECK-NEXT: jmp LBB0_1
; CHECK-LABEL: LBB0_1:
; CHECK: set {{.*}} 1
; CHECK: store x
; CHECK: jmp LBB0_3
; CHECK-LABEL: LBB0_2:
; CHECK: set {{.*}} 2
; CHECK: store x
; CHECK: jmp LBB0_3
