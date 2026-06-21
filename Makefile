# MSYS2: no shell MSYS puro, gcc fica no prefixo da toolchain (ucrt64/mingw64)
CC     ?= gcc

MSYS2_PREFIX :=
ifneq ($(wildcard /ucrt64/bin/gcc),)
  MSYS2_PREFIX := /ucrt64
else ifneq ($(wildcard /mingw64/bin/gcc),)
  MSYS2_PREFIX := /mingw64
endif
ifneq ($(MSYS2_PREFIX),)
  export PATH := $(MSYS2_PREFIX)/bin:$(PATH)
  ifeq ($(shell command -v $(CC) 2>/dev/null),)
    CC := $(MSYS2_PREFIX)/bin/gcc
  endif
endif

LEX    = flex
YACC   = bison
ifeq ($(OS),Windows_NT)
TARGET = lib/interpreter.exe
else
TARGET = lib/interpreter
endif

CFLAGS  = -Wall -Wextra -g -I. \
	-Ilib/ast -Ilib/types -Ilib/simbols \
	-Ilib/analysis -Ilib/exec -Ilib/libs

SRCS = parser.tab.c lex.yy.c \
	lib/ast/ast.c \
	lib/simbols/simbolos.c \
	lib/analysis/semantico.c \
	lib/analysis/otimizador.c \
	lib/exec/interpreter.c \
	lib/exec/funcs.c \
	lib/libs/math.c \
	lib/libs/stdlib.c \
	src/main.c

FEATURES = atualizacao_variaveis declaracao_variaveis for if_else while \
           operadores_aritmeticos operadores_logicos math stdlib \
           switch_case recursao otimizador

define FEATURE_LAYERS
testes/$(1)/sintatico/validos testes/$(1)/sintatico/invalidos \
testes/$(1)/semantico/validos testes/$(1)/semantico/invalidos \
testes/$(1)/execucao/validos testes/$(1)/execucao/invalidos
endef

ALL_TEST_DIRS := $(foreach f,$(FEATURES),$(call FEATURE_LAYERS,$(f)))
EXEC_TEST_DIRS := $(foreach f,$(FEATURES),testes/$(f)/execucao/validos testes/$(f)/execucao/invalidos)

build: $(TARGET)

check-env:
	@echo "CC=$(CC)"
	@echo "PATH=$(PATH)"
	@$(CC) --version
	@$(CC) -dumpmachine

$(TARGET): $(SRCS)
	@echo "Compilando $(TARGET) com $(CC)..."
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
	$(call RUN_TESTS,$(ALL_TEST_DIRS))

test-execucao: build
	$(call RUN_TESTS,$(EXEC_TEST_DIRS))

test-switch: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,switch_case))

test-math: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,math))

test-stdlib: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,stdlib))

test-otimizador: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,otimizador))

test-for: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,for))

test-while: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,while))

test-if-else: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,if_else))

test-decl: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,declaracao_variaveis))

test-atrib: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,atualizacao_variaveis))

test-op-arit: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,operadores_aritmeticos))

test-op-log: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,operadores_logicos))

test-recursao: build
	$(call RUN_TESTS,$(call FEATURE_LAYERS,recursao))

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h lib/interpreter lib/interpreter.exe $(TARGET)

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
