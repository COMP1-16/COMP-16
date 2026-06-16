#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "ast.h"
#include "simbolos.h"
#include "types.h"

int checar(No *raiz, Celula **tabela);
void registrar_math(Celula **tabela);
void registrar_stdlib(Celula **tabela);

#endif