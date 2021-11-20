CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -g -lSDL2

.PHONY: all
all: clean game run

game: main.o
	$(CC) -o game $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $^ -o $@ $(CFLAGS)

.PHONY: run
run:
	./game

.PHONY: clean
clean:
	rm -f game; rm -f *.o; rm -f *.out
