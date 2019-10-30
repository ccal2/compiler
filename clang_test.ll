; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.15.0"

@nom_nom = global i32 88, align 4
@arroz = common global i32 0, align 4

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @aqua_baldo() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 160, i32* %1, align 4
  %4 = load i32, i32* %1, align 4
  %5 = srem i32 %4, 11
  %6 = sub nsw i32 %5, 2
  store i32 %6, i32* %2, align 4
  %7 = load i32, i32* %2, align 4
  %8 = add nsw i32 6, %7
  %9 = srem i32 11, %8
  store i32 %9, i32* %3, align 4
  %10 = load i32, i32* %1, align 4
  %11 = sub nsw i32 1, %10
  %12 = load i32, i32* %2, align 4
  %13 = load i32, i32* %3, align 4
  %14 = srem i32 %12, %13
  %15 = add nsw i32 %11, %14
  ret i32 %15
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @function7(i32, i32) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  store i32 %1, i32* %4, align 4
  %6 = load i32, i32* %3, align 4
  %7 = load i32, i32* %4, align 4
  %8 = srem i32 %6, %7
  store i32 %8, i32* %5, align 4
  %9 = load i32, i32* %3, align 4
  %10 = sub nsw i32 0, %9
  %11 = load i32, i32* %4, align 4
  %12 = load i32, i32* %5, align 4
  %13 = add nsw i32 %11, %12
  %14 = mul nsw i32 %10, %13
  store i32 %14, i32* %4, align 4
  %15 = load i32, i32* %5, align 4
  %16 = load i32, i32* %4, align 4
  %17 = sdiv i32 %16, 2
  %18 = sub nsw i32 %15, %17
  ret i32 %18
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @kk(i32) #0 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = sdiv i32 %3, 2
  store i32 %4, i32* @arroz, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @aster(i32, i32, i32) #0 {
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 %0, i32* %4, align 4
  store i32 %1, i32* %5, align 4
  store i32 %2, i32* %6, align 4
  %8 = load i32, i32* %4, align 4
  %9 = mul nsw i32 3, %8
  %10 = load i32, i32* %5, align 4
  %11 = mul nsw i32 5, %10
  %12 = add nsw i32 %9, %11
  %13 = load i32, i32* %6, align 4
  %14 = mul nsw i32 2, %13
  %15 = add nsw i32 %12, %14
  %16 = sdiv i32 %15, 10
  store i32 %16, i32* %7, align 4
  %17 = load i32, i32* %4, align 4
  %18 = mul nsw i32 255, %17
  %19 = load i32, i32* %7, align 4
  %20 = sdiv i32 %18, %19
  store i32 %20, i32* %4, align 4
  %21 = load i32, i32* %5, align 4
  %22 = mul nsw i32 255, %21
  %23 = load i32, i32* %7, align 4
  %24 = sdiv i32 %22, %23
  store i32 %24, i32* %5, align 4
  %25 = load i32, i32* %6, align 4
  %26 = sub nsw i32 %25, 127
  %27 = mul nsw i32 255, %26
  %28 = load i32, i32* %7, align 4
  %29 = sdiv i32 %27, %28
  store i32 %29, i32* %6, align 4
  %30 = load i32, i32* %4, align 4
  %31 = load i32, i32* %5, align 4
  %32 = add nsw i32 %30, %31
  %33 = load i32, i32* %6, align 4
  %34 = add nsw i32 %32, %33
  ret i32 %34
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %4 = call i32 @aster(i32 123, i32 111, i32 240)
  store i32 %4, i32* %3, align 4
  %5 = call i32 @aqua_baldo()
  store i32 %5, i32* @nom_nom, align 4
  %6 = load i32, i32* %3, align 4
  call void @kk(i32 %6)
  %7 = call i32 @function7(i32 7, i32 38)
  %8 = load i32, i32* @arroz, align 4
  %9 = sub nsw i32 %7, %8
  store i32 %9, i32* %2, align 4
  ret i32 0
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "darwin-stkchk-strong-link" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "probe-stack"="___chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 10, i32 15]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 7, !"PIC Level", i32 2}
!3 = !{!"Apple clang version 11.0.0 (clang-1100.0.33.8)"}
