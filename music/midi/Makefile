.PHONY: all clean run lib

SRC_DIR = src
INCLUDE_DIRS =
OBJECT_DIR = obj
LIBRARY_DIRS = 
BIN_DIR = bin

OBJ_FILES = midi.o
BIN_NAME = midi
LIBRARIES = 

OBJ = $(patsubst %, $(OBJECT_DIR)/%,$(OBJ_FILES))

BIN = $(BIN_DIR)/$(BIN_NAME)
RUN_ARGS = 

CXX = gcc
CXXFLAGS = -std=c99 $(INCLUDES) -Wall
LINKFLAGS = $(LIBRARY_DIRS) $(LIBRARIES)

AR = ar
ARFLAGS = rvs

all: lib

$(OBJECT_DIR):
	mkdir $(OBJECT_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(SRC_DIR):
	mkdir $(SRC_DIR)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJECT_DIR)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(BIN): $(OBJ) | $(BIN_DIR)
	$(CXX) -o $@ $^ $(LINKFLAGS)

$(BIN).a: $(OBJ) | $(BIN_DIR)
	$(AR) $(ARFLAGS) $(BIN).a $^

lib: $(BIN).a

run: $(BIN)
	$(BIN) $(RUN_ARGS)

clean:
	rm -rf obj
	rm -rf bin
