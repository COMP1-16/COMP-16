#include "funcs.h"

float paraFloat(Valor v) {
    if (v.tipo == TIPO_FLOAT) return v.dado.f;
    return (float)v.dado.i;
}

Valor fazerOperacao(Valor a, Valor b, char op) {
    Valor resultado = {0};

    if (a.tipo == TIPO_INT && b.tipo == TIPO_INT) {
        resultado.tipo = TIPO_INT;
        switch (op) {
            case '+': resultado.dado.i = a.dado.i + b.dado.i; break;
            case '-': resultado.dado.i = a.dado.i - b.dado.i; break;
            case '*': resultado.dado.i = a.dado.i * b.dado.i; break;
            case '/': resultado.dado.i = a.dado.i / b.dado.i; break;
        }
        return resultado;
    }

    float v1 = paraFloat(a);
    float v2 = paraFloat(b);
    resultado.tipo = TIPO_FLOAT;
    switch (op) {
        case '+': resultado.dado.f = v1 + v2; break;
        case '-': resultado.dado.f = v1 - v2; break;
        case '*': resultado.dado.f = v1 * v2; break;
        case '/': resultado.dado.f = v1 / v2; break;
    }
    return resultado;
}
