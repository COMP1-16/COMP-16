%option yylineno
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser/types.h"
#include "parser.tab.h"

#define RETURN_TOKEN(tok) \
    do { \
        printf("TOKEN: %s | VALUE: %s\n", #tok, yytext); \
        switch (tok) { \
            case NUM: \
                yylval.iValue = atoi(yytext); \
                break; \
            case FLOAT_NUM: \
                yylval.fValue = atof(yytext); \
                break; \
            case STRING: \
                yylval.sStr = strdup(yytext+1); \
                yylval.sStr[strlen(yylval.sStr)-1] = '\0'; \
                printf("TOKEN: STRING | VALUE: %s\n", yylval.sStr); \
                break; \
            case ID: \
                yylval.sStr = malloc(strlen(yytext)+1); \
                strcpy(yylval.sStr, yytext); \
                printf("TOKEN: ID | VALUE: %s\n", yytext); \
                break; \
            case CHAR_LIT: \
                yylval.cValue = yytext[1]; \
                printf("TOKEN: CHAR_LIT | VALUE: %s\n", yytext); \
                break; \
            default: \
                break; \
        } \
        return tok; \
    } while(0)
%}

%%
    /* keywords */

"return"        { RETURN_TOKEN(KW_RETURN); }


    /* data types */

"int"           { RETURN_TOKEN(TYPE_INT); }
"float"         { RETURN_TOKEN(TYPE_FLOAT); }
"double"        { RETURN_TOKEN(TYPE_DOUBLE); }
"char"          { RETURN_TOKEN(TYPE_CHAR); }
"bool"          { RETURN_TOKEN(TYPE_BOOL); }
"void"          { RETURN_TOKEN(TYPE_VOID); }

    /* identifiers */

[a-zA-Z_]([a-zA-Z0-9_])* { RETURN_TOKEN(ID); }

    /* strings */

\"[^\"\\]*\"           { RETURN_TOKEN(STRING); }

    /* characters */

\'([^'\\]|\\.)?\'   { RETURN_TOKEN(CHAR_LIT); }

    /* numbers */

[0-9]+\.[0-9]+         { RETURN_TOKEN(FLOAT_NUM); }
[0-9]+                 { RETURN_TOKEN(NUM); }


    /* relational operators */

"=="            { RETURN_TOKEN(EQUAL); }
"!="            { RETURN_TOKEN(DIFF); }
"<="            { RETURN_TOKEN(LESS_EQ); }
">="            { RETURN_TOKEN(GREAT_EQ); }
"<"             { RETURN_TOKEN(LESSER); }
">"             { RETURN_TOKEN(GREATER); }

    /* update operators */

"+="            { RETURN_TOKEN(UPDT_PLUS); }
"-="            { RETURN_TOKEN(UPDT_MINUS); }
"*="            { RETURN_TOKEN(UPDT_TIMES); }
"/="            { RETURN_TOKEN(UPDT_DIVIDE); }
"++"            { RETURN_TOKEN(UPDT_INC); }
"--"            { RETURN_TOKEN(UPDT_DEC); }

    /* assign */

"="             { RETURN_TOKEN(INITVAR); }

    /* arithmetic operators */

"+"             { RETURN_TOKEN(PLUS); }
"-"             { RETURN_TOKEN(MINUS); }
"*"             { RETURN_TOKEN(TIMES); }
"//"            { RETURN_TOKEN(INTDIVIDE); }
"/"             { RETURN_TOKEN(DIVIDE); }
"%"             { RETURN_TOKEN(MOD); }

    /* logical operators */

"&&"            { RETURN_TOKEN(AND); }
"||"            { RETURN_TOKEN(OR); }
"!"             { RETURN_TOKEN(NOT); }

    /* address operator */

"&"             { RETURN_TOKEN(ADDR); }

    /* symbols */

"("             { RETURN_TOKEN(L_PAREN); }
")"             { RETURN_TOKEN(R_PAREN); }
"["             { RETURN_TOKEN(L_SQBRACKET); }
"]"             { RETURN_TOKEN(R_SQBRACKET); }
"{"             { RETURN_TOKEN(L_CRLRBRACKET); }
"}"             { RETURN_TOKEN(R_CRLRBRACKET); }
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
