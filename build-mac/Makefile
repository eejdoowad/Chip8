MAIN=../bin/chip8

# CFLAGS=-g -O2 -Wall -Wextra -Iscr -I/usr/local/include/SDL -D_GNU_SOURCE=1 -D_THREAD_SAFE
# LDFLAGS=-L/usr/local/lib -lSDLmain -lSDL -Wl,-framework,Cocoa
CFLAGS=-g -O2 -Wall -Wextra -std=c99
LDFLAGS=-std=c99


SRC=$(wildcard ../src/*.c)
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
