# UNIX Makefile

CC=g++
LD=g++
CFLAGS=-g
LDFLAGS=

TestLogic : LogicGates.o LogicProcessor.o Main.o
	$(LD) -o TestLogic Main.o LogicProcessor.o LogicGates.o  

LogicGates.o: LogicGates.cpp LogicGates.h
	$(CC) $(CFLAGS) -c LogicGates.cpp

LogicProcessor.o: LogicProcessor.cpp LogicProcessor.h LogicGates.h 
	$(CC) $(CFLAGS) -c LogicProcessor.cpp

Main.o: Main.cpp LogicProcessor.h LogicGates.h TestCases.h
	$(CC) $(CFLAGS) -c Main.cpp

clean:
	rm *.o TestLogic
