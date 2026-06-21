#include "comp_stdlib.h"
#include <stdlib.h>

static void pular_espacos(const char **s) {
    while (**s == ' ' || **s == '\t' || **s == '\n' || **s == '\r')
        (*s)++;
}

int stdlib_atoi(const char *s) {
    if (!s) return 0;

    const char *p = s;
    pular_espacos(&p);

    int negativo = 0;
    if (*p == '-') {
        negativo = 1;
        p++;
    } else if (*p == '+') {
        p++;
    }

    int resultado = 0;
    while (*p >= '0' && *p <= '9') {
        resultado = resultado * 10 + (*p - '0');
        p++;
    }

    return negativo ? -resultado : resultado;
}

float stdlib_atof(const char *s) {
    if (!s) return 0.0f;

    const char *p = s;
    pular_espacos(&p);

    int negativo = 0;
    if (*p == '-') {
        negativo = 1;
        p++;
    } else if (*p == '+') {
        p++;
    }

    float resultado = 0.0f;
    while (*p >= '0' && *p <= '9') {
        resultado = resultado * 10.0f + (float)(*p - '0');
        p++;
    }

    if (*p == '.') {
        p++;
        float frac = 0.1f;
        while (*p >= '0' && *p <= '9') {
            resultado += (float)(*p - '0') * frac;
            frac *= 0.1f;
            p++;
        }
    }

    return negativo ? -resultado : resultado;
}

void stdlib_exit(int code) {
    exit(code);
}
