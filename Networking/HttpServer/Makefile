# build an executable named httpserver for httpserver.c
CC=gcc
CFLAGS=-Wall -g
OBJFILES=main.o
TARGET=httpserver

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) -lm
clean:
	rm -f $(OBJFILES) $(TARGET)
