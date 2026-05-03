%{
#include <stdio.h>
#include <stdlib.h>

#include "parser/funcs.h"

extern int yylineno;

int yylex(void);
void yyerror(const char *s);

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
    | code KW_RETURN expressao SEMICOLON {printf("Added a return statement.\n");}
    ;

codeblock:
    declaracao {printf("Started as statement.\n");}
    | expressao SEMICOLON {printf("Started as expression.\n");}
    | atualizacao
    ;

//tipo: .... é um agrupamento lógico.
tipo:
    TYPE_INT
    | TYPE_FLOAT
    | TYPE_DOUBLE
    | TYPE_CHAR
    | TYPE_BOOL
    | TYPE_VOID
    ;

declarador:
    ID
    | ID INITVAR expressao
    ;

declaradores:
    declarador
    | declaradores COMMA declarador
    ;

declaracao:
    tipo declaradores SEMICOLON
    ;

atualizacao:
    ID INITVAR expressao SEMICOLON
    | ID UPDT_PLUS expressao SEMICOLON
    | ID UPDT_MINUS expressao SEMICOLON
    | ID UPDT_TIMES expressao SEMICOLON
    | ID UPDT_DIVIDE expressao SEMICOLON
    | ID UPDT_INC SEMICOLON
    | ID UPDT_DEC SEMICOLON
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
        $$.dado.i = $1;
        printf("Inteiro: %d\n", $$.dado.i);
    }
  | FLOAT_NUM {
        $$.dado.f = $1;
        printf("Float: %f\n", $$.dado.f);
    }
  | STRING {
        $$.dado.s = $1;
        printf("String: %s\n", $$.dado.s);
    }
  | CHAR_LIT {
        $$.dado.c = $1;
        printf("Caractere: %c\n", $$.dado.c);
    }
  | ID {
        $$.dado.s = $1;
        printf("Variável: %s\n", $$.dado.s);
    }

%%

void yyerror(const char *s) {
    fprintf(stderr, "Linha %d, Erro sintático: %s\n", yylineno, s);
}

int main(void) {
    yyparse();
    return 0;
}
