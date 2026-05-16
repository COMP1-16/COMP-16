#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"
#include "interpreter.h"

// Verifica se um tipo é passível de Casting Matemático
static int compatibilidade_casting(int tipo){ 
    return(tipo == TIPO_INT || tipo == TIPO_FLOAT || tipo == TIPO_DOUBLE || 
            tipo == TIPO_CHAR || tipo == TIPO_BOOL);
}


static Valor coercionar(Valor v, int tipoAlvo) {
    if (v.tipo == tipoAlvo) return v; 

    Valor resultado = v;
    resultado.tipo = tipoAlvo;
    if (compatibilidade_casting(v.tipo) && compatibilidade_casting(tipoAlvo)) {
        
        switch (tipoAlvo) {
            case TIPO_INT:
                if (v.tipo == TIPO_FLOAT)       resultado.dado.i = (int)v.dado.f;
                else if (v.tipo == TIPO_DOUBLE) resultado.dado.i = (int)v.dado.d;
                else if (v.tipo == TIPO_CHAR)   resultado.dado.i = (int)v.dado.c;
                else if (v.tipo == TIPO_BOOL)   resultado.dado.i = (int)v.dado.i;
                break;

            case TIPO_FLOAT:
                if (v.tipo == TIPO_INT)         resultado.dado.f = (float)v.dado.i;
                else if (v.tipo == TIPO_DOUBLE) resultado.dado.f = (float)v.dado.d;
                else if (v.tipo == TIPO_CHAR)   resultado.dado.f = (float)v.dado.c;
                else if (v.tipo == TIPO_BOOL)   resultado.dado.f = (float)v.dado.i;
                break;

            case TIPO_DOUBLE:
                if (v.tipo == TIPO_INT)         resultado.dado.d = (double)v.dado.i;
                else if (v.tipo == TIPO_FLOAT)  resultado.dado.d = (double)v.dado.f;
                else if (v.tipo == TIPO_CHAR)   resultado.dado.d = (double)v.dado.c;
                else if (v.tipo == TIPO_BOOL)   resultado.dado.d = (double)v.dado.i;
                break;

            case TIPO_CHAR:
                if (v.tipo == TIPO_INT)         resultado.dado.c = (char)v.dado.i;
                else if (v.tipo == TIPO_FLOAT)  resultado.dado.c = (char)v.dado.f;
                else if (v.tipo == TIPO_DOUBLE) resultado.dado.c = (char)v.dado.d;
                else if (v.tipo == TIPO_BOOL)   resultado.dado.c = (char)v.dado.i;
                break;

            case TIPO_BOOL:
                if (v.tipo == TIPO_INT)         resultado.dado.i = (v.dado.i != 0) ? 1 : 0;
                else if (v.tipo == TIPO_FLOAT)  resultado.dado.i = (v.dado.f != 0.0f) ? 1 : 0;
                else if (v.tipo == TIPO_DOUBLE) resultado.dado.i = (v.dado.d != 0.0) ? 1 : 0;
                else if (v.tipo == TIPO_CHAR)   resultado.dado.i = (v.dado.c != 0) ? 1 : 0;
                break;
        }
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

        case NO_RELACIONAL: {
        Valor esq = avaliar(no->esq, tabela);
        Valor dir = avaliar(no->dir, tabela);
        Valor resultado;
        resultado.tipo = TIPO_BOOL;
        float v1 = (esq.tipo == TIPO_FLOAT) ? esq.dado.f : esq.dado.i;
        float v2 = (dir.tipo == TIPO_FLOAT) ? dir.dado.f : dir.dado.i;

        switch (no->relop) {
            case OP_EQ: resultado.dado.i = (v1 == v2); break;
            case OP_NE: resultado.dado.i = (v1 != v2); break;
            case OP_LT: resultado.dado.i = (v1 <  v2); break;
            case OP_GT: resultado.dado.i = (v1 >  v2); break;
            case OP_LE: resultado.dado.i = (v1 <= v2); break;
            case OP_GE: resultado.dado.i = (v1 >= v2); break;

            default:
                fprintf(stderr, "[Runtime] Operador relacional invalido\n");
                exit(1);
        }
        return resultado;
    }

    case NO_IF: {
        Valor cond = avaliar(no->condicao, tabela);
        int verdadeiro = 0;

        if (cond.tipo == TIPO_BOOL || cond.tipo == TIPO_INT)
            verdadeiro = cond.dado.i;
        if (verdadeiro)
            return avaliar(no->thenBranch, tabela);
        if (no->elseBranch)
            return avaliar(no->elseBranch, tabela);
        return resultado;
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
