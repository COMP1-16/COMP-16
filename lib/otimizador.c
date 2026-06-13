#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "otimizador.h"
#include "math.h"

/* ================================================================== *
 * liberarNo — libera recursivamente uma subárvore                     *
 * ================================================================== */
void liberarNo(No *no) {
    if (!no) return;

    switch (no->tipo) {
        case NO_BINOP:
        case NO_RELACIONAL:
        case NO_LOGICAL_AND:
        case NO_LOGICAL_OR:
            liberarNo(no->u.bin.esq);
            liberarNo(no->u.bin.dir);
            break;
        case NO_NOT:
            liberarNo(no->u.not.expr);
            break;
        case NO_IF:
            liberarNo(no->u.if_stmt.cond);
            liberarNo(no->u.if_stmt.thenBranch);
            liberarNo(no->u.if_stmt.elseBranch);
            break;
        case NO_WHILE:
            liberarNo(no->u.while_stmt.cond);
            liberarNo(no->u.while_stmt.body);
            break;
        case NO_FOR:
            liberarNo(no->u.for_stmt.init);
            liberarNo(no->u.for_stmt.cond);
            liberarNo(no->u.for_stmt.inc);
            liberarNo(no->u.for_stmt.body);
            break;
        case NO_DECL_INIT:
        case NO_ATRIB:
        case NO_ATRIB_OP:
            liberarNo(no->u.bin.esq);
            break;
        case NO_RETURN:
            liberarNo(no->u.bin.esq);
            break;
        case NO_BLOCO:
            for (int i = 0; i < no->u.bloco.count; i++)
                liberarNo(no->u.bloco.stmts[i]);
            free(no->u.bloco.stmts);
            break;
        case NO_PRINTF:
        case NO_FUNC_CALL:
            liberarNo(no->u.call.args);
            break;
        case NO_FUNC_DECL:
            liberarNo(no->u.func_decl.params);
            liberarNo(no->u.func_decl.body);
            break;
        case NO_STR:
            free(no->sval);
            break;
        default:
            break;
    }

    free(no->nome);
    free(no);
}

/* ================================================================== *
 * Helpers internos                                                    *
 * ================================================================== */

/* Retorna 1 se o nó é um literal numérico inteiro */
static int ehLiteralInt(No *n) {
    return n && n->tipo == NO_INT;
}

/* Retorna 1 se o nó é um literal numérico float */
static int ehLiteralFloat(No *n) {
    return n && n->tipo == NO_FLOAT;
}

/* Retorna 1 se o nó é qualquer literal numérico */
static int ehLiteralNum(No *n) {
    return ehLiteralInt(n) || ehLiteralFloat(n);
}

/* Extrai valor float de um literal numérico (int ou float) */
static float valorFloat(No *n) {
    if (n->tipo == NO_FLOAT) return n->fval;
    return (float)n->ival;
}

/* Retorna 1 se o literal é "truthy" (não-zero) */
static int ehTruthy(No *n) {
    if (!n) return 0;
    if (n->tipo == NO_INT)   return n->ival != 0;
    if (n->tipo == NO_FLOAT) return n->fval != 0.0f;
    return 0;
}

/* Condição literal falsa/verdadeira (BOOL/INT) */
static int ehLiteralFalso(No *n) {
    return n && n->tipo == NO_INT && n->ival == 0;
}

static int ehLiteralVerdadeiro(No *n) {
    return n && n->tipo == NO_INT && n->ival != 0;
}

/* ================================================================== *
 * Constant folding para BINOP                                         *
 * ================================================================== */
static No *dobrarBinop(No *no) {
    No *esq = no->u.bin.esq;
    No *dir = no->u.bin.dir;
    char op = no->u.bin.op;

    if (!ehLiteralNum(esq) || !ehLiteralNum(dir)) return no;

    if (ehLiteralInt(esq) && ehLiteralInt(dir)) {
        int a = esq->ival, b = dir->ival;
        int res = 0;
        switch (op) {
            case '+': res = a + b; break;
            case '-': res = a - b; break;
            case '*': res = a * b; break;
            case '/':
                if (b == 0) return no;
                res = a / b;
                break;
            default: return no;
        }
        No *novo = noInt(res);
        liberarNo(no);
        return novo;
    }

    float a = valorFloat(esq), b = valorFloat(dir);
    float res = 0.0f;
    switch (op) {
        case '+': res = a + b; break;
        case '-': res = a - b; break;
        case '*': res = a * b; break;
        case '/':
            if (b == 0.0f) return no;
            res = a / b;
            break;
        default: return no;
    }
    No *novo = noFloat(res);
    liberarNo(no);
    return novo;
}

