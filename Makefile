CC=g++
CFLAGS= -o
OBJS=rcmk.o
TARGET=rcmk

all: $(TARGET)

clean:
	rm -rf *.o
	rm -rf $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $@ $(OBJS)

rcmk.o: rcmk.cpp
	$(CC) -c rcmk.cpp
