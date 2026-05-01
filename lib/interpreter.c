#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"
#include "ast.h"
#include "simbolos.h"
#include "interpreter.h"

static Valor coercionar(Valor v, int tipoAlvo) {
    if (v.tipo == tipoAlvo) return v;

    Valor resultado;
    resultado.tipo = tipoAlvo;

    if (tipoAlvo == TIPO_FLOAT && v.tipo == TIPO_INT) {
        resultado.dado.f = (float)v.dado.i;
    } else if (tipoAlvo == TIPO_INT && v.tipo == TIPO_FLOAT) {
        resultado.dado.i = (int)v.dado.f;
    } else {
        resultado = v;
    }
    return resultado;
}

/* ------------------------------------------------------------------ *
 * Avaliação principal                                                 *
 * ------------------------------------------------------------------ */
Valor avaliar(No *no, Celula **tabela) {
    Valor resultado = {0};
    if (!no) return resultado;

    switch (no->tipo) {

    case NO_INT:
        resultado.tipo    = TIPO_INT;
        resultado.dado.i  = no->ival;
        return resultado;

    case NO_FLOAT:
        resultado.tipo    = TIPO_FLOAT;
        resultado.dado.f  = no->fval;
        return resultado;

    case NO_CHAR:
        resultado.tipo    = TIPO_CHAR;
        resultado.dado.c  = no->cval;
        return resultado;

    case NO_STR:
        resultado.tipo    = TIPO_STR;
        resultado.dado.s  = no->sval;
        return resultado;

    case NO_ID: {
        Celula *c = buscarSimbolo(no->nome, tabela);
        return c->valor;
    }

    case NO_BINOP: {
        Valor esq = avaliar(no->esq, tabela);
        Valor dir = avaliar(no->dir, tabela);

        if (no->op == '/') {
            int zeroint   = (dir.tipo == TIPO_INT  && dir.dado.i == 0);
            int zerofloat = (dir.tipo == TIPO_FLOAT && dir.dado.f == 0.0f);
            if (zeroint || zerofloat) {
                fprintf(stderr, "[Runtime] Erro: divisao por zero\n");
                exit(1);
            }
        }

        return fazerOperacao(esq, dir, no->op);
    }

    case NO_DECL: {
        Valor vazio = {0};
        vazio.tipo = no->tipoDeclarado;
        inserirSimbolo(no->nome, no->tipoDeclarado, vazio, tabela);
        return vazio;
    }

    case NO_DECL_INIT: {
        Valor val = avaliar(no->esq, tabela);
        val = coercionar(val, no->tipoDeclarado);
        inserirSimbolo(no->nome, no->tipoDeclarado, val, tabela);
        return val;
    }

    case NO_ATRIB: {
        Celula *c = buscarSimbolo(no->nome, tabela);
        Valor val = avaliar(no->esq, tabela);
        val = coercionar(val, c->tipo);
        c->valor = val;
        return val;
    }

    case NO_ATRIB_OP: {
        Celula *c   = buscarSimbolo(no->nome, tabela);
        Valor atual = c->valor;
        Valor expr  = avaliar(no->esq, tabela);
        Valor novo  = fazerOperacao(atual, expr, no->op);
        novo = coercionar(novo, c->tipo);
        c->valor = novo;
        return novo;
    }

    case NO_INC: {
        Celula *c = buscarSimbolo(no->nome, tabela);
        Valor um  = { .tipo = c->tipo };
        if (c->tipo == TIPO_FLOAT) um.dado.f = 1.0f;
        else                       um.dado.i = 1;
        c->valor = fazerOperacao(c->valor, um, '+');
        return c->valor;
    }

    case NO_DEC: {
        Celula *c = buscarSimbolo(no->nome, tabela);
        Valor um  = { .tipo = c->tipo };
        if (c->tipo == TIPO_FLOAT) um.dado.f = 1.0f;
        else                       um.dado.i = 1;
        c->valor = fazerOperacao(c->valor, um, '-');
        return c->valor;
    }

    case NO_BLOCO:
        for (int i = 0; i < no->count; i++)
            resultado = avaliar(no->stmts[i], tabela);
        return resultado;

    /* --- return --- */
    case NO_RETURN:
        return avaliar(no->esq, tabela);

    default:
        fprintf(stderr, "[Runtime] Erro: no desconhecido tipo=%d\n", no->tipo);
        exit(1);
    }
}
