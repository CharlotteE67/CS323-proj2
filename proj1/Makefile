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

bin:
	mkdir -p $(BIN)

.lex: lex.l
	$(FLEX) lex.l
.syntax: syntax.y
	$(BISON) -t -d -v syntax.y
.spl_type: .lex .syntax
	$(CPP) -c spl_type.cpp -o $(BIN)/spl_type.o
	@ar -rc $(BIN)/libspl_type.a $(BIN)/spl_type.o
.spl_node: .lex .syntax
	$(CPP) -c spl_node.cpp -o $(BIN)/spl_node.o
	@ar -rc $(BIN)/libspl_node.a $(BIN)/spl_node.o
.symbol_table: .lex .syntax
	$(CPP) -c symbol_table.cpp -o $(BIN)/symbol_table.o
	@ar -rc $(BIN)/libsymbol_table.a $(BIN)/symbol_table.o
.parser: .lex .syntax
	$(CPP) -c syntax.tab.c -o $(BIN)/parser.o -lfl -ly
	@ar -rc $(BIN)/libparser.a $(BIN)/parser.o
splc: bin .spl_type .spl_node .symbol_table .parser
	$(CPP) main.cpp -static -L$(BIN)/ -lspl_node -lspl_type -lsymbol_table -lparser -o $(BIN)/splc
clean:
	@rm -rf $(BIN)/
	@rm -rf $(MAKE_PATH)/ lex.yy.c syntax.tab.* syntax.output