; ModuleID = 'fib.c'
source_filename = "fib.c"
target datalayout = "e-m:o-p:32:32-Fi8-f64:32:64-v64:32:64-v128:32:128-a:0:32-n32-S32"
target triple = "armv4t-apple-macosx15.0.0"

@n = global i32 10, align 4
@out = common global [50 x i32] zeroinitializer, align 4

; Function Attrs: noinline nounwind optnone ssp
define i32 @_start() #0 {
  %1 = alloca i32, align 4
  store i32 0, ptr @out, align 4
  store i32 1, ptr getelementptr inbounds ([50 x i32], ptr @out, i32 0, i32 1), align 4
  store i32 2, ptr %1, align 4
  br label %2

2:                                                ; preds = %18, %0
  %3 = load i32, ptr %1, align 4
  %4 = load i32, ptr @n, align 4
  %5 = icmp sle i32 %3, %4
  br i1 %5, label %6, label %21

6:                                                ; preds = %2
  %7 = load i32, ptr %1, align 4
  %8 = sub nsw i32 %7, 1
  %9 = getelementptr inbounds [50 x i32], ptr @out, i32 0, i32 %8
  %10 = load i32, ptr %9, align 4
  %11 = load i32, ptr %1, align 4
  %12 = sub nsw i32 %11, 2
  %13 = getelementptr inbounds [50 x i32], ptr @out, i32 0, i32 %12
  %14 = load i32, ptr %13, align 4
  %15 = add nsw i32 %10, %14
  %16 = load i32, ptr %1, align 4
  %17 = getelementptr inbounds [50 x i32], ptr @out, i32 0, i32 %16
  store i32 %15, ptr %17, align 4
  br label %18

18:                                               ; preds = %6
  %19 = load i32, ptr %1, align 4
  %20 = add nsw i32 %19, 1
  store i32 %20, ptr %1, align 4
  br label %2, !llvm.loop !6

21:                                               ; preds = %2
  %22 = load i32, ptr @n, align 4
  %23 = getelementptr inbounds [50 x i32], ptr @out, i32 0, i32 %22
  %24 = load i32, ptr %23, align 4
  ret i32 %24
}

attributes #0 = { noinline nounwind optnone ssp "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="arm7tdmi" "target-features"="+armv4t,+soft-float,+strict-align,-aes,-bf16,-d32,-dotprod,-fp-armv8,-fp-armv8d16,-fp-armv8d16sp,-fp-armv8sp,-fp16,-fp16fml,-fp64,-fpregs,-fullfp16,-mve,-mve.fp,-neon,-sha2,-thumb-mode,-vfp2,-vfp2sp,-vfp3,-vfp3d16,-vfp3d16sp,-vfp3sp,-vfp4,-vfp4d16,-vfp4d16sp,-vfp4sp" "use-soft-float"="true" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 15, i32 4]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 1, !"min_enum_size", i32 4}
!3 = !{i32 8, !"PIC Level", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Apple clang version 17.0.0 (clang-1700.0.13.3)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
