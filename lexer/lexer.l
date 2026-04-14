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

    /* keywords */

"if"            { RETURN_TOKEN(KW_IF); }
"else"          { RETURN_TOKEN(KW_ELSE); }      
"while"         { RETURN_TOKEN(KW_WHILE); }
"for"           { RETURN_TOKEN(KW_FOR); }
"do"            { RETURN_TOKEN(KW_DO); }
"switch"        { RETURN_TOKEN(KW_SWITCH); }
"case"          { RETURN_TOKEN(KW_CASE); }
"break"         { RETURN_TOKEN(KW_BREAK); }
"continue"      { RETURN_TOKEN(KW_CONTINUE); }
"default"       { RETURN_TOKEN(KW_DEFAULT); }
"return"        { RETURN_TOKEN(KW_RETURN); }

    /* data types */

"int"           { RETURN_TOKEN(TYPE_INT); }
"float"         { RETURN_TOKEN(TYPE_FLOAT); }
"double"        { RETURN_TOKEN(TYPE_DOUBLE); }  
"char"          { RETURN_TOKEN(TYPE_CHAR); }
"bool"          { RETURN_TOKEN(TYPE_BOOL); }
"void"          { RETURN_TOKEN(TYPE_VOID); }

    /* identifiers */

[a-zA-Z_]([a-zA-Z0-9_])* {
                                yylval.sIndex = malloc(strlen(yytext)+1);
                                strcpy(yylval.sIndex, yytext);
                                printf("TOKEN: ID | VALUE: %s\n", yytext);
                                return ID;
                            }

    /* strings */               

\"[^\"\\]*\"           {
                                yylval.sValue = strdup(yytext+1);
                                yylval.sValue[strlen(yylval.sValue)-1] = '\0';
                                printf("TOKEN: STRING | VALUE: %s\n", yylval.sValue);
                                return STRING;
                            }

    /* characters */

\'[^\'\\]\'            {
                                yylval.cValue = *(yytext+1);
                                printf("TOKEN: CHAR_LIT | VALUE: %c\n", yylval.cValue);
                                return CHAR_LIT;
                            }

    /* numbers */

[0-9]+\.[0-9]+         { RETURN_TOKEN(FLOAT_NUM); }
[0-9]+                 { 
                                //yylval = atoi(yytext); 
                                RETURN_TOKEN(NUM); 
                            }


    /* relational operators */

"=="            { RETURN_TOKEN(EQUAL); }
"!="            { RETURN_TOKEN(DIFF); }
"<="            { RETURN_TOKEN(LESS_EQ); }
">="            { RETURN_TOKEN(GREAT_EQ); }
"<"             { RETURN_TOKEN(LESSER); }
">"             { RETURN_TOKEN(GREATER); }

    /* assign */

"++"            { RETURN_TOKEN(UPDT_INC); }
"--"            { RETURN_TOKEN(UPDT_DEC); }
"+="            { RETURN_TOKEN(UPDT_PLUS); }
"-="            { RETURN_TOKEN(UPDT_MINUS); }
"*="            { RETURN_TOKEN(UPDT_TIMES); }
"/="            { RETURN_TOKEN(UPDT_DIVIDE); }
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
"["             { RETURN_TOKEN(L_SQBRACKET); }
"]"             { RETURN_TOKEN(R_SQBRACKET); }
"{"             { RETURN_TOKEN(L_CRLRBRACKET); }
"}"             { RETURN_TOKEN(R_CRLRBRACKET); }
","             { RETURN_TOKEN(COMMA); }
";"             { RETURN_TOKEN(SEMICOLON); }

    /* logical operators */

"&&"            { RETURN_TOKEN(AND); }
"||"            { RETURN_TOKEN(OR); }
"!"             { RETURN_TOKEN(NOT); }

    /* adress operator */

"&"             { RETURN_TOKEN(ADDR); }

    /* whitespace */

[ \t\n]+        { /* Ignorar espaços em branco */ }

    /* invalid */

.               { printf("\nCaractere inválido: %s\n\n", yytext); }

%%

int yywrap(void) {
    return 1;
}