%{
#include <stdio.h>
#include <stdlib.h>

#include "parser/funcs.h"
#include "parser/simbolos.h"

extern int yylineno;

int yylex(void);
void yyerror(const char *s);

celula **TabelaDeSimbolos;
int tipo_atual_declaracao;

%}



%code requires {
#include "parser/types.h"
}

%union {
    int iValue;
    float fValue;
    char cValue;
    char *sStr;
    Valor valor;
}

%token <iValue> NUM 
%token <sStr> STRING
%token <cValue> CHAR_LIT
%token <fValue> FLOAT_NUM

%token <sStr> ID

%type <valor> expressao

    //Comparison Operators
%token EQUAL DIFF LESS_EQ GREAT_EQ LESSER GREATER

    //Update Operators
%token UPDT_PLUS UPDT_MINUS UPDT_TIMES UPDT_DIVIDE
%token UPDT_INC UPDT_DEC

%token INITVAR

    //Math Operators
%token PLUS MINUS TIMES DIVIDE INTDIVIDE MOD

    //Brackets
%token L_PAREN R_PAREN L_SQBRACKET R_SQBRACKET L_CRLRBRACKET R_CRLRBRACKET

    //Logical Operators
%token AND OR NOT

    //Types: Definição dos Tokens(Símbolos Terminais) dos tipos.
%token TYPE_INT TYPE_FLOAT TYPE_DOUBLE TYPE_CHAR TYPE_BOOL TYPE_VOID

%token SEMICOLON
%token COMMA

%token KW_RETURN

%token ADDR


%left PLUS MINUS
%left TIMES DIVIDE INTDIVIDE MOD
%left L_PAREN R_PAREN

%%

code:
    codeblock
    | code codeblock
    | KW_RETURN expressao SEMICOLON {printf("Added a return statement.\n");}
    | code KW_RETURN expressao SEMICOLON {printf("Added a return statement.\n");}
    ;

codeblock:
    declaracao {printf("Started as statement.\n");}
    | expressao SEMICOLON {printf("Started as expression.\n");}
    | atualizacao
    ;

//tipo: .... é um agrupamento lógico.
tipo:
    TYPE_INT { tipo_atual_declaracao = TIPO_INT; }
    | TYPE_FLOAT { tipo_atual_declaracao = TIPO_FLOAT; }
    | TYPE_DOUBLE { tipo_atual_declaracao = TIPO_DOUBLE; }
    | TYPE_CHAR { tipo_atual_declaracao = TIPO_CHAR; }
    | TYPE_BOOL { tipo_atual_declaracao = TIPO_BOOL; }
    | TYPE_VOID { tipo_atual_declaracao = TIPO_VOID; }
    ;

declarador:
    ID {
        /* Variavel criada sem valor inicial (ex: float x;) */
        Valor vazio;
        vazio.tipo = tipo_atual_declaracao;
        /* Zera o dado dependendo do tipo para nao ter lixo de memoria */
        if (vazio.tipo == TIPO_FLOAT) vazio.dado.f = 0.0;
        else vazio.dado.i = 0;

        inserirSimbolo($1, vazio, TabelaDeSimbolos);
        printf("[Memoria] Variavel '%s' declarada sem valor.\n", $1);
    }
    | ID INITVAR expressao {
        /* Variavel criada com valor (ex: float x = 10;) */
        
        /* Sistema de Coercao (Casting Implicito) */
        if (tipo_atual_declaracao == TIPO_FLOAT && $3.tipo == TIPO_INT) {
            /* Converte o Int para Float silenciosamente */
            $3.dado.f = (float)$3.dado.i;
            $3.tipo = TIPO_FLOAT;
        } 
        else if (tipo_atual_declaracao == TIPO_INT && $3.tipo == TIPO_FLOAT) {
            /* Aviso de perda de precisao (Opcional, mas muito profissional) */
            printf("Aviso: Convertendo float para int na variavel '%s' (perda de precisao).\n", $1);
            $3.dado.i = (int)$3.dado.f;
            $3.tipo = TIPO_INT;
        }
        else if (tipo_atual_declaracao != $3.tipo) {
            /* Erro fatal (Ex: Tentar jogar String num Int) */
            printf("Erro Semantico: O valor atribuido a '%s' e totalmente incompativel.\n", $1);
            exit(1);
        }
        
        inserirSimbolo($1, $3, TabelaDeSimbolos);
        printf("[Memoria] Variavel '%s' inicializada com sucesso.\n", $1);
    }
    ;

declaradores:
    declarador
    | declaradores COMMA declarador
    ;

