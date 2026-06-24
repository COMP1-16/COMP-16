%option yylineno nounput noinput
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/ast/ast.h"
#include "parser.tab.h"
%}

%%

    /* --- includes --- */
"#include <math.h>" { return INCLUDE_MATH; }
"#include <stdlib.h>" { return INCLUDE_STDLIB; }

    /* --- espaço e comentários --- */
[ \t\r\n]+          { /* ignora espaço em branco */ }
"//"[^\n]*          { /* ignora comentário de linha */ }

    /* --- estruturas condicionais --- */
"if"                { return KW_IF;     }
"else"              { return KW_ELSE;   }
"while"             { return KW_WHILE;  }
"for"               { return KW_FOR;    }

    /* --- estruturas de switch case --- */

"switch"            { return KW_SWITCH; }
"case"              { return KW_CASE; }
"default"           { return KW_DEFAULT; }


    /* --- palavras-chave --- */
"return"            { return KW_RETURN; }
"break"             { return KW_BREAK; }
"continue"          { return KW_CONTINUE; }
"printf"            { return KW_PRINTF; }

    /* --- tipos --- */
"int"               { return TYPE_INT;    }
"float"             { return TYPE_FLOAT;  }
"double"            { return TYPE_DOUBLE; }
"char"              { return TYPE_CHAR;   }
"bool"              { return TYPE_BOOL;   }
"void"              { return TYPE_VOID;   }
"string"            { return TYPE_STRING; }

    /* --- literais --- */
[0-9]+\.[0-9]+      { yylval.fValue = atof(yytext);  return FLOAT_NUM; }
[0-9]+              { yylval.iValue = atoi(yytext);  return NUM;       }
\"([^\"\\]|\\.)*\"  {
                      /* remove as aspas */
                      yylval.sStr = strdup(yytext + 1);
                      yylval.sStr[strlen(yylval.sStr) - 1] = '\0';
                      return STRING;
                    }
\'([^'\\]|\\.)\'    { yylval.cValue = yytext[1]; return CHAR_LIT; }

    /* --- identificadores --- */
[a-zA-Z_][a-zA-Z0-9_]*  {
                      yylval.sStr = strdup(yytext);
                      return ID;
                    }

    /* --- operadores relacionais --- */
"=="                { return EQUAL;    }
"!="                { return DIFF;     }
"<="                { return LESS_EQ;  }
">="                { return GREAT_EQ; }
"<"                 { return LESSER;   }
">"                 { return GREATER;  }

    /* --- operadores de atualização --- */
"+="                { return UPDT_PLUS;   }
"-="                { return UPDT_MINUS;  }
"*="                { return UPDT_TIMES;  }
"/="                { return UPDT_DIVIDE; }
"++"                { return UPDT_INC;    }
"--"                { return UPDT_DEC;    }

    /* --- atribuição --- */
"="                 { return INITVAR; }

    /* --- operadores aritméticos --- */
"+"                 { return PLUS;   }
"-"                 { return MINUS;  }
"*"                 { return TIMES;  }
"/"                 { return DIVIDE; }
"%"                 { return MOD;    }

    /* --- operadores lógicos --- */
"&&"                { return AND;  }
"||"                { return OR;   }
"!"                 { return NOT;  }

    /* --- endereço --- */
"&"                 { return ADDR; }

    /* --- pontuação --- */
"("                 { return L_PAREN;        }
")"                 { return R_PAREN;        }
"["                 { return L_SQBRACKET;    }
"]"                 { return R_SQBRACKET;    }
"{"                 { return L_CRLRBRACKET;  }
"}"                 { return R_CRLRBRACKET;  }
","                 { return COMMA;          }
";"                 { return SEMICOLON;      }
":"                 { return COLON; }

    /* --- caractere inválido --- */
.                   { fprintf(stderr, "[Lexico] Linha %d: caractere invalido '%s'\n",
                               yylineno, yytext); }

%%

int yywrap(void) { return 1; }
