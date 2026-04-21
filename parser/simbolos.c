#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolos.h"

/* Abordagem escolhida na implementação da Tabela de Símbolos: Hashing Estático Aberto
    Motivação: É simples de implementar e faz com que não tenhamos problemas com estouro cedo,
    porém tem o risco de se tornar lento com listas encadeadas extensas. 
    Para este projeto é válido.
*/

// Como nossa hash é identificada através de um ID -> Nome(String), precisamos de uma função hash apropriada
// Utilizaremos : Hashing Polinomial Simples
int hashFunction(char *nome){
    unsigned int hash = 0;


    // Percorre do Início ao fim da String Identificado por '\0' - é melhor do que usar strlen() neste caso.
    for(int i = 0; nome[i] != '\0'; i++) {
        // Multiplica o hash acumulado por 31 e soma o caractere atual
        hash = (hash * 31) + nome[i];
    }

    return hash % TAMANHO_HASH;
}



// Alocação da memória com calloc a fim de obter um vetor 'Limpo'
// Nosso Vetor, é um Vetor de Ponteiros, 
// cada posição contém um ponteiro apontando pra NULL ou para uma Lista Encadeada
celula** criarTabelaSimbolos(){
    celula **tabela = calloc(TAMANHO_HASH, sizeof(celula*));

    // Verifica se a tabela foi criada com Sucesso -> Memória Ram não acabou
    if (tabela == NULL) {
        return NULL; 
    }

    return tabela;
}



void inserirSimbolo(char *nome,Valor value, celula **tabela){
    int hashNumber = hashFunction(nome);

    celula *pointer = tabela[hashNumber];
    celula *pointerant = NULL;

    while(pointer != NULL && strcmp(pointer->nome,nome) != 0){
        pointerant = pointer;
        pointer = pointer->prox;
    }

    // Símbolo já existe
    if(pointer != NULL && strcmp(pointer->nome,nome) == 0){
        if (pointer->tipo != value.tipo) {
            printf("Erro Semantico: Tentativa de atribuir tipo incompativel a variavel '%s'\n", nome);
            exit(1);
        }
        pointer->valor = value;
        return;
    }

    celula *novoSimbolo = malloc(sizeof(celula));
    if(novoSimbolo == NULL){
        // Erro Ram
        return;
    }
    novoSimbolo->nome = strdup(nome);
    if(novoSimbolo->nome == NULL){ // Tratamento de exceção
        free(novoSimbolo);
        return;
    }
    novoSimbolo->tipo = value.tipo;
    novoSimbolo->valor = value;
    novoSimbolo->prox = NULL;

    // O símbolo ainda não existe
    if(pointer == NULL){
        pointer = pointerant;
        if(pointer == NULL){
            tabela[hashNumber] = novoSimbolo;
            return;
        }
    }

    pointer->prox = novoSimbolo;
    return;
}

void atualizarSimbolo(char *nome,Valor value, celula **tabela){
    int hashNumber = hashFunction(nome);

    celula *pointer = tabela[hashNumber];

    while(pointer != NULL && strcmp(pointer->nome,nome) != 0){
        pointer = pointer->prox;
    }

    if(pointer == NULL){
        printf("Erro Semantico: Variavel '%s' nao declarada\n", nome);
        exit(1);
    }
    
    // É Garantido que o pointer atual é o desejado.
    if (pointer->tipo != value.tipo) {
        printf("Erro Semantico: Tentativa de atribuir tipo incompativel a variavel '%s'\n", nome);
        exit(1);
    }
    pointer->valor = value;
    return;
    
}

Valor buscaSimbolo(char *nome, celula **tabela){
    int hashNumber = hashFunction(nome);

    celula *pointer = tabela[hashNumber];

    while(pointer != NULL && strcmp(pointer->nome,nome) != 0){
        pointer = pointer->prox;
    }

    if(pointer == NULL){
    printf("Erro Semantico: Variavel '%s' nao declarada\n", nome);
    exit(1);
    }

    return pointer->valor;
   
 
}

void removerSimbolo(char *nome, celula **tabela){
    int hashNumber = hashFunction(nome);

    celula *pointer = tabela[hashNumber];
    celula *pointerant = NULL;

    while(pointer != NULL && strcmp(pointer->nome,nome) != 0){
        pointerant = pointer;
        pointer = pointer->prox;
    }

    if(pointer == NULL){
    printf("Erro Semantico: Variavel '%s' nao declarada\n", nome);
    exit(1);
    }

    if(pointerant == NULL){
        tabela[hashNumber] = pointer->prox;
        free(pointer->nome);
        free(pointer);
        return;
    }

    pointerant->prox = pointer->prox;
    free(pointer->nome);
    free(pointer);

}

