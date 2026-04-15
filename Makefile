CC = gcc
LEX = flex
YACC = bison

TARGET = interpretador

LEXER = lexer/lexer.l
PARSER = parser/parser.y

LEX_OUT = lex.yy.c
PARSER_C = parser.tab.c
PARSER_H = parser.tab.h

all: build

build: # make -> Para compilar tudo
	$(YACC) -d $(PARSER)
	$(LEX) $(LEXER)
	$(CC) -I. $(PARSER_C) $(LEX_OUT) -o $(TARGET)

run: build # make run -> Para rodar manual 
	./$(TARGET)

test: build # make test -> Para rodas todos os testes
	python3 tests.py

clean:
	rm -f $(LEX_OUT) $(PARSER_C) $(PARSER_H) $(TARGET)