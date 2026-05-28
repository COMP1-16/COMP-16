#include <stdlib.h>
#include <string.h>
#include "ast.h"

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
    n->u.bin.op = op;
    n->u.bin.esq = esq;
    n->u.bin.dir = dir;
    return n;
}

No *noDecl(int tipoDeclarado, char *nome) {
    No *n = alocar(NO_DECL);
    n->tipoDeclarado = tipoDeclarado;
    n->nome = strdup(nome);
    return n;
}

No *noDeclInit(int tipoDeclarado, char *nome, No *expr) {
    No *n = alocar(NO_DECL_INIT);
    n->tipoDeclarado = tipoDeclarado;
    n->nome = strdup(nome);
    n->u.bin.esq = expr;
    return n;
}

No *noAtrib(char *nome, No *expr) {
    No *n = alocar(NO_ATRIB);
    n->nome = strdup(nome);
    n->u.bin.esq = expr;
    return n;
}

No *noAtribOp(char op, char *nome, No *expr) {
    No *n = alocar(NO_ATRIB_OP);
    n->nome = strdup(nome);
    n->u.bin.op = op;
    n->u.bin.esq = expr;
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
    No *n = alocar(NO_BLOCO);
    n->u.bloco.stmts = malloc(count * sizeof(No *));
    memcpy(n->u.bloco.stmts, stmts, count * sizeof(No *));
    n->u.bloco.count = count;
    return n;
}

No *noReturn(No *expr) {
    No *n = alocar(NO_RETURN);
    n->u.bin.esq = expr;
    return n;
}

No *noRelacional(TipoRelacional op, No *esq, No *dir) {
    No *n = alocar(NO_RELACIONAL);
    n->relop = op;
    n->u.bin.esq = esq;
    n->u.bin.dir = dir;
    return n;
}

No *noIf(No *cond, No *thenBranch, No *elseBranch) {
    No *n = alocar(NO_IF);
    n->u.if_stmt.cond = cond;
    n->u.if_stmt.thenBranch = thenBranch;
    n->u.if_stmt.elseBranch = elseBranch;
    return n;
}

No *noWhile(No *cond, No *body) {
    No *n = alocar(NO_WHILE);
    n->u.while_stmt.cond = cond;
    n->u.while_stmt.body = body;
    return n;
}

No *noFor(No *init, No *cond, No *inc, No *body) {
    No *n = alocar(NO_FOR);
    n->u.for_stmt.init = init;
    n->u.for_stmt.cond = cond;
    n->u.for_stmt.inc = inc;
    n->u.for_stmt.body = body;
    return n;
}

No *noLogicalAnd(No *esq, No *dir) {
    No *n = alocar(NO_LOGICAL_AND);
    n->u.bin.esq = esq;
    n->u.bin.dir = dir;
    return n;
}

No *noLogicalOr(No *esq, No *dir) {
    No *n = alocar(NO_LOGICAL_OR);
    n->u.bin.esq = esq;
    n->u.bin.dir = dir;
    return n;
}

No *noNot(No *expr) {
    No *n = alocar(NO_NOT);
    n->u.not.expr = expr;
    return n;
}

No *noPrintf(No *args) {
    No *n = alocar(NO_PRINTF);
    n->u.call.args = args;
    return n;
}

No *noFuncDecl(int tipoRetorno, char *nome, No *params, No *body) {
    No *n = alocar(NO_FUNC_DECL);
    n->tipoDeclarado = tipoRetorno;
    n->nome = strdup(nome);
    n->u.func_decl.params = params;
    n->u.func_decl.body = body;
    return n;
}

No *noFuncCall(char *nome, No *args) {
    No *n = alocar(NO_FUNC_CALL);
    n->nome = strdup(nome);
    n->u.call.args = args;
    return n;
}