declaracao:
    tipo declaradores SEMICOLON
    ;

atualizacao:
    ID UPDT_PLUS expressao SEMICOLON {
        Valor atual = buscaSimbolo($1, TabelaDeSimbolos);
        Valor novo = fazer_operacao(atual, $3, '+');
        atualizarSimbolo($1, novo, TabelaDeSimbolos);
        printf("[Memoria] Variavel '%s' atualizada (+).\n", $1);
    }
    | ID UPDT_MINUS expressao SEMICOLON {
        Valor atual = buscaSimbolo($1, TabelaDeSimbolos);
        Valor novo = fazer_operacao(atual, $3, '-');
        atualizarSimbolo($1, novo, TabelaDeSimbolos);
        printf("[Memoria] Variavel '%s' atualizada (-).\n", $1);
    }
    | ID UPDT_TIMES expressao SEMICOLON{
        Valor atual = buscaSimbolo($1, TabelaDeSimbolos);
        Valor novo = fazer_operacao(atual, $3, '*');
        atualizarSimbolo($1, novo, TabelaDeSimbolos);
        printf("[Memoria] Variavel '%s' atualizada (*).\n", $1);
    }
    | ID UPDT_DIVIDE expressao SEMICOLON{
        Valor atual = buscaSimbolo($1, TabelaDeSimbolos);
        Valor novo = fazer_operacao(atual, $3, '/');
        atualizarSimbolo($1, novo, TabelaDeSimbolos);
        printf("[Memoria] Variavel '%s' atualizada (/).\n", $1);
    }
    | ID UPDT_INC SEMICOLON{
        Valor atual = buscaSimbolo($1, TabelaDeSimbolos);
        Valor um;
        um.tipo = TIPO_INT;
        um.dado.i = 1;
        Valor novo = fazer_operacao(atual, um, '+');
        atualizarSimbolo($1, novo, TabelaDeSimbolos);
        printf("[Memoria] Variavel '%s' atualizada (++).\n", $1);
    }
    | ID UPDT_DEC SEMICOLON{
        Valor atual = buscaSimbolo($1, TabelaDeSimbolos);
        Valor um;
        um.tipo = TIPO_INT;
        um.dado.i = 1;
        Valor novo = fazer_operacao(atual, um, '-');
        atualizarSimbolo($1, novo, TabelaDeSimbolos);
        printf("[Memoria] Variavel '%s' atualizada (--).\n", $1);
    }
    ;

// STRING ou CHAR_LIT "pode ser somado" com INT(?)
expressao:
  expressao PLUS expressao {
        $$ = fazer_operacao($1, $3, '+');
    }
  | expressao MINUS expressao {
        $$ = fazer_operacao($1, $3, '-');
    }
  | expressao TIMES expressao {
        $$ = fazer_operacao($1, $3, '*');
    }
  | expressao DIVIDE expressao {
        $$ = fazer_operacao($1, $3, '/');
    }
  | L_PAREN expressao R_PAREN {
        $$ = $2;
        printf("Expressão: %f\n", $$.dado.f);
    }
  | NUM {
        $$.tipo = TIPO_INT;
        $$.dado.i = $1;
        printf("Inteiro: %d\n", $$.dado.i);
    }
  | FLOAT_NUM {
        $$.tipo = TIPO_FLOAT;
        $$.dado.f = $1;
        printf("Float: %f\n", $$.dado.f);
    }
  | STRING {
        $$.tipo = TIPO_STR;
        $$.dado.s = $1;
        printf("String: %s\n", $$.dado.s);
    }
  | CHAR_LIT {
        $$.tipo = TIPO_CHAR;
        $$.dado.c = $1;
        printf("Caractere: %c\n", $$.dado.c);
    }
  | ID {
        $$ = buscaSimbolo($1, TabelaDeSimbolos);
        
        /* Debug visual para ajudar a equipe */
        if ($$.tipo == TIPO_INT) printf("Lendo '%s': %d\n", $1, $$.dado.i);
        else if ($$.tipo == TIPO_FLOAT) printf("Lendo '%s': %f\n", $1, $$.dado.f);
    }

%%

void yyerror(const char *s) {
    fprintf(stderr, "Linha %d, Erro sintático: %s\n", yylineno, s);
}

int main(void) {
    /* Cria a Hash de tamanho 997 e preenche com NULL */
    TabelaDeSimbolos = criarTabelaSimbolos();
    
    if (TabelaDeSimbolos == NULL) {
        fprintf(stderr, "Erro fatal: Nao foi possivel alocar a Tabela de Simbolos.\n");
        return 1;
    }
    yyparse();
    return 0;
}
