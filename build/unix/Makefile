# Makefile

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

# Gets file paths of headers, sources, and generated objects
SOURCES:=$(wildcard $(SOURCEDIR)*.c)
HEADERS=$(wildcard $(SOURCEDIR)*.h)
OBJECTS=$(patsubst $(SOURCEDIR)%.c,$(OBJECTDIR)%.o,$(SOURCES))

all: $(OBJECTDIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJECTDIR):
	mkdir -p $(OBJECTDIR)

$(OBJECTDIR)%.o: $(SOURCEDIR)%.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	-rm $(EXECUTABLE)
	-rm -rf $(OBJECTDIR)
# The starting - suppresses output value from rm (which fails if files don't exist)

.PHONY: all clean

# End Makefile