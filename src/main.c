#include <stdio.h>
#include <stdlib.h>
#include "lib/ast.h"
#include "lib/simbolos.h"
#include "lib/semantico.h"
#include "lib/interpreter.h"

extern int  yyparse(void);
extern void yyset_in(FILE *);
extern No  *raiz;

int main(int argc, char **argv) {
    /* --- entrada --- */
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) { perror(argv[1]); return 1; }
        yyset_in(f);
    }

    /* --- fase 1: análise sintática → constrói AST --- */
    if (yyparse() != 0) {
        fprintf(stderr, "Abortando: erro sintático.\n");
        return 1;
    }

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

    /* --- fase 3: execução --- */
    liberarTabelaSimbolos(tabela);
    tabela = criarTabelaSimbolos();

    avaliar(raiz, tabela);

    liberarTabelaSimbolos(tabela);
    return 0;
}
