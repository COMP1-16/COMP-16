#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolos.h"

static unsigned int hash(const char *nome) {
    unsigned int h = 0;
    for (int i = 0; nome[i] != '\0'; i++)
        h = h * 31 + (unsigned char)nome[i];
    return h % TAMANHO_HASH;
}

Celula **criarTabelaSimbolos(void) {
    return calloc(TAMANHO_HASH, sizeof(Celula *));
}

void liberarTabelaSimbolos(Celula **tabela) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        Celula *c = tabela[i];
        while (c) {
            Celula *prox = c->prox;
            free(c->nome);
            free(c);
            c = prox;
        }
    }
    free(tabela);
}

int inserirSimbolo(char *nome, int tipo, Valor valor, Celula **tabela) {
    unsigned int idx = hash(nome);
    Celula *c = tabela[idx];

    /* verifica redeclaração */
    while (c) {
        if (strcmp(c->nome, nome) == 0) return -1; /* já existe */
        c = c->prox;
    }

    /* cria nova célula e insere na cabeça do bucket */
    Celula *nova = malloc(sizeof(Celula));
    if (!nova) return -1;

    nova->nome  = strdup(nome);
    nova->tipo  = tipo;
    nova->valor = valor;
    nova->prox  = tabela[idx];
    tabela[idx] = nova;
    return 0;
}

int atualizarSimbolo(char *nome, Valor valor, Celula **tabela) {
    unsigned int idx = hash(nome);
    Celula *c = tabela[idx];

    while (c) {
        if (strcmp(c->nome, nome) == 0) {
            if (c->tipo != valor.tipo) return -2; /* tipo incompatível */
            c->valor = valor;
            return 0;
        }
        c = c->prox;
    }
    return -1; /* não encontrado */
}

Celula *buscarSimbolo(char *nome, Celula **tabela) {
    unsigned int idx = hash(nome);
    Celula *c = tabela[idx];

    while (c) {
        if (strcmp(c->nome, nome) == 0) return c;
        c = c->prox;
    }
    return NULL;
}

int removerSimbolo(char *nome, Celula **tabela) {
    unsigned int idx = hash(nome);
    Celula *c   = tabela[idx];
    Celula *ant = NULL;

    while (c) {
        if (strcmp(c->nome, nome) == 0) {
            if (ant) ant->prox    = c->prox;
            else     tabela[idx]  = c->prox;
            free(c->nome);
            free(c);
            return 0;
        }
        ant = c;
        c   = c->prox;
    }
    return -1; /* não encontrado */
}
