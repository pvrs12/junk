.PHONY: all
.PHONY: clean
.PHONY: run
.PHONY: run_convert

SRC_DIR = src
INCLUDE_DIRS =
OBJECT_DIR = obj
LIBRARY_DIRS = -Llib
BIN_DIR = bin

BIN_NAME = ifs
SRC_FILES = ifs.cpp
LIBRARIES = 

SRC = $(patsubst %,$(SRC_DIR)/%,$(SRC_FILES))

OBJ_FILES = $(patsubst %.cpp,%.o,$(SRC_FILES))
OBJ = $(patsubst %,$(OBJECT_DIR)/%,$(OBJ_FILES))

BIN = $(BIN_DIR)/$(BIN_NAME)
RUN_ARGS = 2500000 < inputs/fern > fern.pbm

CXX = g++
CXXFLAGS = -std=c++11 $(INCLUDES)
LINKFLAGS = $(LIBRARY_DIRS) $(LIBRARIES)

all: $(BIN)

$(OBJECT_DIR):
	mkdir obj

$(BIN_DIR):
	mkdir bin

$(OBJ): $(SRC) $(OBJECT_DIR)
	$(CXX) -o $(OBJ) -c $(SRC) $(CXXFLAGS)

$(BIN): $(OBJ) $(BIN_DIR)
	$(CXX) -o $@ $(OBJ) $(LINKFLAGS)

fern.pbm: $(BIN)
	$(BIN) $(RUN_ARGS)

run: fern.pbm

fern.png: fern.pbm
	convert fern.pbm fern.png

run_convert: fern.png

clean:
	rm -rf obj
	rm -rf bin
