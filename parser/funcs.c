#include <stdio.h>
#include "funcs.h"

float para_float(Valor v) {
    return v.tipo == TIPO_FLOAT ? v.dado.f : (float)v.dado.i;
}

Valor fazer_operacao(Valor a, Valor b, char op) {
    Valor resultado;
    float v1 = para_float(a);
    float v2 = para_float(b);
    
    resultado.tipo = TIPO_FLOAT;
    switch(op) {
        case '+': resultado.dado.f = v1 + v2; break;
        case '-': resultado.dado.f = v1 - v2; break;
        case '*': resultado.dado.f = v1 * v2; break;
        case '/': resultado.dado.f = v1 / v2; break;
    }
    
    printf("Operação %c: %f\n", op, resultado.dado.f);
    return resultado;
}