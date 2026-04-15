%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUM FLOAT_NUM
%token STRING CHAR_LIT

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

%token ID

%token SEMICOLON
%token COMMA

%token ADDR

    //Operator precedence
%left PLUS MINUS
%left TIMES DIVIDE INTDIVIDE MOD

%%

programa:
    comandos
    ;

comandos:
    comando
    | comandos comando
    ;

comando:
    declaracao
    | atribuicao
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

atribuicao:
    ID INITVAR expressao SEMICOLON
    ;

atualizacao:
    ID UPDT_PLUS expressao SEMICOLON
    | ID UPDT_MINUS expressao SEMICOLON
    | ID UPDT_TIMES expressao SEMICOLON
    | ID UPDT_DIVIDE expressao SEMICOLON
    | ID UPDT_INC SEMICOLON
    | ID UPDT_DEC SEMICOLON
    ;

expressao:
    expressao PLUS expressao
  | expressao MINUS expressao
  | expressao TIMES expressao
  | expressao DIVIDE expressao
  | expressao INTDIVIDE expressao
  | expressao MOD expressao
  | L_PAREN expressao R_PAREN
  | NUM
  | FLOAT_NUM
  | ID
  | STRING
  | CHAR_LIT
  ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}