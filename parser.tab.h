/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 14 "parser/parser.y"

#include "parser/types.h"

#line 53 "parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUM = 258,                     /* NUM  */
    STRING = 259,                  /* STRING  */
    CHAR_LIT = 260,                /* CHAR_LIT  */
    FLOAT_NUM = 261,               /* FLOAT_NUM  */
    ID = 262,                      /* ID  */
    EQUAL = 263,                   /* EQUAL  */
    DIFF = 264,                    /* DIFF  */
    LESS_EQ = 265,                 /* LESS_EQ  */
    GREAT_EQ = 266,                /* GREAT_EQ  */
    LESSER = 267,                  /* LESSER  */
    GREATER = 268,                 /* GREATER  */
    UPDT_PLUS = 269,               /* UPDT_PLUS  */
    UPDT_MINUS = 270,              /* UPDT_MINUS  */
    UPDT_TIMES = 271,              /* UPDT_TIMES  */
    UPDT_DIVIDE = 272,             /* UPDT_DIVIDE  */
    UPDT_INC = 273,                /* UPDT_INC  */
    UPDT_DEC = 274,                /* UPDT_DEC  */
    INITVAR = 275,                 /* INITVAR  */
    PLUS = 276,                    /* PLUS  */
    MINUS = 277,                   /* MINUS  */
    TIMES = 278,                   /* TIMES  */
    DIVIDE = 279,                  /* DIVIDE  */
    INTDIVIDE = 280,               /* INTDIVIDE  */
    MOD = 281,                     /* MOD  */
    L_PAREN = 282,                 /* L_PAREN  */
    R_PAREN = 283,                 /* R_PAREN  */
    L_SQBRACKET = 284,             /* L_SQBRACKET  */
    R_SQBRACKET = 285,             /* R_SQBRACKET  */
    L_CRLRBRACKET = 286,           /* L_CRLRBRACKET  */
    R_CRLRBRACKET = 287,           /* R_CRLRBRACKET  */
    AND = 288,                     /* AND  */
    OR = 289,                      /* OR  */
    NOT = 290,                     /* NOT  */
    TYPE_INT = 291,                /* TYPE_INT  */
    TYPE_FLOAT = 292,              /* TYPE_FLOAT  */
    TYPE_DOUBLE = 293,             /* TYPE_DOUBLE  */
    TYPE_CHAR = 294,               /* TYPE_CHAR  */
    TYPE_BOOL = 295,               /* TYPE_BOOL  */
    TYPE_VOID = 296,               /* TYPE_VOID  */
    SEMICOLON = 297,               /* SEMICOLON  */
    COMMA = 298,                   /* COMMA  */
    KW_RETURN = 299,               /* KW_RETURN  */
    ADDR = 300                     /* ADDR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "parser/parser.y"

    int iValue;
    float fValue;
    char cValue;
    char *sStr;
    Valor valor;

#line 123 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
