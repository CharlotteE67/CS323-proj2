GPP=g++
FLEX=flex
BISON=bison
CXX_STD = -std=c++17

BIN=bin
CXX_DEBUG = -g -Wall
SRC_PATH=src
CXX_WARN = -Wall -Wextra
MAKE_PATH=./make-build-dir
CXX_FLAGS = -O3 $(CXX_DEBUG) $(CXX_STD) $(CXX_WARN)
CPP = $(GPP) $(CXX_FLAGS)

all: splc
bin:
	mkdir -p $(BIN)

.lex: lex.l
	$(FLEX) lex.l
.syntax: syntax.y
	$(BISON) -t -d -v syntax.y
.spl_node: .lex .syntax
	$(CPP) -c spl_node.cpp -o $(BIN)/spl_node.o
	@ar -rc $(BIN)/libspl_node.a $(BIN)/spl_node.o
.spl_type: .lex .syntax
	$(CPP) -c spl_type.cpp -o $(BIN)/spl_type.o
	@ar -rc $(BIN)/libspl_type.a $(BIN)/spl_type.o
.symbol_table: .lex .syntax
	$(CPP) -c symbol_table.cpp -o $(BIN)/symbol_table.o
	@ar -rc $(BIN)/libsymbol_table.a $(BIN)/symbol_table.o
splc: bin .spl_node .spl_type .symbol_table
	$(CPP) main.cpp -static -L$(BIN) -lsymbol_table -lspl_type -lspl_node -o $(BIN)/splc

clean:
	@rm -rf $(BIN)/
	@rm -rf $(MAKE_PATH)/ lex.yy.c syntax.tab.* syntax.output