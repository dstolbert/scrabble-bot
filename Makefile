
# Compiler
CXX:= g++

# Executables
SERVER := scrabbleBot

# Include
LIB_INC := -I./include

LIBS := ${LIB_INC}

# Source
SOURCES := $(shell find source -type f -iname '*.cxx')

# Object files
OBJECTS := $(SOURCES:.cxx=.obj)

#Make rules
all: $(SERVER)

$(SERVER): $(OBJECTS) server.cxx
	$(CXX) $(LIBS) $^ -o $@

%.obj: %.cxx
	$(CXX) $(LIBS) -c $< -o $@

clean:
	rm -f $(OBJECTS)