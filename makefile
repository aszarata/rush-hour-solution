CC = g++
CFLAGS = -Wall -std=c++11

TARGET = program 

TEST_FILE = chb/test10

all: $(TARGET)


$(TARGET): main.o
	$(CC) $(CFLAGS) -o $@ $^


main.o: main.cpp
	$(CC) $(CFLAGS) -c $<


test: $(TARGET)
	./$(TARGET) $$(cat $(TEST_FILE))


clean:
	rm -f $(TARGET) *.o

