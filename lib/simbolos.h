#ifndef SIMBOLOS_H
#define SIMBOLOS_H

#include "lib/types.h"

/* ------------------------------------------------------------------ *
 * Tabela de Símbolos — estrutura de dados pura                        *
 *                                                                     *
 * Responsabilidade: guardar e recuperar símbolos por nome.            *
 * Esta camada NÃO valida tipos, NÃO emite erros semânticos e         *
 * NÃO chama exit(). Essas decisões pertencem ao semantico.c.          *
 *                                                                     *
 * Implementação: hash aberto com encadeamento (separate chaining).    *
 * Tamanho primo para minimizar colisões por agrupamento.              *
 * ------------------------------------------------------------------ */

#define TAMANHO_HASH 997

typedef struct Celula {
    char          *nome;
    int            tipo;   /* código TIPO_* de types.h */
    Valor          valor;
    struct Celula *prox;
} Celula;

/* Cria e retorna uma tabela zerada. Retorna NULL se falhar. */
Celula **criarTabelaSimbolos(void);

/* Libera toda a memória da tabela. */
void liberarTabelaSimbolos(Celula **tabela);

/*
 * Insere um símbolo novo.
 * Retorna  0 em sucesso.
 * Retorna -1 se o nome já existir (redeclaração — decisão do chamador).
 */
int inserirSimbolo(char *nome, int tipo, Valor valor, Celula **tabela);

/*
 * Atualiza o valor de um símbolo existente.
 * Retorna  0 em sucesso.
 * Retorna -1 se o símbolo não existir.
 * Retorna -2 se o tipo for incompatível.
 */
int atualizarSimbolo(char *nome, Valor valor, Celula **tabela);

/*
 * Busca um símbolo pelo nome.
 * Retorna ponteiro para a Celula, ou NULL se não encontrar.
 * O chamador decide o que fazer com NULL.
 */
Celula *buscarSimbolo(char *nome, Celula **tabela);

/* Remove um símbolo. Retorna 0 em sucesso, -1 se não encontrar. */
int removerSimbolo(char *nome, Celula **tabela);

#endif /* SIMBOLOS_H */
