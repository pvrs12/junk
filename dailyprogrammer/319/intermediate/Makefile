SRC=worm_wars.c
DST=worm_wars

CLANG_DIR=clang
GCC_DIR=gcc

.PHONY: all
all: clangm gccm

.PHONY: clangm
clangm: $(CLANG_DIR)/$(DST)-clang $(CLANG_DIR)/$(DST)-clang.s

$(CLANG_DIR)/$(DST)-clang: $(CLANG_DIR) $(SRC)
	clang -o $(CLANG_DIR)/$(DST)-clang $(SRC)

$(CLANG_DIR)/$(DST)-clang.s: $(CLANG_DIR) $(SRC)
	clang -o $(CLANG_DIR)/$(DST)-clang.s -S $(SRC)

$(CLANG_DIR):
	mkdir $(CLANG_DIR)

.PHONY: gccm
gccm: $(GCC_DIR)/$(DST)-gcc $(GCC_DIR)/$(DST)-gcc.s

$(GCC_DIR)/$(DST)-gcc: $(GCC_DIR) $(SRC)
	gcc -std=c99 -o $(GCC_DIR)/$(DST)-gcc $(SRC)

$(GCC_DIR)/$(DST)-gcc.s: $(GCC_DIR) $(SRC)
	gcc -std=c99 -o $(GCC_DIR)/$(DST)-gcc.s -S $(SRC)

$(GCC_DIR):
	mkdir $(GCC_DIR)

.PHONY: clean
clean:
	rm -rf $(GCC_DIR) $(CLANG_DIR)
