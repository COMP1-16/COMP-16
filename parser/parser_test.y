%{
#include <stdio.h>
#include <stdlib.h>

extern int yylineno;
int  yylex(void);
void yyerror(const char *s);

#define REDUCE(x) printf("[REDUCE] %s\n", x)
%}

%union {
    int   iValue;
    float fValue;
    char  cValue;
    char *sStr;
}

%token <iValue> NUM
%token <fValue> FLOAT_NUM
%token <cValue> CHAR_LIT
%token <sStr>   STRING
%token <sStr>   ID

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
    : stmts  { REDUCE("programa"); printf("SUCESSO\n"); }
    ;

bloco
    : L_CRLRBRACKET stmts R_CRLRBRACKET { REDUCE("bloco -> { stmts }"); }
    | L_CRLRBRACKET R_CRLRBRACKET       { REDUCE("bloco -> { }"); }
    ;

stmts
    : stmts stmt { REDUCE("stmts -> stmts stmt"); }
    | stmt       { REDUCE("stmts -> stmt"); }
    ;

stmt
    : declaracao                                              { REDUCE("stmt -> declaracao"); }
    | funcao_decl                                            { REDUCE("stmt -> funcao_decl"); }
    | atualizacao                                            { REDUCE("stmt -> atualizacao"); }
    | expr SEMICOLON                                         { REDUCE("stmt -> expr ;"); }
    | KW_RETURN expr SEMICOLON                               { REDUCE("stmt -> return expr ;"); }
    | KW_BREAK SEMICOLON                                    { REDUCE("stmt -> break ;"); }
    | KW_PRINTF L_PAREN arg_list_opt R_PAREN SEMICOLON      { REDUCE("stmt -> printf(arg_list_opt) ;"); }
    | if_stmt                                                { REDUCE("stmt -> if_stmt"); }
    | while_stmt                                             { REDUCE("stmt -> while_stmt"); }
    | for_stmt                                               { REDUCE("stmt -> for_stmt"); }
    | switch_stmt                                            { REDUCE("stmt -> switch_stmt"); }
    ;

declaracao
    : tipo declaradores SEMICOLON { REDUCE("declaracao -> tipo declaradores ;"); }
    ;

funcao_decl
    : tipo ID L_PAREN param_list_opt R_PAREN bloco { REDUCE("funcao_decl"); free($2); }
    ;

param_list_opt
    : param_list  { REDUCE("param_list_opt -> param_list"); }
    | /* vazio */ { REDUCE("param_list_opt -> (vazio)"); }
    ;

param_list
    : param_list COMMA param { REDUCE("param_list -> param_list , param"); }
    | param                  { REDUCE("param_list -> param"); }
    ;

param
    : tipo ID { REDUCE("param -> tipo ID"); free($2); }
    ;

tipo
    : TYPE_INT    { REDUCE("tipo -> int"); }
    | TYPE_FLOAT  { REDUCE("tipo -> float"); }
    | TYPE_DOUBLE { REDUCE("tipo -> double"); }
    | TYPE_CHAR   { REDUCE("tipo -> char"); }
    | TYPE_BOOL   { REDUCE("tipo -> bool"); }
    | TYPE_VOID   { REDUCE("tipo -> void"); }
    ;

declaradores
    : declaradores COMMA declarador { REDUCE("declaradores -> declaradores , declarador"); }
    | declarador                    { REDUCE("declaradores -> declarador"); }
    ;

declarador
    : ID               { REDUCE("declarador -> ID");            free($1); }
    | ID INITVAR expr  { REDUCE("declarador -> ID := expr");    free($1); }
    ;

atualizacao
    : ID INITVAR expr SEMICOLON     { REDUCE("atualizacao -> ID := expr ;");   free($1); }
    | ID UPDT_PLUS  expr SEMICOLON  { REDUCE("atualizacao -> ID += expr ;");   free($1); }
    | ID UPDT_MINUS expr SEMICOLON  { REDUCE("atualizacao -> ID -= expr ;");   free($1); }
    | ID UPDT_TIMES expr SEMICOLON  { REDUCE("atualizacao -> ID *= expr ;");   free($1); }
    | ID UPDT_DIVIDE expr SEMICOLON { REDUCE("atualizacao -> ID /= expr ;");   free($1); }
    | ID UPDT_INC SEMICOLON         { REDUCE("atualizacao -> ID++ ;");         free($1); }
    | ID UPDT_DEC SEMICOLON         { REDUCE("atualizacao -> ID-- ;");         free($1); }
    ;

