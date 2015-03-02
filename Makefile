MAIN=chip8

# CFLAGS=-g -O2 -Wall -Wextra -Iscr -I/usr/local/include/SDL -D_GNU_SOURCE=1 -D_THREAD_SAFE
# LDFLAGS=-L/usr/local/lib -lSDLmain -lSDL -Wl,-framework,Cocoa
CFLAGS=-g -O2 -Wall -Wextra
LDFLAGS=


SRC=$(wildcard src/*.c)
OBJ=$(patsubst %.o,%.c,$(SRC))

%.o : %.c
	@echo CC $<
	@$(CC) -c $(CFLAGS) $<

all: chip8

chip8: $(OBJ)
	@echo cc -o $@
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	@echo cleaning
	@rm chip8

.PHONY: all options clean
