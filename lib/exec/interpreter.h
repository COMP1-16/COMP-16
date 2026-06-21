#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "types.h"
#include "ast.h"
#include "simbolos.h"

Valor avaliar(No *no, Celula **tabela);

#endif