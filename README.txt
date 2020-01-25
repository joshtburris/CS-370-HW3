README
======

This package includes the following files:

|-- Checker.c [This is the file that will perform the calculation for the Coordinator]
|-- Checker.h [The Header file declaring the function in Checker]
|-- Coordinator.c [This will create a new process to run the calculation and print its steps and output]
|-- Makefile [This compiles your Checker.c and Coordinator.c programs to a runnable Checker.out and Coordinator.out executables]
|-- README.txt [This file]

To compile:
    gcc *.c
    make

To run:
    ./Coordinator <Divisor> <Dividend 1> <Dividend 2> <Dividend 3> <Dividend 4>

For example:
    ./Coordinator 7 32 49 846 22344

