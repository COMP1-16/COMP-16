#ifndef FUNCS_H
#define FUNCS_H

#include "types.h"

/* Converte qualquer Valor numérico para float. */
float paraFloat(Valor v);

/* Executa op (+, -, *, /) sobre dois Valores. */
Valor fazerOperacao(Valor a, Valor b, char op);

#endif
