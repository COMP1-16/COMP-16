#ifndef TYPES_H
#define TYPES_H

/* ------------------------------------------------------------------ *
 * Códigos de tipo                                                      *
 * ------------------------------------------------------------------ */
#define TIPO_ERRO    -1
#define TIPO_INT      0
#define TIPO_FLOAT    1
#define TIPO_CHAR     2
#define TIPO_STR      3
#define TIPO_DOUBLE   4
#define TIPO_BOOL     5
#define TIPO_VOID     6
#define TIPO_FUNC     7

struct No; /* Forward declaration */

/* Valor em runtime */
typedef struct {
    int tipo;
    union {
        int    i;
        float  f;
        double d;
        char   c;
        char  *s;
        struct No *func_ast;
    } dado;
} Valor;

#endif