expr
    : expr PLUS  expr    { REDUCE("expr -> expr + expr"); }
    | expr MINUS expr    { REDUCE("expr -> expr - expr"); }
    | expr TIMES expr    { REDUCE("expr -> expr * expr"); }
    | expr DIVIDE expr   { REDUCE("expr -> expr / expr"); }
    | expr EQUAL expr    { REDUCE("expr -> expr == expr"); }
    | expr DIFF expr     { REDUCE("expr -> expr != expr"); }
    | expr LESSER expr   { REDUCE("expr -> expr < expr"); }
    | expr GREATER expr  { REDUCE("expr -> expr > expr"); }
    | expr LESS_EQ expr  { REDUCE("expr -> expr <= expr"); }
    | expr GREAT_EQ expr { REDUCE("expr -> expr >= expr"); }
    | expr AND expr      { REDUCE("expr -> expr && expr"); }
    | expr OR expr       { REDUCE("expr -> expr || expr"); }
    | NOT expr           { REDUCE("expr -> ! expr"); }
    | L_PAREN expr R_PAREN              { REDUCE("expr -> ( expr )"); }
    | NUM                               { REDUCE("expr -> NUM"); }
    | FLOAT_NUM                         { REDUCE("expr -> FLOAT_NUM"); }
    | CHAR_LIT                          { REDUCE("expr -> CHAR_LIT"); }
    | STRING                            { REDUCE("expr -> STRING"); free($1); }
    | ID                                { REDUCE("expr -> ID"); free($1); }
    | ID L_PAREN arg_list_opt R_PAREN  { REDUCE("expr -> ID ( arg_list_opt )"); free($1); }
    ;

arg_list_opt
    : arg_list    { REDUCE("arg_list_opt -> arg_list"); }
    | /* vazio */ { REDUCE("arg_list_opt -> (vazio)"); }
    ;

arg_list
    : arg_list COMMA expr { REDUCE("arg_list -> arg_list , expr"); }
    | expr                { REDUCE("arg_list -> expr"); }
    ;

if_stmt
    : KW_IF expr L_CRLRBRACKET stmts R_CRLRBRACKET                                              { REDUCE("if_stmt -> if expr { stmts }"); }
    | KW_IF expr L_CRLRBRACKET stmts R_CRLRBRACKET KW_ELSE L_CRLRBRACKET stmts R_CRLRBRACKET  { REDUCE("if_stmt -> if expr { stmts } else { stmts }"); }
    | KW_IF expr L_CRLRBRACKET stmts R_CRLRBRACKET KW_ELSE if_stmt                             { REDUCE("if_stmt -> if expr { stmts } else if_stmt"); }
    ;

case_stmt
    : KW_CASE NUM COLON stmt_list_opt      { REDUCE("case_stmt -> case NUM : stmt_list_opt"); }
    | KW_CASE CHAR_LIT COLON stmt_list_opt { REDUCE("case_stmt -> case CHAR :stmt_list_opts"); }
    | KW_DEFAULT COLON stmt_list_opt       { REDUCE("case_stmt -> default : stmt_list_opt"); }
    ;

stmt_list_opt
    : stmts { REDUCE("stmt_list_opt -> stmts"); }
    | { REDUCE("stmt_list_opt -> "); }/* vazio */
    ;

switch_stmt
    : KW_SWITCH L_PAREN expr R_PAREN L_CRLRBRACKET switch_cases R_CRLRBRACKET { REDUCE("switch_stmt"); }
    ;

switch_cases
    : switch_cases case_stmt { REDUCE("switch_cases -> switch_cases case_stmt"); }
    | case_stmt              { REDUCE("switch_cases -> case_stmt"); }
    ;

while_stmt
    : KW_WHILE L_PAREN expr R_PAREN stmt { REDUCE("while_stmt"); }
    ;

for_stmt
    : KW_FOR L_PAREN for_init SEMICOLON for_cond SEMICOLON for_inc R_PAREN stmt { REDUCE("for_stmt"); }
    ;

for_init
    : declaracao     { REDUCE("for_init -> declaracao"); }
    | expr SEMICOLON { REDUCE("for_init -> expr ;"); }
    | SEMICOLON      { REDUCE("for_init -> (vazio)"); }
    ;

for_cond
    : expr        { REDUCE("for_cond -> expr"); }
    | /* vazio */ { REDUCE("for_cond -> (vazio)"); }
    ;

for_inc
    : expr        { REDUCE("for_inc -> expr"); }
    | /* vazio */ { REDUCE("for_inc -> (vazio)"); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "[FALHA] Linha %d: %s\n", yylineno, s);
}

int main(void)
{
    if (yyparse() == 0)
        printf("\nSUCESSO\n");

    return 0;
}