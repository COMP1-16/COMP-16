CC     = gcc
LEX    = flex
YACC   = bison
TARGET = lib/interpreter

CFLAGS  = -Wall -Wextra -g -I.

SRCS = parser.tab.c lex.yy.c \
       lib/ast.c lib/simbolos.c lib/semantico.c lib/interpreter.c lib/funcs.c src/main.c

all: build

build: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

parser.tab.c parser.tab.h: parser/parser.y
	$(YACC) -d parser/parser.y

lex.yy.c: lexer/lexer.l parser.tab.h
	$(LEX) lexer/lexer.l

run: build
	./$(TARGET)

test: build
	python3 tests.py

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h $(TARGET)
