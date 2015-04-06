# Makefile

# Uses emscripten to generate html/js/rom data for chip8 in bin directory
# currently broken by EXPORTED_FUNCTIONS FLAG
# have to figure out why

# why does this work in powershell but not in the make file!
# emcc -o ../../bin/chip8.js ../../src/c8_util.c ../../src/main.c ../../src/c8_actions.c ../../src/instructions.c ../../src/io.c -s EXPORTED_FUNCTIONS="['_main', '_change_ROM_emscripten']" -s USE_SDL=2  --preload-file ../../roms/@/

CC=emcc

ROOTDIR=../../
OUTDIR=$(ROOTDIR)bin/

EXECUTABLE=$(OUTDIR)chip8.js
SOURCEDIR=$(ROOTDIR)src/
SOURCES=$(wildcard $(SOURCEDIR)*.c)
HEADERS=$(wildcard $(SOURCEDIR)*.h)
ROMSDIR=$(ROOTDIR)roms/

FLAGS=-O3 -s USE_SDL=2  --preload-file $(ROMSDIR)@/ -s EXPORTED_FUNCTIONS="['_main', '_set_ROM']"

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) -o $(EXECUTABLE) $(SOURCES) $(FLAGS)

clean:
	rm $(EXECUTABLE)*

.PHONY: all clean

# End Makefile