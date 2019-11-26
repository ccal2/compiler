@nom_nom = global int32 88, align 4
@arroz = common global int32 0, align 4

define i32 @aqua_baldo() #0 {
	%1 = alloca i32, align 4
	store i32 160, i32 %1, align 4
	%2 = alloca i32, align 4
	%3 = load i32, i32* %1, align 4
	%4 = srem i32 %3, 11
	%5 = sub nsw i32 %4, 2
	store i32 %5, i32 %2, align 4
	%6 = alloca i32, align 4
	%7 = load i32, i32* %2, align 4
	%8 = add nsw i32 6, %7
	%9 = srem i32 11, %8
	store i32 %9, i32 %6, align 4
	%10 = load i32, i32* %1, align 4
	%11 = sub nsw i32 1, %10
	%12 = load i32, i32* %2, align 4
	%13 = load i32, i32* %6, align 4
	%14 = srem i32 %12, %13
	%15 = add nsw i32 %11, %14
	ret i32 %15
}

define i32 @function7(i32, i32) #0 {
	%3 = alloca i32, align 4
	store i32 %0, i32* %3, align 4
	%4 = alloca i32, align 4
	store i32 %1, i32* %4, align 4
	%5 = alloca i32, align 4
	%6 = load i32, i32* %3, align 4
	%7 = load i32, i32* %4, align 4
	%8 = srem i32 %6, %7
	store i32 %8, i32 %5, align 4
	%9 = load i32, i32* %3, align 4
	%10 = sub nsw i32 0,%9
	%11 = load i32, i32* %4, align 4
	%12 = load i32, i32* %5, align 4
	%13 = add nsw i32 %11, %12
	%14 = mul nsw i32 %10, %13
	store i32 %14, i32 %4, align 4
	%15 = load i32, i32* %4, align 4
	%16 = sdiv i32 %15, 2
	%17 = load i32, i32* %5, align 4
	%18 = sub nsw i32 %17, %16
	ret i32 %18
}

define void @kk(i32) #0 {
	%2 = alloca i32, align 4
	store i32 %0, i32* %2, align 4
	%3 = load i32, i32* %2, align 4
	%4 = sdiv i32 %3, 2
	store i32 %4, i32 @arroz, align 4
	ret void
}

define i32 @aster(i32, i32, i32) #0 {
	%4 = alloca i32, align 4
	store i32 %0, i32* %4, align 4
	%5 = alloca i32, align 4
	store i32 %1, i32* %5, align 4
	%6 = alloca i32, align 4
	store i32 %2, i32* %6, align 4
	%7 = alloca i32, align 4
	%8 = load i32, i32* %4, align 4
	%9 = mul nsw i32 3, %8
	%10 = load i32, i32* %5, align 4
	%11 = mul nsw i32 5, %10
	%12 = add nsw i32 %9, %11
	%13 = load i32, i32* %6, align 4
	%14 = mul nsw i32 2, %13
	%15 = add nsw i32 %12, %14
	%16 = sdiv i32 %15, 10
	store i32 %16, i32 %7, align 4
	%17 = load i32, i32* %4, align 4
	%18 = mul nsw i32 255, %17
	%19 = load i32, i32* %7, align 4
	%20 = sdiv i32 %18, %19
	store i32 %20, i32 %4, align 4
	%21 = load i32, i32* %5, align 4
	%22 = mul nsw i32 255, %21
	%23 = load i32, i32* %7, align 4
	%24 = sdiv i32 %22, %23
	store i32 %24, i32 %5, align 4
	%25 = load i32, i32* %6, align 4
	%26 = sub nsw i32 %25, 127
	%27 = mul nsw i32 255, %26
	%28 = load i32, i32* %7, align 4
	%29 = sdiv i32 %27, %28
	store i32 %29, i32 %6, align 4
	%30 = load i32, i32* %4, align 4
	%31 = load i32, i32* %5, align 4
	%32 = add nsw i32 %30, %31
	%33 = load i32, i32* %6, align 4
	%34 = add nsw i32 %32, %33
	ret i32 %34
}

define i32 @main() #0 {
	%1 = alloca i32, align 4
	store i32 0, i32 %1, align 4
	%2 = alloca i32, align 4
	store i32 %0, i32 %2, align 4
	store i32 %0, i32 @nom_nom, align 4
	%3 = load i32, i32* @arroz, align 4
	%4 = sub nsw i32 %0, %3
	store i32 %4, i32 %1, align 4
	ret i32 0
}
