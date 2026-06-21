#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/ast/ast.h"
#include "lib/simbols/simbolos.h"
#include "lib/analysis/semantico.h"
#include "lib/analysis/otimizador.h"
#include "lib/exec/interpreter.h"

extern int  yyparse(void);
extern void yyset_in(FILE *);
extern No  *raiz;

int main(int argc, char **argv) {
    int skip_opt = 0;
    int parse_only = 0;
    int semantic_only = 0;
    int optimize_only = 0;
    char *input_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--no-opt") == 0)
            skip_opt = 1;
        else if (strcmp(argv[i], "--parse-only") == 0)
            parse_only = 1;
        else if (strcmp(argv[i], "--semantic-only") == 0)
            semantic_only = 1;
        else if (strcmp(argv[i], "--optimize-only") == 0)
            optimize_only = 1;
        else
            input_file = argv[i];
    }

    /* --- entrada --- */
    if (input_file) {
        FILE *f = fopen(input_file, "r");
        if (!f) { perror(input_file); return 1; }
        yyset_in(f);
    }

    /* --- fase 1: análise sintática → constrói AST --- */
    if (yyparse() != 0) {
        fprintf(stderr, "Abortando: erro sintático.\n");
        return 1;
    }

    if (parse_only)
        return 0;

    /* --- fase 2: análise semântica --- */
    Celula **tabela = criarTabelaSimbolos();
    if (!tabela) {
        fprintf(stderr, "Erro fatal: nao foi possivel alocar a tabela de simbolos.\n");
        return 1;
    }

    int erros = checar(raiz, tabela);
    if (erros > 0) {
        fprintf(stderr, "%d erro(s) semantico(s). Abortando.\n", erros);
        liberarTabelaSimbolos(tabela);
        return 1;
    }

    if (semantic_only) {
        liberarTabelaSimbolos(tabela);
        return 0;
    }

    /* --- fase 3: otimização da AST --- */
    if (!skip_opt)
        raiz = otimizar(raiz);

    if (optimize_only) {
        liberarTabelaSimbolos(tabela);
        return 0;
    }

    /* --- fase 4: execução --- */
    liberarTabelaSimbolos(tabela);
    tabela = criarTabelaSimbolos();

    avaliar(raiz, tabela);

    liberarTabelaSimbolos(tabela);
    return 0;
}
