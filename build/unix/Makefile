# Makefile

CFLAGS=-g -O2 -Wall -Wextra -std=c99 `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs`

ROOTDIR=../../
OUTDIR=$(ROOTDIR)bin/
EXECUTABLE=$(OUTDIR)chip8
SOURCEDIR=$(ROOTDIR)src/

SOURCES=$(wildcard $(SOURCEDIR)*.c)
SOURCES_NO_PATH=$(patsubst $(SOURCEDIR)%,%,$(SOURCES))
# Mapping: SOURCES => SOURCES_NO_PATH
# Example: ../../sources/main.c => main.c
HEADERS=$(wildcard $(SOURCEDIR)*.h)
OBJECTS=$(patsubst %.c,$(OUTDIR)%.o,$(SOURCES_NO_PATH))

$(OUTDIR)%.o: $(SOURCEDIR)%.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

clean:
	rm $(EXECUTABLE)
	rm $(OBJECTS)

.PHONY: all clean

# End Makefile