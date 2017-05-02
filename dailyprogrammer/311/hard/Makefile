.PHONY: all clean run lib

SRC_DIR = src
INCLUDE_DIRS =
OBJECT_DIR = obj
LIBRARY_DIRS = 
BIN_DIR = bin

OBJ_FILES = main.o dungeon.o
BIN_NAME = dungeon
LIBRARIES = 

OBJ = $(patsubst %, $(OBJECT_DIR)/%,$(OBJ_FILES))

BIN = $(BIN_DIR)/$(BIN_NAME)
RUN_ARGS = 20 20

CXX = clang++
CXXFLAGS = -std=c++11 -g $(INCLUDES)
LINKER = clang++
LINKFLAGS = $(LIBRARY_DIRS) $(LIBRARIES)

AR = ar
ARFLAGS = rvs

all: $(BIN)

$(OBJECT_DIR):
	mkdir $(OBJECT_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(SRC_DIR):
	mkdir $(SRC_DIR)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJECT_DIR)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(BIN): $(OBJ) | $(BIN_DIR)
	$(LINKER) -o $@ $^ $(LINKFLAGS)

$(BIN).a: $(OBJ) | $(BIN_DIR)
	$(AR) $(ARFLAGS) $(BIN).a $^

lib: $(BIN).a

run: $(BIN)
	$(BIN) $(RUN_ARGS)

clean:
	rm -rf $(OBJECT_DIR)
	rm -rf $(BIN_DIR)
