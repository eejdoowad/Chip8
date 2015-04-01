# Makefile

# Uses emscripten to generate html/js/rom data for chip8 in bin directory

CC=emcc

ROOTDIR=../../
OUTDIR=$(ROOTDIR)bin/
# don't include .html in EXECUTABLE name
EXECUTABLE=$(OUTDIR)chip8
SOURCEDIR=$(ROOTDIR)src/
SOURCES=$(wildcard $(SOURCEDIR)*.c)
HEADERS=$(wildcard $(SOURCEDIR)*.h)
ROMSDIR=$(ROOTDIR)roms/

FLAGS=-O3 -s USE_SDL=2 --preload-file $(ROMSDIR)@/

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) -o $(EXECUTABLE).html $(SOURCES) $(FLAGS)

clean:
	rm $(EXECUTABLE)*

.PHONY: all clean

# End Makefile