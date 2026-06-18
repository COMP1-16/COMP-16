#ifndef OTIMIZADOR_H
#define OTIMIZADOR_H

#include "ast.h"

/*
 * Percorre a AST de forma bottom-up e aplica as seguintes transformações:
 *
 *  1. Constant Folding     — avalia expressões cujos operandos são literais.
 *  2. Algebraic Simplif.   — identidades algébricas (x+0, x*1, x+=0, etc).
 *  3. Dead Code Elimination— if(0), while(0), for(...;0;...) → removidos.
 *  4. Transformação de INC/DEC → binop (para aproveitar folding)
 *
 * Retorna a raiz da AST otimizada (pode ser um nó diferente do original).
 * O nó original pode ser liberado internamente quando substituído.
 */
No *otimizar(No *no);

/*
 * Libera recursivamente todos os nós de uma (sub-)AST.
 * Usado internamente pelo otimizador ao descartar ramos mortos,
 * mas também pode ser chamado externamente.
 */
void liberarNo(No *no);

#endif /* OTIMIZADOR_H */