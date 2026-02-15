@out = common global [100 x i32] zeroinitializer, align 4

define i32 @main() {
entry:
  ; Triggers SelectionDAG GlobalAddress materialization through GEP.
  %p = getelementptr inbounds [100 x i32], ptr @out, i64 0, i64 1
  %v = load i32, ptr %p, align 4
  ret i32 %v
}
