@nom_nom = global int32 88, align 4
@arroz = common global int32 0, align 4

define i32 @aqua_baldo() #0 {
	%1 = alloca i32, align 4
	store i32 160, i32 %1, align 4
	%2 = load i32, i32 %1, align 4
	%3 = alloca i32, align 4
	%4 = alloca i32, align 4
	ret i32 0
}

define i32 @function7(i32, i32) #0 {
	%3 = alloca i32, align 4
	store i32 %0, i32* %3, align 4
	%4 = load i32, i32* %3, align 4
	%5 = alloca i32, align 4
	store i32 %1, i32* %5, align 4
	%6 = load i32, i32* %5, align 4
	%7 = alloca i32, align 4
	ret i32 0
}

define void @kk(i32) #0 {
	%2 = alloca i32, align 4
	store i32 %0, i32* %2, align 4
	%3 = load i32, i32* %2, align 4
	ret void
}

define i32 @aster(i32, i32, i32) #0 {
	%4 = alloca i32, align 4
	store i32 %0, i32* %4, align 4
	%5 = load i32, i32* %4, align 4
	%6 = alloca i32, align 4
	store i32 %1, i32* %6, align 4
	%7 = load i32, i32* %6, align 4
	%8 = alloca i32, align 4
	store i32 %2, i32* %8, align 4
	%9 = load i32, i32* %8, align 4
	%10 = alloca i32, align 4
	ret i32 0
}

define i32 @main() #0 {
	%1 = alloca i32, align 4
	%2 = alloca i32, align 4
	ret i32 0
}
