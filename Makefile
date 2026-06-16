CC     = gcc
LEX    = flex
YACC   = bison
TARGET = lib/interpreter

CFLAGS  = -Wall -Wextra -g -I.

SRCS = parser.tab.c lex.yy.c \
		lib/ast.c lib/simbolos.c lib/semantico.c lib/interpreter.c lib/funcs.c src/main.c

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

PARSER_TEST_TARGET = parser/test_parser
PARSER_TEST_SRCS = parser_test.tab.c lex.test.c

test_parser_build: $(PARSER_TEST_TARGET)

$(PARSER_TEST_TARGET): $(PARSER_TEST_SRCS)
	$(CC) $(CFLAGS) $^ -o $@

parser_test.tab.c parser_test.tab.h: parser/parser_test.y
	$(YACC) -d parser/parser_test.y

lex.test.c: lexer/lexer_test.l parser_test.tab.h
	$(LEX) -o $@ lexer/lexer_test.l

clean_parser_test:
	rm -f lex.test.c parser_test.tab.c parser_test.tab.h $(PARSER_TEST_TARGET)
