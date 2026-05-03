%{
#include <stdio.h>
#include <stdlib.h>
#include "lib/ast.h"
#include "lib/types.h"

extern int yylineno;
int  yylex(void);
void yyerror(const char *s);

No *raiz;

/* Buffer para acumular statements (tipos de comando) no bloco raiz */
#define MAX_STMTS 1024
static No  *stmtBuf[MAX_STMTS];
static int  stmtCount = 0;

static int tipoAtual;
%}

%code requires {
#include "lib/ast.h"
#include "lib/types.h"
}

%union {
    int   iValue;
    float fValue;
    char  cValue;
    char *sStr;
    No   *no;
}

/* --- tokens com valor --- */
%token <iValue> NUM
%token <fValue> FLOAT_NUM
%token <cValue> CHAR_LIT
%token <sStr>   STRING
%token <sStr>   ID

/* --- tipos de regras que produzem nós --- */
%type <no> stmt expr declaracao declaradores declarador atualizacao

/* --- tokens de operadores e pontuação --- */
%token EQUAL DIFF LESS_EQ GREAT_EQ LESSER GREATER
%token UPDT_PLUS UPDT_MINUS UPDT_TIMES UPDT_DIVIDE UPDT_INC UPDT_DEC
%token INITVAR
%token PLUS MINUS TIMES DIVIDE MOD
%token L_PAREN R_PAREN L_SQBRACKET R_SQBRACKET L_CRLRBRACKET R_CRLRBRACKET
%token AND OR NOT ADDR
%token SEMICOLON COMMA
%token KW_RETURN

/* --- tipos primitivos --- */
%token TYPE_INT TYPE_FLOAT TYPE_DOUBLE TYPE_CHAR TYPE_BOOL TYPE_VOID

/* --- precedência e associatividade --- */
%left  PLUS MINUS
%left  TIMES DIVIDE MOD
%right UMINUS

%%

programa
    : lista_stmts  { raiz = noBloco(stmtBuf, stmtCount); }
    ;

lista_stmts
    : lista_stmts stmt  { stmtBuf[stmtCount++] = $2; }
    | stmt              { stmtBuf[stmtCount++] = $1; }
    ;

stmt
    : declaracao        { $$ = $1; }
    | atualizacao       { $$ = $1; }
    | expr SEMICOLON    { $$ = $1; }
    | KW_RETURN expr SEMICOLON { $$ = noReturn($2); }
    ;

declaracao
    : tipo declaradores SEMICOLON { $$ = $2; }
    ;

tipo
    : TYPE_INT    { tipoAtual = TIPO_INT;    }
    | TYPE_FLOAT  { tipoAtual = TIPO_FLOAT;  }
    | TYPE_DOUBLE { tipoAtual = TIPO_DOUBLE; }
    | TYPE_CHAR   { tipoAtual = TIPO_CHAR;   }
    | TYPE_BOOL   { tipoAtual = TIPO_BOOL;   }
    | TYPE_VOID   { tipoAtual = TIPO_VOID;   }
    ;

declaradores
    : declaradores COMMA declarador { $$ = $3; }
    | declarador                    { $$ = $1; }
    ;

declarador
    : ID               { $$ = noDecl(tipoAtual, $1);       free($1); }
    | ID INITVAR expr  { $$ = noDeclInit(tipoAtual, $1, $3); free($1); }
    ;

atualizacao
    : ID INITVAR expr SEMICOLON        { $$ = noAtrib($1, $3);         free($1); }
    | ID UPDT_PLUS  expr SEMICOLON     { $$ = noAtribOp('+', $1, $3);  free($1); }
    | ID UPDT_MINUS expr SEMICOLON     { $$ = noAtribOp('-', $1, $3);  free($1); }
    | ID UPDT_TIMES expr SEMICOLON     { $$ = noAtribOp('*', $1, $3);  free($1); }
    | ID UPDT_DIVIDE expr SEMICOLON    { $$ = noAtribOp('/', $1, $3);  free($1); }
    | ID UPDT_INC SEMICOLON            { $$ = noInc($1);               free($1); }
    | ID UPDT_DEC SEMICOLON            { $$ = noDec($1);               free($1); }
    ;

expr
    : expr PLUS  expr   { $$ = noBinop('+', $1, $3); }
    | expr MINUS expr   { $$ = noBinop('-', $1, $3); }
    | expr TIMES expr   { $$ = noBinop('*', $1, $3); }
    | expr DIVIDE expr  { $$ = noBinop('/', $1, $3); }
    | L_PAREN expr R_PAREN    { $$ = $2; }
    | NUM       { $$ = noInt($1);   }
    | FLOAT_NUM { $$ = noFloat($1); }
    | CHAR_LIT  { $$ = noChar($1);  }
    | STRING    { $$ = noStr($1);   }
    | ID        { $$ = noId($1); free($1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "[Sintaxe] Linha %d: %s\n", yylineno, s);
}
