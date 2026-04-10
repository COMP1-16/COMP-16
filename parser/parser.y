%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUM

//Comparison Operators
%token EQUAL DIFF LESS_EQ GREAT_EQ LESSER GREATER

//Update Operators
%token UPDT_PLUS UPDT_MINUS UPDT_TIMES UPDT_DIVIDE

%token INITVAR

//Math Operators
%token PLUS MINUS TIMES DIVIDE INTDIVIDE MOD

//Brackets
%token L_PAREN R_PAREN L_SQBRACKET R_SQBRACKET L_CRLRBRACKET R_CRLRBRACKET

//Logical Operators
%token AND OR NOT

//Types: Definição dos Tokens(Símbolos Terminais) dos tipos.
%token TYPE_INT TYPE_FLOAT TYPE_DOUBLE TYPE_CHAR TYPE_BOOL TYPE_VOID

%token ID

%token SEMICOLON
%token COMMA

%token ADDR

%%
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



expressao:
    expressao PLUS expressao
  | expressao MINUS expressao
  | expressao TIMES expressao
  | expressao DIVIDE expressao
  | L_PAREN expressao R_PAREN
  | NUM
  ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}