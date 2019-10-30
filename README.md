# compiler
Project made for the [Introduction to Compilers course from UFPE](https://github.com/damorim/compilers-cin).

## Goal

Modify the compiler's visitor to generate a file in LLVM IR from another file written in C.

## Run

    $ make
    $ ./compile <filename>.c

## Compare

You can compare the generated file with the one created by clang by running

    clag -S -emit-llvm <filename>.c
