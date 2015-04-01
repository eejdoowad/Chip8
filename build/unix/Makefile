# Makefile

# Compiler
CC=
# Name of Output Executable
OUTPUT=chip8
# Compiler/Linker Flags
CFLAGS=-g -O2 -Wall -Wextra -std=c99 `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs`

# Modify these if you change the project's directory structure
ROOTDIR=../../
OUTDIR=$(ROOTDIR)bin/
OBJECTDIR=$(OUTDIR)objects/
SOURCEDIR=$(ROOTDIR)src/
EXECUTABLE=$(OUTDIR)$(OUTPUT)

# SE = Source Extension (set to .c or .cpp)
# HE = Header Extension (set to .h or .hpp)
SE=.c
HE=.h

# Gets file paths of headers, sources, and generated objects
SOURCES=$(wildcard $(SOURCEDIR)*SE)
HEADERS=$(wildcard $(SOURCEDIR)*HE)
OBJECTS=$(patsubst $(SOURCEDIR)%SE,$(OBJECTDIR)%.o,$(SOURCES))

all: $(OBJECTDIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJECTDIR):
	mkdir -p $(OBJECTDIR)

$(OBJECTDIR)%.o: $(SOURCEDIR)%SE $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	-rm $(EXECUTABLE)
	-rm -rf $(OBJECTDIR)
# The starting - suppresses output value from rm (which fails if files don't exist)

.PHONY: all clean

# End Makefile