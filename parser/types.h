#ifndef TYPES_H
#define TYPES_H

#define TIPO_INT   0
#define TIPO_FLOAT 1
#define TIPO_CHAR  2
#define TIPO_STR   3

typedef struct {
    int tipo;
    union {
        int i;
        float f;
        char c;
        char *s;
    } dado;
} Valor;

#endif