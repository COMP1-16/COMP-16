#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantico.h"

static int erros = 0;

/* ------------------------------------------------------------------ *
 * Helpers de relatório                                                *
 * ------------------------------------------------------------------ */
static void erroSem(const char *msg) {
    fprintf(stderr, "[Semantico] Erro: %s\n", msg);
    erros++;
}

static void avisoSem(const char *msg) {
    fprintf(stderr, "[Semantico] Aviso: %s\n", msg);
}

/* ------------------------------------------------------------------ *
 * Resolução de tipo de uma expressão                                  *
 *                                                                     *
 * Percorre o nó de expressão e retorna o tipo resultante (TIPO_*).   *
 * Não executa — apenas infere o tipo estaticamente.                  *
 * ------------------------------------------------------------------ */
static int resolverTipo(No *no, Celula **tabela) {
    if (!no) return TIPO_ERRO;

    switch (no->tipo) {
    case NO_INT:   return TIPO_INT;
    case NO_FLOAT: return TIPO_FLOAT;
    case NO_CHAR:  return TIPO_CHAR;
    case NO_STR:   return TIPO_STR;

    case NO_ID: {
        Celula *c = buscarSimbolo(no->nome, tabela);
        if (!c) {
            char msg[128];
            snprintf(msg, sizeof(msg), "variavel '%s' nao declarada", no->nome);
            erroSem(msg);
            return TIPO_ERRO;
        }
        return c->tipo;
    }

    case NO_BINOP: {
        int te = resolverTipo(no->esq, tabela);
        int td = resolverTipo(no->dir, tabela);
        if (te == TIPO_ERRO || td == TIPO_ERRO) return TIPO_ERRO;
        /* Regra de promoção: qualquer float promove o resultado */
        if (te == TIPO_FLOAT || td == TIPO_FLOAT) return TIPO_FLOAT;
        return TIPO_INT;
    }

    default:
        return TIPO_ERRO;
    }
}

/* ------------------------------------------------------------------ *
 * Verifica compatibilidade de tipos numa atribuição/declaração        *
 *                                                                     *
 * Retorna o tipo ajustado após coerção, ou TIPO_ERRO se incompatível. *
 * ------------------------------------------------------------------ */
static int verificarAtribuicao(int tipoAlvo, int tipoExpr, const char *nome) {
    if (tipoAlvo == tipoExpr) return tipoAlvo;

    if (tipoAlvo == TIPO_FLOAT && tipoExpr == TIPO_INT) return TIPO_FLOAT;

    if (tipoAlvo == TIPO_INT && tipoExpr == TIPO_FLOAT) {
        char msg[128];
        snprintf(msg, sizeof(msg),
                 "atribuicao de float para int em '%s' (perda de precisao)", nome);
        avisoSem(msg);
        return TIPO_INT;
    }

    /* Incompatível */
    char msg[128];
    snprintf(msg, sizeof(msg),
             "tipo incompativel na atribuicao de '%s'", nome);
    erroSem(msg);
    return TIPO_ERRO;
}

/* ------------------------------------------------------------------ *
 * Passagem principal — visita cada nó do bloco                       *
 * ------------------------------------------------------------------ */
static void checarNo(No *no, Celula **tabela) {
    if (!no) return;

    switch (no->tipo) {

    case NO_BLOCO:
        for (int i = 0; i < no->count; i++)
            checarNo(no->stmts[i], tabela);
        break;

    case NO_DECL: {
        /* int x; — sem inicializador */
        if (buscarSimbolo(no->nome, tabela)) {
            char msg[128];
            snprintf(msg, sizeof(msg), "redeclaracao de '%s'", no->nome);
            erroSem(msg);
            break;
        }
        Valor vazio = {0};
        vazio.tipo = no->tipoDeclarado;
        inserirSimbolo(no->nome, no->tipoDeclarado, vazio, tabela);
        break;
    }

    case NO_DECL_INIT: {
        /* int x = expr; */
        if (buscarSimbolo(no->nome, tabela)) {
            char msg[128];
            snprintf(msg, sizeof(msg), "redeclaracao de '%s'", no->nome);
            erroSem(msg);
            break;
        }
        int tipoExpr = resolverTipo(no->esq, tabela);
        if (tipoExpr != TIPO_ERRO)
            verificarAtribuicao(no->tipoDeclarado, tipoExpr, no->nome);

        Valor vazio = {0};
        vazio.tipo = no->tipoDeclarado;
        inserirSimbolo(no->nome, no->tipoDeclarado, vazio, tabela);
        break;
    }

    case NO_ATRIB: {
        /* x = expr; */
        Celula *c = buscarSimbolo(no->nome, tabela);
        if (!c) {
            char msg[128];
            snprintf(msg, sizeof(msg), "variavel '%s' nao declarada", no->nome);
            erroSem(msg);
            break;
        }
        int tipoExpr = resolverTipo(no->esq, tabela);
        if (tipoExpr != TIPO_ERRO)
            verificarAtribuicao(c->tipo, tipoExpr, no->nome);
        break;
    }

    case NO_ATRIB_OP:
    case NO_INC:
    case NO_DEC: {
        /* x += expr;  x++;  x--; */
        Celula *c = buscarSimbolo(no->nome, tabela);
        if (!c) {
            char msg[128];
            snprintf(msg, sizeof(msg), "variavel '%s' nao declarada", no->nome);
            erroSem(msg);
            break;
        }
        if (no->esq) {
            int tipoExpr = resolverTipo(no->esq, tabela);
            if (tipoExpr != TIPO_ERRO)
                verificarAtribuicao(c->tipo, tipoExpr, no->nome);
        }
        break;
    }

    case NO_RETURN:
        resolverTipo(no->esq, tabela);
        break;

    default:
        /* expressões soltas — apenas resolve o tipo para checar IDs */
        resolverTipo(no, tabela);
        break;
    }
}

int checar(No *raiz, Celula **tabela) {
    erros = 0;
    checarNo(raiz, tabela);
    return erros;
}
