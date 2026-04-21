#ifndef SIMBOLOS_H
#define SIMBOLOS_H

#include "types.h"

#define TAMANHO_HASH 997 // Tamanho da tabela de símbolos : Escolha sempre números primos 
// para estimular uma boa distribuição, ou melhor, para não estimular uma distribuição ruim.

// typedef struct lista{
//     int valor;
//     struct lista *prox;
// }lista;

typedef struct celula{

    char *nome;
    int tipo;
    Valor valor;
    // int tamanho;
    // int dimensao;
    // int ldeclaracao;
    // lista luso;
    // int endereco;
    struct celula *prox;
    

}celula;



int hashFunction(char *nome);
celula** criarTabelaSimbolos();
void inserirSimbolo(char *nome,Valor value, celula **tabela);
void atualizarSimbolo(char *nome,Valor value, celula **tabela);
Valor buscaSimbolo(char *nome, celula **tabela);
void removerSimbolo(char *nome, celula **tabela);


#endif