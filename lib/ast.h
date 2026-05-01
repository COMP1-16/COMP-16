#ifndef AST_H
#define AST_H

typedef enum {
    NO_INT,      
    NO_FLOAT,    
    NO_CHAR,     
    NO_STR,      
    NO_ID,       
    NO_BINOP,    
    NO_DECL,     
    NO_DECL_INIT,
    NO_ATRIB,    
    NO_ATRIB_OP, 
    NO_INC,      
    NO_DEC,      
    NO_BLOCO,    
    NO_RETURN    
} TipoNo;

typedef struct No {
    TipoNo tipo;

    /* literais */
    int    ival;   /* NO_INT                  */
    float  fval;   /* NO_FLOAT                */
    char   cval;   /* NO_CHAR                 */
    char  *sval;   /* NO_STR                  */

    /* nome da variável (NO_ID, NO_DECL, NO_DECL_INIT, NO_ATRIB, NO_ATRIB_OP, NO_INC, NO_DEC) */
    char  *nome;

    /* tipo declarado (NO_DECL, NO_DECL_INIT) — código TIPO_* de types.h */
    int    tipoDeclarado;

    /* operador (NO_BINOP, NO_ATRIB_OP) */
    char   op;

    /* filhos */
    struct No *esq;   /* operando esquerdo / inicializador / expressão */
    struct No *dir;   /* operando direito                              */

    /* NO_BLOCO */
    struct No **stmts;
    int         count;
} No;

/* ------------------------------------------------------------------ *
 * Construtores                                                        *
 * ------------------------------------------------------------------ */
No *noInt     (int val);
No *noFloat   (float val);
No *noChar    (char val);
No *noStr     (char *val);
No *noId      (char *nome);
No *noBinop   (char op, No *esq, No *dir);
No *noDecl    (int tipoDeclarado, char *nome);
No *noDeclInit(int tipoDeclarado, char *nome, No *expr);
No *noAtrib   (char *nome, No *expr);
No *noAtribOp (char op, char *nome, No *expr);
No *noInc     (char *nome);
No *noDec     (char *nome);
No *noBloco   (No **stmts, int count);
No *noReturn  (No *expr);

#endif
