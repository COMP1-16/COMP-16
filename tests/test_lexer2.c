/*
 * test_lexer.c — testa o lexer sem precisar do parser (sem Bison)
 *
 * Compile:
 *   flex lexer.l
 *   gcc lex.yy.c test_lexer.c -lfl -o test_lexer
 *
 * Uso interativo:
 *   ./test_lexer
 *
 * Uso com arquivo:
 *   ./test_lexer < exemplo.c
 */

#include <stdio.h>
#include <stdlib.h>

/* Stub de YYSTYPE para substituir parser.tab.h */
typedef union {
    char *sIndex;   /* identificadores */
    char *sValue;   /* strings         */
    char  cValue;   /* caracteres      */
} YYSTYPE;

YYSTYPE yylval;

/* Declarações necessárias do lexer gerado pelo Flex */
int yylex(void);
int yywrap(void) { return 1; }

int main(int argc, char **argv) {
    if (argc > 1) {
        extern FILE *yyin;
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Erro ao abrir arquivo");
            return 1;
        }
    }

    /* Drena todos os tokens até EOF */
    while (yylex() != 0);

    return 0;
}