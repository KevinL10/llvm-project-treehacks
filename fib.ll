; ModuleID = 'fib.c'
source_filename = "fib.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

@n = global i32 10, align 4
@out = common global [100 x i32] zeroinitializer, align 4

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 0, ptr @out, align 4
  store i32 1, ptr getelementptr inbounds ([100 x i32], ptr @out, i64 0, i64 1), align 4
  store i32 2, ptr %2, align 4
  br label %3

3:                                                ; preds = %22, %0
  %4 = load i32, ptr %2, align 4
  %5 = load i32, ptr @n, align 4
  %6 = icmp slt i32 %4, %5
  br i1 %6, label %7, label %25

7:                                                ; preds = %3
  %8 = load i32, ptr %2, align 4
  %9 = sub nsw i32 %8, 1
  %10 = sext i32 %9 to i64
  %11 = getelementptr inbounds [100 x i32], ptr @out, i64 0, i64 %10
  %12 = load i32, ptr %11, align 4
  %13 = load i32, ptr %2, align 4
  %14 = sub nsw i32 %13, 2
  %15 = sext i32 %14 to i64
  %16 = getelementptr inbounds [100 x i32], ptr @out, i64 0, i64 %15
  %17 = load i32, ptr %16, align 4
  %18 = add nsw i32 %12, %17
  %19 = load i32, ptr %2, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds [100 x i32], ptr @out, i64 0, i64 %20
  store i32 %18, ptr %21, align 4
  br label %22

22:                                               ; preds = %7
  %23 = load i32, ptr %2, align 4
  %24 = add nsw i32 %23, 1
  store i32 %24, ptr %2, align 4
  br label %3, !llvm.loop !6

25:                                               ; preds = %3
  ret i32 0
}

attributes #0 = { noinline nounwind optnone ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+bti,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 15, i32 4]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Apple clang version 17.0.0 (clang-1700.0.13.3)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
