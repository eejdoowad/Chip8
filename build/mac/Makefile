MAIN=../../bin/chip8

CFLAGS=-g -O2 -Wall -Wextra -std=c99 `sdl2-config --cflags`
LDFLAGS=-std=c99 `sdl2-config --libs`

SRC=$(wildcard ../../src/*.c)
OBJ=$(patsubst %.o,%.c,$(SRC))

%.o : %.c
	@echo CC $<
	@$(CC) -c $(CFLAGS) $<

all: $(MAIN)

$(MAIN): $(OBJ)
	@echo cc -o $@
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	@echo cleaning
	@rm $(MAIN)

.PHONY: all options clean
