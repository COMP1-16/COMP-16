#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* Aloca um nó zerado — campos não usados ficam NULL/0 por calloc. */
static No *alocar(TipoNo tipo) {
    No *no = calloc(1, sizeof(No));
    no->tipo = tipo;
    return no;
}

No *noInt(int val) {
    No *n = alocar(NO_INT);
    n->ival = val;
    return n;
}

No *noFloat(float val) {
    No *n = alocar(NO_FLOAT);
    n->fval = val;
    return n;
}

No *noChar(char val) {
    No *n = alocar(NO_CHAR);
    n->cval = val;
    return n;
}

No *noStr(char *val) {
    No *n = alocar(NO_STR);
    n->sval = strdup(val);
    return n;
}

No *noId(char *nome) {
    No *n = alocar(NO_ID);
    n->nome = strdup(nome);
    return n;
}

No *noBinop(char op, No *esq, No *dir) {
    No *n = alocar(NO_BINOP);
    n->op  = op;
    n->esq = esq;
    n->dir = dir;
    return n;
}

No *noDecl(int tipoDeclarado, char *nome) {
    No *n = alocar(NO_DECL);
    n->tipoDeclarado = tipoDeclarado;
    n->nome          = strdup(nome);
    return n;
}

No *noDeclInit(int tipoDeclarado, char *nome, No *expr) {
    No *n = alocar(NO_DECL_INIT);
    n->tipoDeclarado = tipoDeclarado;
    n->nome          = strdup(nome);
    n->esq           = expr;
    return n;
}

No *noAtrib(char *nome, No *expr) {
    No *n = alocar(NO_ATRIB);
    n->nome = strdup(nome);
    n->esq  = expr;
    return n;
}

No *noAtribOp(char op, char *nome, No *expr) {
    No *n = alocar(NO_ATRIB_OP);
    n->op   = op;
    n->nome = strdup(nome);
    n->esq  = expr;
    return n;
}

No *noInc(char *nome) {
    No *n = alocar(NO_INC);
    n->nome = strdup(nome);
    return n;
}

No *noDec(char *nome) {
    No *n = alocar(NO_DEC);
    n->nome = strdup(nome);
    return n;
}

No *noBloco(No **stmts, int count) {
    No *n    = alocar(NO_BLOCO);
    n->stmts = malloc(count * sizeof(No *));
    memcpy(n->stmts, stmts, count * sizeof(No *));
    n->count = count;
    return n;
}

No *noReturn(No *expr) {
    No *n = alocar(NO_RETURN);
    n->esq = expr;
    return n;
}