/* ================================================================== *
 * Simplificações algébricas para BINOP                               *
 * (aplicadas DEPOIS do folding, quando um lado ainda não é literal)  *
 * ================================================================== */
static No *simplificarBinop(No *no) {
    No *esq = no->u.bin.esq;
    No *dir = no->u.bin.dir;
    char op = no->u.bin.op;

    switch (op) {
        case '+':
            /* x + 0 → x */
            if (ehLiteralInt(dir) && dir->ival == 0) {
                No *r = esq; no->u.bin.esq = NULL; dir->ival = 0;
                no->u.bin.esq = NULL;
                free(no->nome); free(no);
                liberarNo(dir);
                return r;
            }
            /* 0 + x → x */
            if (ehLiteralInt(esq) && esq->ival == 0) {
                No *r = dir; no->u.bin.dir = NULL;
                free(no->nome); free(no);
                liberarNo(esq);
                return r;
            }
            break;
        case '-':
            /* x - 0 → x */
            if (ehLiteralInt(dir) && dir->ival == 0) {
                No *r = esq; no->u.bin.esq = NULL;
                free(no->nome); free(no);
                liberarNo(dir);
                return r;
            }
            break;
        case '*':
            /* x * 0 → 0 */
            if ((ehLiteralInt(dir) && dir->ival == 0) ||
                (ehLiteralFloat(dir) && dir->fval == 0.0f)) {
                No *zero = ehLiteralFloat(esq) ? noFloat(0.0f) : noInt(0);
                liberarNo(no);
                return zero;
            }
            /* 0 * x → 0 */
            if ((ehLiteralInt(esq) && esq->ival == 0) ||
                (ehLiteralFloat(esq) && esq->fval == 0.0f)) {
                No *zero = ehLiteralFloat(dir) ? noFloat(0.0f) : noInt(0);
                liberarNo(no);
                return zero;
            }
            /* x * 1 → x */
            if (ehLiteralInt(dir) && dir->ival == 1) {
                No *r = esq; no->u.bin.esq = NULL;
                free(no->nome); free(no);
                liberarNo(dir);
                return r;
            }
            /* 1 * x → x */
            if (ehLiteralInt(esq) && esq->ival == 1) {
                No *r = dir; no->u.bin.dir = NULL;
                free(no->nome); free(no);
                liberarNo(esq);
                return r;
            }
            break;
        case '/':
            /* x / 1 → x */
            if (ehLiteralInt(dir) && dir->ival == 1) {
                No *r = esq; no->u.bin.esq = NULL;
                free(no->nome); free(no);
                liberarNo(dir);
                return r;
            }
            break;
    }
    return no;
}

static No *transformarIncrDecr(No *no) {
    /* ++x → x = x + 1    e    --x → x = x - 1 */
    if (no->tipo != NO_INC && no->tipo != NO_DEC) return no;

    char op = (no->tipo == NO_INC) ? '+' : '-';
    No *id = noId(no->nome);
    No *um = noInt(1);
    No *bin = noBinop(op, id, um);
    No *atrib = noAtrib(strdup(no->nome), bin);

    liberarNo(no);
    return atrib;
}

/* ================================================================== *
 * Simplificação de ATRIB_OP (x += 0, x *= 1, folding do RHS)        *
 * ================================================================== */
static No *simplificarAtribOp(No *no) {
    No *expr = no->u.bin.esq;
    char op = no->u.bin.op;

    if (expr && expr->tipo == NO_BINOP) {
        No *folded = dobrarBinop(expr);
        if (folded != expr) {
            no->u.bin.esq = folded;
            expr = folded;
        }
        if (expr->tipo == NO_BINOP) {
            No *simpl = simplificarBinop(expr);
            if (simpl != expr) {
                no->u.bin.esq = simpl;
                expr = simpl;
            }
        }
    }

    if ((op == '+' || op == '-') && ehLiteralInt(expr) && expr->ival == 0) {
        liberarNo(no);
        return noBloco(NULL, 0);
    }
    if ((op == '*' || op == '/') && ehLiteralInt(expr) && expr->ival == 1) {
        liberarNo(no);
        return noBloco(NULL, 0);
    }
    return no;
}

