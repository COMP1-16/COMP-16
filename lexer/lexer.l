%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

#define RETURN_TOKEN(tok) \
    do { \
        printf("TOKEN: %s | VALUE: %s\n", #tok, yytext); \
        return tok; \
    } while(0)
%}

%%

    /* data types */

"int"           { RETURN_TOKEN(TYPE_INT); }
"float"         { RETURN_TOKEN(TYPE_FLOAT); }
"double"        { RETURN_TOKEN(TYPE_DOUBLE); }  
"char"          { RETURN_TOKEN(TYPE_CHAR); }
"bool"          { RETURN_TOKEN(TYPE_BOOL); }
"void"          { RETURN_TOKEN(TYPE_VOID); }

    /* identifiers */

[a-zA-Z_]([a-zA-Z0-9_]*) {
    printf("TOKEN: ID | VALUE: %s\n", yytext);
    return ID;
}

    /* numbers */

[0-9]+          { RETURN_TOKEN(NUM); }

    /* assign */

"="             { RETURN_TOKEN(INITVAR); }

    /* arithmetic operators */

"+"             { RETURN_TOKEN(PLUS); }
"-"             { RETURN_TOKEN(MINUS); }
"*"             { RETURN_TOKEN(TIMES); }
"//"            { RETURN_TOKEN(INTDIVIDE); }
"/"             { RETURN_TOKEN(DIVIDE); }
"%"             { RETURN_TOKEN(MOD); }

    /* symbols */

"("             { RETURN_TOKEN(L_PAREN); }
")"             { RETURN_TOKEN(R_PAREN); }
","             { RETURN_TOKEN(COMMA); }
";"             { RETURN_TOKEN(SEMICOLON); }

    /* whitespace */

[ \t\n]+        { /* Ignorar espaços em branco */ }

    /* invalid */

.               { printf("\nCaractere inválido: %s\n\n", yytext); }

%%

int yywrap(void) {
    return 1;
}