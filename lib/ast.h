#ifndef AST_H
#define AST_H

typedef enum {
    NO_INT,
    NO_FLOAT,
    NO_CHAR,
    NO_STR,
    NO_ID,
    NO_BINOP,
    NO_RELACIONAL,
    NO_IF,
    NO_WHILE,
    NO_FOR,
    NO_DECL,
    NO_DECL_INIT,
    NO_ATRIB,
    NO_ATRIB_OP,
    NO_INC,
    NO_DEC,
    NO_BLOCO,
    NO_RETURN,
    NO_LOGICAL_AND,
    NO_LOGICAL_OR,
    NO_NOT,
    NO_PRINTF,
    NO_FUNC_DECL,
    NO_FUNC_CALL,
    NO_INCLUDE_MATH,
    NO_INCLUDE_STDLIB,
    NO_INCLUDE_CTYPE,
} TipoNo;

typedef enum {
    OP_EQ,
    OP_NE,
    OP_LT,
    OP_GT,
    OP_LE,
    OP_GE
} TipoRelacional;

typedef struct No {
    TipoNo tipo;

    /* campos comuns a vários nós */
    int    ival;      /* NO_INT */
    float  fval;      /* NO_FLOAT */
    char   cval;      /* NO_CHAR */
    char  *sval;      /* NO_STR */
    char  *nome;      /* NO_ID, NO_DECL, NO_DECL_INIT, NO_ATRIB, NO_ATRIB_OP, NO_INC, NO_DEC */
    int    tipoDeclarado; /* NO_DECL, NO_DECL_INIT */

    /* operador relacional (apenas para NO_RELACIONAL) */
    TipoRelacional relop;

    /* union com estruturas específicas para cada tipo de nó */
    union {
        /* NO_BINOP, NO_LOGICAL_AND, NO_LOGICAL_OR, NO_ATRIB_OP, NO_RELACIONAL (usa bin) */
        struct {
            struct No *esq;
            struct No *dir;
            char       op;      /* '+', '-', '*', '/', etc. */
        } bin;

        /* NO_NOT */
        struct {
            struct No *expr;
        } not;

        /* NO_IF */
        struct {
            struct No *cond;
            struct No *thenBranch;
            struct No *elseBranch;
        } if_stmt;

        /* NO_WHILE */
        struct {
            struct No *cond;
            struct No *body;
        } while_stmt;

        /* NO_FOR */
        struct {
            struct No *init;
            struct No *cond;
            struct No *inc;
            struct No *body;
        } for_stmt;

        /* NO_BLOCO */
        struct {
            struct No **stmts;
            int         count;
        } bloco;
        
        /* NO_PRINTF e NO_FUNC_CALL */
        struct {
            struct No *args;
        } call;
        
        /* NO_FUNC_DECL */
        struct {
            struct No *params;
            struct No *body;
        } func_decl;
    } u;
} No;

/* Construtores */
No *noInt(int val);
No *noFloat(float val);
No *noChar(char val);
No *noStr(char *val);
No *noId(char *nome);
No *noBinop(char op, No *esq, No *dir);
No *noDecl(int tipoDeclarado, char *nome);
No *noDeclInit(int tipoDeclarado, char *nome, No *expr);
No *noAtrib(char *nome, No *expr);
No *noAtribOp(char op, char *nome, No *expr);
No *noInc(char *nome);
No *noDec(char *nome);
No *noBloco(No **stmts, int count);
No *noReturn(No *expr);
No *noRelacional(TipoRelacional op, No *esq, No *dir);
No *noIf(No *cond, No *thenBranch, No *elseBranch);
No *noWhile(No *cond, No *body);
No *noFor(No *init, No *cond, No *inc, No *body);
No *noLogicalAnd(No *esq, No *dir);
No *noLogicalOr(No *esq, No *dir);
No *noNot(No *expr);
No *noPrintf(No *args);
No *noFuncDecl(int tipoRetorno, char *nome, No *params, No *body);
No *noFuncCall(char *nome, No *args);
No *noIncludeMath();
No *noIncludeStdlib();
No *noIncludeCtype();

#endif