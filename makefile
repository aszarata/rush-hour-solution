CC = g++
CFLAGS = -Wall -std=c++11

TARGET = program 

TEST_DIR = ./test/chc

TEST_FILE = $(TEST_DIR)/test5

all: $(TARGET)


$(TARGET): main.o
	$(CC) $(CFLAGS) -o $@ $^


main.o: main.cpp
	$(CC) $(CFLAGS) -c $<


test: $(TARGET)
	./$(TARGET) $$(cat $(TEST_FILE))


clean:
	rm -f $(TARGET) *.o