/* ================================================================== *
 * Remove blocos vazios (no-ops) e código após return                  *
 * ================================================================== */
static No *compactarBloco(No *no) {
    int novo_count = 0;
    for (int i = 0; i < no->u.bloco.count; i++) {
        No *stmt = no->u.bloco.stmts[i];
        if (stmt && stmt->tipo == NO_BLOCO && stmt->u.bloco.count == 0) {
            liberarNo(stmt);
            continue;
        }
        no->u.bloco.stmts[novo_count++] = stmt;
    }
    if (novo_count == no->u.bloco.count) return no;

    if (novo_count == 0) {
        free(no->u.bloco.stmts);
        no->u.bloco.stmts = NULL;
    } else {
        No **novos = realloc(no->u.bloco.stmts, (size_t)novo_count * sizeof(No *));
        if (novos) no->u.bloco.stmts = novos;
    }
    no->u.bloco.count = novo_count;
    return no;
}

static No *eliminarDeadCodeAposReturn(No *no) {
    int return_idx = -1;
    for (int i = 0; i < no->u.bloco.count; i++) {
        if (no->u.bloco.stmts[i]->tipo == NO_RETURN) {
            return_idx = i;
            break;
        }
    }
    if (return_idx < 0) return no;

    for (int i = return_idx + 1; i < no->u.bloco.count; i++)
        liberarNo(no->u.bloco.stmts[i]);

    no->u.bloco.count = return_idx + 1;
    return no;
}

/* ================================================================== *
 * Constant folding para RELACIONAL                                    *
 * ================================================================== */
static No *dobrarRelacional(No *no) {
    No *esq = no->u.bin.esq;
    No *dir = no->u.bin.dir;

    if (!ehLiteralNum(esq) || !ehLiteralNum(dir)) return no;

    float a = valorFloat(esq), b = valorFloat(dir);
    int res = 0;

    switch (no->relop) {
        case OP_EQ: res = (a == b); break;
        case OP_NE: res = (a != b); break;
        case OP_LT: res = (a <  b); break;
        case OP_GT: res = (a >  b); break;
        case OP_LE: res = (a <= b); break;
        case OP_GE: res = (a >= b); break;
    }

    No *novo = noInt(res);
    liberarNo(no);
    return novo;
}

/* ================================================================== *
 * Constant folding para LOGICAL AND / OR                             *
 * ================================================================== */
static No *dobrarLogicalAnd(No *no) {
    No *esq = no->u.bin.esq;
    No *dir = no->u.bin.dir;

    /* 0 && qualquer → 0 */
    if (ehLiteralInt(esq) && esq->ival == 0) {
        No *zero = noInt(0);
        liberarNo(no);
        return zero;
    }
    /* qualquer && 0 → 0 */
    if (ehLiteralInt(dir) && dir->ival == 0) {
        No *zero = noInt(0);
        liberarNo(no);
        return zero;
    }
    /* 1 && x → x */
    if (ehLiteralInt(esq) && esq->ival != 0) {
        No *r = dir; no->u.bin.dir = NULL;
        free(no->nome); free(no);
        liberarNo(esq);
        return r;
    }
    /* x && 1 → x */
    if (ehLiteralInt(dir) && dir->ival != 0) {
        No *r = esq; no->u.bin.esq = NULL;
        free(no->nome); free(no);
        liberarNo(dir);
        return r;
    }
    /* ambos literais não-zero → 1 */
    if (ehLiteralNum(esq) && ehLiteralNum(dir)) {
        No *um = noInt(ehTruthy(esq) && ehTruthy(dir) ? 1 : 0);
        liberarNo(no);
        return um;
    }
    return no;
}

