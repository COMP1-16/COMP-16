%{
#include <stdio.h>
#include <stdlib.h>
#include "lib/ast.h"
#include "lib/types.h"

extern int yylineno;
int  yylex(void);
void yyerror(const char *s);

No *raiz;

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

%token <iValue> NUM
%token <fValue> FLOAT_NUM
%token <cValue> CHAR_LIT
%token <sStr>   STRING
%token <sStr>   ID

%type <no> stmt expr declaracao declaradores declarador atualizacao if_stmt switch_stmt case_stmt while_stmt for_stmt for_init for_cond for_inc
%type <no> bloco stmts switch_cases stmt_list_opt param_list_opt param_list param arg_list_opt arg_list funcao_decl

%token EQUAL DIFF LESS_EQ GREAT_EQ LESSER GREATER
%token UPDT_PLUS UPDT_MINUS UPDT_TIMES UPDT_DIVIDE UPDT_INC UPDT_DEC
%token INITVAR
%token PLUS MINUS TIMES DIVIDE MOD
%token L_PAREN R_PAREN L_SQBRACKET R_SQBRACKET L_CRLRBRACKET R_CRLRBRACKET
%token AND OR NOT ADDR
%token SEMICOLON COMMA COLON
%token KW_RETURN KW_BREAK KW_PRINTF
%token KW_IF KW_ELSE KW_WHILE KW_FOR KW_SWITCH KW_CASE KW_DEFAULT
%token TYPE_INT TYPE_FLOAT TYPE_DOUBLE TYPE_CHAR TYPE_BOOL TYPE_VOID

%left EQUAL DIFF
%left LESSER GREATER LESS_EQ GREAT_EQ
%left PLUS MINUS
%left TIMES DIVIDE MOD
%left AND OR
%right NOT
%right UMINUS

%%

programa
    : stmts  { raiz = $1; }
    ;

bloco
    : L_CRLRBRACKET stmts R_CRLRBRACKET { $$ = $2; }
    | L_CRLRBRACKET R_CRLRBRACKET { $$ = noBloco(NULL, 0); }
    ;

stmts
    : stmts stmt {
        $$ = $1;
        $$->u.bloco.stmts = realloc($$->u.bloco.stmts, ($$->u.bloco.count + 1) * sizeof(No*));
        $$->u.bloco.stmts[$$->u.bloco.count++] = $2;
    }
    | stmt {
        $$ = noBloco(&$1, 1);
    }
    ;

stmt
    : declaracao
    | funcao_decl
    | atualizacao
    | expr SEMICOLON
    | KW_RETURN expr SEMICOLON { $$ = noReturn($2); }
    | KW_BREAK SEMICOLON { $$ = noBreak(); }
    | KW_PRINTF L_PAREN arg_list_opt R_PAREN SEMICOLON { $$ = noPrintf($3); }
    | if_stmt
    | while_stmt
    | for_stmt
    | switch_stmt
    ;

declaracao
    : tipo declaradores SEMICOLON { $$ = $2; }
    ;

funcao_decl
    : tipo ID L_PAREN param_list_opt R_PAREN bloco {
        $$ = noFuncDecl(tipoAtual, $2, $4, $6);
        free($2);
    }
    ;

param_list_opt
    : param_list { $$ = $1; }
    | /* vazio */ { $$ = noBloco(NULL, 0); }
    ;

param_list
    : param_list COMMA param {
        $$ = $1;
        $$->u.bloco.stmts = realloc($$->u.bloco.stmts, ($$->u.bloco.count + 1) * sizeof(No*));
        $$->u.bloco.stmts[$$->u.bloco.count++] = $3;
    }
    | param {
        $$ = noBloco(&$1, 1);
    }
    ;

param
    : tipo ID { $$ = noDecl(tipoAtual, $2); free($2); }
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
    | expr EQUAL expr   { $$ = noRelacional(OP_EQ, $1, $3); }
    | expr DIFF expr    { $$ = noRelacional(OP_NE, $1, $3); }
    | expr LESSER expr  { $$ = noRelacional(OP_LT, $1, $3); }
    | expr GREATER expr { $$ = noRelacional(OP_GT, $1, $3); }
    | expr LESS_EQ expr { $$ = noRelacional(OP_LE, $1, $3); }
    | expr GREAT_EQ expr{ $$ = noRelacional(OP_GE, $1, $3); }
    | expr AND expr     { $$ = noLogicalAnd($1, $3); }
    | expr OR expr      { $$ = noLogicalOr($1, $3); }
    | NOT expr          { $$ = noNot($2); }
    | L_PAREN expr R_PAREN { $$ = $2; }
    | NUM               { $$ = noInt($1);   }
    | FLOAT_NUM         { $$ = noFloat($1); }
    | CHAR_LIT          { $$ = noChar($1);  }
    | STRING            { $$ = noStr($1);   }
    | ID                { $$ = noId($1); free($1); }
    | ID L_PAREN arg_list_opt R_PAREN { $$ = noFuncCall($1, $3); free($1); }
    ;

arg_list_opt
    : arg_list { $$ = $1; }
    | /* vazio */ { $$ = noBloco(NULL, 0); }
    ;

arg_list
    : arg_list COMMA expr {
        $$ = $1;
        $$->u.bloco.stmts = realloc($$->u.bloco.stmts, ($$->u.bloco.count + 1) * sizeof(No*));
        $$->u.bloco.stmts[$$->u.bloco.count++] = $3;
    }
    | expr {
        $$ = noBloco(&$1, 1);
    }
    ;

if_stmt
    : KW_IF expr L_CRLRBRACKET stmts R_CRLRBRACKET { $$ = noIf($2, $4, NULL); }
    | KW_IF expr L_CRLRBRACKET stmts R_CRLRBRACKET KW_ELSE L_CRLRBRACKET stmts R_CRLRBRACKET { $$ = noIf($2, $4, $8); }
    | KW_IF expr L_CRLRBRACKET stmts R_CRLRBRACKET KW_ELSE if_stmt { $$ = noIf($2, $4, $7); }
    ;

case_stmt
    : KW_CASE NUM COLON stmt_list_opt      { $$ = noCaseInt($2, $4); }
    | KW_CASE CHAR_LIT COLON stmt_list_opt { $$ = noCaseChar($2, $4); }
    | KW_DEFAULT COLON stmt_list_opt       { $$ = noDefault($3); }
    ;
stmt_list_opt
    : stmts { $$ = $1; }
    | { $$ = NULL; }/* vazio */
    ;

switch_stmt
    : KW_SWITCH L_PAREN expr R_PAREN L_CRLRBRACKET switch_cases R_CRLRBRACKET { $$ = noSwitch($3, $6); }
    ;

switch_cases
    : switch_cases case_stmt { $$ = $1; $$->u.bloco.stmts = realloc($$->u.bloco.stmts, ($$->u.bloco.count + 1) * sizeof(No*)); $$->u.bloco.stmts[$$->u.bloco.count++] = $2; }
    | case_stmt { $$ = noBloco(&$1, 1); }
    ;

while_stmt
    : KW_WHILE L_PAREN expr R_PAREN stmt { $$ = noWhile($3, $5); }
    ;

for_stmt
    : KW_FOR L_PAREN for_init SEMICOLON for_cond SEMICOLON for_inc R_PAREN stmt
        { $$ = noFor($3, $5, $7, $9); }
    ;

for_init
    : declaracao          { $$ = $1; }
    | expr SEMICOLON      { $$ = $1; }
    | SEMICOLON           { $$ = NULL; }
    ;

for_cond
    : expr                { $$ = $1; }
    |                     { $$ = NULL; }
    ;

for_inc
    : expr                { $$ = $1; }
    |                     { $$ = NULL; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "[Sintaxe] Linha %d: %s\n", yylineno, s);
}