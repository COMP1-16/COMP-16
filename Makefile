CC     = gcc
LEX    = flex
YACC   = bison
TARGET = lib/interpreter

CFLAGS  = -Wall -Wextra -g -I.

SRCS = parser.tab.c lex.yy.c \
    lib/ast.c lib/simbolos.c lib/semantico.c lib/otimizador.c lib/interpreter.c lib/funcs.c lib/math.c lib/stdlib.c lib/ctype.c src/main.c

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

define RUN_TESTS
	@if command -v python3 >/dev/null 2>&1; then \
		python3 tests.py $(1); \
	elif command -v python >/dev/null 2>&1; then \
		python tests.py $(1); \
	else \
		for py in /c/Users/*/AppData/Local/Programs/Python/Python*/python.exe; do \
			if [ -x "$$py" ]; then "$$py" tests.py $(1); exit $$?; fi; \
		done; \
		echo "Python nao encontrado."; \
		echo "Opcoes: pacman -S python  OU  export PATH=/c/Users/SEU_USUARIO/AppData/Local/Programs/Python/Python313:$$PATH"; \
		exit 1; \
	fi
endef

test: build
	$(call RUN_TESTS,)

test-math: build
	$(call RUN_TESTS,testes/math/sintatico testes/math/semantico)

test-stdlib: build
	$(call RUN_TESTS,testes/stdlib/sintatico testes/stdlib/semantico)

test-ctype: build
	$(call RUN_TESTS,testes/ctype/sintatico testes/ctype/semantico)

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h $(TARGET)
