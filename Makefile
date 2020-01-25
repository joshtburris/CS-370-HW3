CXXFLAGS=-Wall -Wextra -Wextra -Wpedantic -Werror -Wfatal-errors
CC=gcc
FILE1=Coordinator.c
FILE2=Checker.c
OUT_EXE=Coordinator
OUT_EXE2=Checker
assignment=Joshua-Burris-HW3
build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILE1)
	$(CC) -o $(OUT_EXE2) $(FILE2)

tar:
	tar -cvv $(MAKEFILE_LIST) *.c >$(assignment).tar

clean:
	rm -f *.o $(OUT_EXE)
	rm -f *.o $(OUT_EXE2)