static No *dobrarLogicalOr(No *no) {
    No *esq = no->u.bin.esq;
    No *dir = no->u.bin.dir;

    /* 1 || qualquer → 1 */
    if (ehLiteralInt(esq) && esq->ival != 0) {
        No *um = noInt(1);
        liberarNo(no);
        return um;
    }
    /* qualquer || 1 → 1 */
    if (ehLiteralInt(dir) && dir->ival != 0) {
        No *um = noInt(1);
        liberarNo(no);
        return um;
    }
    /* 0 || x → x */
    if (ehLiteralInt(esq) && esq->ival == 0) {
        No *r = dir; no->u.bin.dir = NULL;
        free(no->nome); free(no);
        liberarNo(esq);
        return r;
    }
    /* x || 0 → x */
    if (ehLiteralInt(dir) && dir->ival == 0) {
        No *r = esq; no->u.bin.esq = NULL;
        free(no->nome); free(no);
        liberarNo(dir);
        return r;
    }
    /* ambos literais → avalia */
    if (ehLiteralNum(esq) && ehLiteralNum(dir)) {
        No *res = noInt(ehTruthy(esq) || ehTruthy(dir) ? 1 : 0);
        liberarNo(no);
        return res;
    }
    return no;
}

/* ================================================================== *
 * Constant folding para NOT                                           *
 * ================================================================== */
static No *dobrarNot(No *no) {
    No *expr = no->u.not.expr;

    if (!ehLiteralNum(expr)) return no;

    No *res = noInt(!ehTruthy(expr));
    liberarNo(no);
    return res;
}

/* ================================================================== *
 * Dead Code Elimination para IF                                       *
 * ================================================================== */
static No *eliminarDeadCodeIf(No *no) {
    No *cond = no->u.if_stmt.cond;

    if (!ehLiteralInt(cond)) return no;

    if (ehLiteralVerdadeiro(cond)) {
        /* if(true) → sempre executa then, descarta else */
        No *then = no->u.if_stmt.thenBranch;
        no->u.if_stmt.thenBranch = NULL;
        liberarNo(no);
        return then;
    } else {
        /* if(false) → nunca executa then, mantém else (ou bloco vazio) */
        No *els = no->u.if_stmt.elseBranch;
        no->u.if_stmt.elseBranch = NULL;
        liberarNo(no);

        if (els) return els;

        /* sem else: retorna bloco vazio para não quebrar o pai */
        No **stmts = NULL;
        return noBloco(stmts, 0);
    }
}

/* ================================================================== *
 * Dead Code Elimination para WHILE                                   *
 * ================================================================== */
static No *eliminarDeadCodeWhile(No *no) {
    No *cond = no->u.while_stmt.cond;

    /* while(0) → nunca executa, descarta inteiro */
    if (ehLiteralFalso(cond)) {
        No **stmts = NULL;
        liberarNo(no);
        return noBloco(stmts, 0);
    }
    return no;
}

/* ================================================================== *
 * Dead Code Elimination para FOR                                     *
 * ================================================================== */
static No *eliminarDeadCodeFor(No *no) {
    No *cond = no->u.for_stmt.cond;

    /* for(...; 0; ...) → executa só o init, descarta corpo/inc/cond */
    if (ehLiteralFalso(cond)) {
        No *init = no->u.for_stmt.init;
        no->u.for_stmt.init = NULL;
        liberarNo(no);
        if (init) return init;
        return noBloco(NULL, 0);
    }
    return no;
}

/* ================================================================== *
 * Função principal — percurso bottom-up da AST                       *
 * ================================================================== */
