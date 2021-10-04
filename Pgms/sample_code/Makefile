all: sample.o
	gcc sample.o -lstdc++ -o sample

sample.o: sample.cpp
	gcc -g -std=c++11 sample.cpp

clean:
	rm sample.o sample
