
# Compiler
CXX:= g++

# Executables
SERVER := scrabbleBot

# Include
LIB_INC := -I./include

# Boost
LIB_BST := -lboost_signals

# Source
SOURCES := $(shell find source -type f -iname '*.cxx')

# Object files
OBJECTS := $(SOURCES:.cxx=.obj)

#Make rules
all: $(SERVER)

$(SERVER): $(OBJECTS) server.cxx

clean:
	rm -f $(OBJECTS)