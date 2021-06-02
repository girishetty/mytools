all: main.o
	gcc main.o -lstdc++ -o threading

main.o: main.cpp
	gcc -g -std=c++11 main.cpp

clean:
	rm main.o threading