No *otimizar(No *no) {
    if (!no) return NULL;

    switch (no->tipo) {
        case NO_BINOP:
        case NO_RELACIONAL:
        case NO_LOGICAL_AND:
        case NO_LOGICAL_OR:
            no->u.bin.esq = otimizar(no->u.bin.esq);
            no->u.bin.dir = otimizar(no->u.bin.dir);
            break;

        case NO_NOT:
            no->u.not.expr = otimizar(no->u.not.expr);
            break;

        case NO_IF:
            no->u.if_stmt.cond = otimizar(no->u.if_stmt.cond);
            no->u.if_stmt.thenBranch = otimizar(no->u.if_stmt.thenBranch);
            no->u.if_stmt.elseBranch = otimizar(no->u.if_stmt.elseBranch);
            break;

        case NO_WHILE:
            no->u.while_stmt.cond = otimizar(no->u.while_stmt.cond);
            no->u.while_stmt.body = otimizar(no->u.while_stmt.body);
            break;

        case NO_FOR:
            no->u.for_stmt.init = otimizar(no->u.for_stmt.init);
            no->u.for_stmt.cond = otimizar(no->u.for_stmt.cond);
            no->u.for_stmt.inc  = otimizar(no->u.for_stmt.inc);
            no->u.for_stmt.body = otimizar(no->u.for_stmt.body);
            break;

        case NO_DECL_INIT:
        case NO_ATRIB:
        case NO_ATRIB_OP:
            no->u.bin.esq = otimizar(no->u.bin.esq);
            break;

        case NO_RETURN:
            no->u.bin.esq = otimizar(no->u.bin.esq);
            break;

        case NO_BLOCO:
            for (int i = 0; i < no->u.bloco.count; i++)
                no->u.bloco.stmts[i] = otimizar(no->u.bloco.stmts[i]);
            no = compactarBloco(no);
            no = eliminarDeadCodeAposReturn(no);
            break;

        case NO_PRINTF:
        case NO_FUNC_CALL:
            if (no->u.call.args)
                no->u.call.args = otimizar(no->u.call.args);
            break;

        case NO_FUNC_DECL:
            /* NÃO otimiza params — são declarações com tipoDeclarado, não expressões */
            no->u.func_decl.body = otimizar(no->u.func_decl.body);
            break;

        /* Folhas: sem filhos para otimizar */
        case NO_INT:
        case NO_FLOAT:
        case NO_CHAR:
        case NO_STR:
        case NO_ID:
        case NO_DECL:
        case NO_INCLUDE_MATH:
            return no;

        default:
            break;
    }

    /* ── Subida: aplica transformações no nó atual ── */
    switch (no->tipo) {

        case NO_BINOP:
            no = dobrarBinop(no);
            /* se ainda é BINOP (não dobrou completamente), tenta algébrica */
            if (no->tipo == NO_BINOP)
                no = simplificarBinop(no);
            break;

        case NO_RELACIONAL:
            no = dobrarRelacional(no);
            break;

        case NO_LOGICAL_AND:
            no = dobrarLogicalAnd(no);
            break;

        case NO_LOGICAL_OR:
            no = dobrarLogicalOr(no);
            break;

        case NO_NOT:
            no = dobrarNot(no);
            break;

        case NO_IF:
            no = eliminarDeadCodeIf(no);
            break;

        case NO_WHILE:
            no = eliminarDeadCodeWhile(no);
            break;

        case NO_FOR:
            no = eliminarDeadCodeFor(no);
            break;

        case NO_ATRIB_OP:
            no = simplificarAtribOp(no);
            break;

        case NO_FUNC_CALL:
            if ((strcmp(no->nome, "sqrt") == 0 || strcmp(no->nome, "abs") == 0 ||
                 strcmp(no->nome, "floor") == 0 || strcmp(no->nome, "ceil") == 0 ||
                 strcmp(no->nome, "round") == 0) &&
                no->u.call.args && no->u.call.args->u.bloco.count == 1) {
                
                No *arg = no->u.call.args->u.bloco.stmts[0];
                if (ehLiteralNum(arg)) {
                    float fval = valorFloat(arg);
                    float res = 0;
                    if (strcmp(no->nome, "sqrt") == 0) res = math_sqrt(fval);
                    else if (strcmp(no->nome, "abs") == 0) res = math_abs(fval);
                    else if (strcmp(no->nome, "floor") == 0) res = math_floor(fval);
                    else if (strcmp(no->nome, "ceil") == 0) res = math_ceil(fval);
                    else if (strcmp(no->nome, "round") == 0) res = math_round(fval);
                    
                    liberarNo(no);
                    return noFloat(res);
                }
            }
            if (strcmp(no->nome, "pow") == 0 && no->u.call.args && no->u.call.args->u.bloco.count == 2) {
                No *arg1 = no->u.call.args->u.bloco.stmts[0];
                No *arg2 = no->u.call.args->u.bloco.stmts[1];
                if (ehLiteralNum(arg1) && ehLiteralNum(arg2)) {
                    float res = math_pow(valorFloat(arg1), valorFloat(arg2));
                    liberarNo(no);
                    return noFloat(res);
                }
            }
            break;

        case NO_INC:
        case NO_DEC:
            no = transformarIncrDecr(no);
            return otimizar(no);

        default:
            break;
    }

    return no;
}