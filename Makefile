CC = g++
CFLAGS = -std=c++11 -Wall -pedantic

ifeq ($(OS), Windows_NT)
	RM = del
	TARGET = main.exe
else
	RM = rm
	TARGET = main
endif

all:
	$(CC) $(CFLAGS) -o $(TARGET) main.cpp

clean:
	$(RM) $(TARGET)

run: all
	./$(TARGET)
