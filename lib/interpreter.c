#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"
#include "interpreter.h"
#include "math.h"
#include "stdlib.h"
#include "semantico.h"

static int compatibilidade_casting(int tipo) {
    return (tipo == TIPO_INT || tipo == TIPO_FLOAT || tipo == TIPO_DOUBLE ||
            tipo == TIPO_CHAR || tipo == TIPO_BOOL);
}

static Valor coercionar(Valor v, int tipoAlvo) {
    if (v.tipo == tipoAlvo) return v;
    Valor resultado = v;
    resultado.tipo = tipoAlvo;
    if (compatibilidade_casting(v.tipo) && compatibilidade_casting(tipoAlvo)) {
        switch (tipoAlvo) {
            case TIPO_INT:
                if (v.tipo == TIPO_FLOAT)       resultado.dado.i = (int)v.dado.f;
                else if (v.tipo == TIPO_DOUBLE) resultado.dado.i = (int)v.dado.d;
                else if (v.tipo == TIPO_CHAR)   resultado.dado.i = (int)v.dado.c;
                else if (v.tipo == TIPO_BOOL)   resultado.dado.i = (int)v.dado.i;
                break;
            case TIPO_FLOAT:
                if (v.tipo == TIPO_INT)         resultado.dado.f = (float)v.dado.i;
                else if (v.tipo == TIPO_DOUBLE) resultado.dado.f = (float)v.dado.d;
                else if (v.tipo == TIPO_CHAR)   resultado.dado.f = (float)v.dado.c;
                else if (v.tipo == TIPO_BOOL)   resultado.dado.f = (float)v.dado.i;
                break;
            case TIPO_DOUBLE:
                if (v.tipo == TIPO_INT)         resultado.dado.d = (double)v.dado.i;
                else if (v.tipo == TIPO_FLOAT)  resultado.dado.d = (double)v.dado.f;
                else if (v.tipo == TIPO_CHAR)   resultado.dado.d = (double)v.dado.c;
                else if (v.tipo == TIPO_BOOL)   resultado.dado.d = (double)v.dado.i;
                break;
            case TIPO_CHAR:
                if (v.tipo == TIPO_INT)         resultado.dado.c = (char)v.dado.i;
                else if (v.tipo == TIPO_FLOAT)  resultado.dado.c = (char)v.dado.f;
                else if (v.tipo == TIPO_DOUBLE) resultado.dado.c = (char)v.dado.d;
                else if (v.tipo == TIPO_BOOL)   resultado.dado.c = (char)v.dado.i;
                break;
            case TIPO_BOOL:
                if (v.tipo == TIPO_INT)         resultado.dado.i = (v.dado.i != 0) ? 1 : 0;
                else if (v.tipo == TIPO_FLOAT)  resultado.dado.i = (v.dado.f != 0.0f) ? 1 : 0;
                else if (v.tipo == TIPO_DOUBLE) resultado.dado.i = (v.dado.d != 0.0) ? 1 : 0;
                else if (v.tipo == TIPO_CHAR)   resultado.dado.i = (v.dado.c != 0) ? 1 : 0;
                break;
        }
    }
    return resultado;
}

static int has_returned = 0;
static Valor return_val = {0};
static Celula **tabela_global = NULL;

static Celula *buscarSimboloInterpretador(char *nome, Celula **tabela) {
    Celula *c = buscarSimbolo(nome, tabela);
    if (!c && tabela_global && tabela != tabela_global) {
        c = buscarSimbolo(nome, tabela_global);
    }
    return c;
}

Valor avaliar(No *no, Celula **tabela) {
    Valor resultado = {0};
    if (!no) return resultado;
    
    if (!tabela_global) tabela_global = tabela;

    switch (no->tipo) {
        case NO_INCLUDE_MATH:
            registrar_math(tabela);
            return resultado;
        case NO_INCLUDE_STDLIB:
            registrar_stdlib(tabela);
            return resultado;
        case NO_INT:
            resultado.tipo = TIPO_INT;
            resultado.dado.i = no->ival;
            return resultado;
        case NO_FLOAT:
            resultado.tipo = TIPO_FLOAT;
            resultado.dado.f = no->fval;
            return resultado;
        case NO_CHAR:
            resultado.tipo = TIPO_CHAR;
            resultado.dado.c = no->cval;
            return resultado;
        case NO_STR:
            resultado.tipo = TIPO_STR;
            resultado.dado.s = no->sval;
            return resultado;
        case NO_ID: {
            Celula *c = buscarSimboloInterpretador(no->nome, tabela);
            if (!c) {
                fprintf(stderr, "[Runtime] Erro: variavel '%s' nao encontrada\n", no->nome);
                exit(1);
            }
            return c->valor;
        }
        case NO_BINOP: {
            Valor esq = avaliar(no->u.bin.esq, tabela);
            Valor dir = avaliar(no->u.bin.dir, tabela);
            if (no->u.bin.op == '/') {
                int zeroint   = (dir.tipo == TIPO_INT  && dir.dado.i == 0);
                int zerofloat = (dir.tipo == TIPO_FLOAT && dir.dado.f == 0.0f);
                if (zeroint || zerofloat) {
                    fprintf(stderr, "[Runtime] Erro: divisao por zero\n");
                    exit(1);
                }
            }
            return fazerOperacao(esq, dir, no->u.bin.op);
        }
        case NO_DECL: {
            Valor vazio = {0};
            vazio.tipo = no->tipoDeclarado;
            inserirSimbolo(no->nome, no->tipoDeclarado, vazio, tabela);
            return vazio;
        }
        case NO_DECL_INIT: {
            Valor val = avaliar(no->u.bin.esq, tabela);
            val = coercionar(val, no->tipoDeclarado);
            inserirSimbolo(no->nome, no->tipoDeclarado, val, tabela);
            return val;
        }
        case NO_ATRIB: {
            Celula *c = buscarSimboloInterpretador(no->nome, tabela);
            if (!c) {
                fprintf(stderr, "[Runtime] Erro: variavel '%s' nao encontrada\n", no->nome);
                exit(1);
            }
            Valor val = avaliar(no->u.bin.esq, tabela);
            val = coercionar(val, c->tipo);
            c->valor = val;
            return val;
        }
        case NO_ATRIB_OP: {
            Celula *c = buscarSimboloInterpretador(no->nome, tabela);
            if (!c) {
                fprintf(stderr, "[Runtime] Erro: variavel '%s' nao encontrada\n", no->nome);
                exit(1);
            }
            Valor atual = c->valor;
            Valor expr = avaliar(no->u.bin.esq, tabela);
            Valor novo = fazerOperacao(atual, expr, no->u.bin.op);
            novo = coercionar(novo, c->tipo);
            c->valor = novo;
            return novo;
        }
        case NO_INC: {
            Celula *c = buscarSimboloInterpretador(no->nome, tabela);
            Valor um = { .tipo = c->tipo };
            if (c->tipo == TIPO_FLOAT) um.dado.f = 1.0f;
            else                       um.dado.i = 1;
            c->valor = fazerOperacao(c->valor, um, '+');
            return c->valor;
        }
        case NO_DEC: {
            Celula *c = buscarSimboloInterpretador(no->nome, tabela);
            Valor um = { .tipo = c->tipo };
            if (c->tipo == TIPO_FLOAT) um.dado.f = 1.0f;
            else                       um.dado.i = 1;
            c->valor = fazerOperacao(c->valor, um, '-');
            return c->valor;
        }
        case NO_RELACIONAL: {
            Valor esq = avaliar(no->u.bin.esq, tabela);
            Valor dir = avaliar(no->u.bin.dir, tabela);
            resultado.tipo = TIPO_BOOL;
            float v1 = (esq.tipo == TIPO_FLOAT) ? esq.dado.f : esq.dado.i;
            float v2 = (dir.tipo == TIPO_FLOAT) ? dir.dado.f : dir.dado.i;
            switch (no->relop) {
                case OP_EQ: resultado.dado.i = (v1 == v2); break;
                case OP_NE: resultado.dado.i = (v1 != v2); break;
                case OP_LT: resultado.dado.i = (v1 <  v2); break;
                case OP_GT: resultado.dado.i = (v1 >  v2); break;
                case OP_LE: resultado.dado.i = (v1 <= v2); break;
                case OP_GE: resultado.dado.i = (v1 >= v2); break;
                default: fprintf(stderr, "[Runtime] Operador relacional invalido\n"); exit(1);
            }
            return resultado;
        }
        case NO_IF: {
            Valor cond = avaliar(no->u.if_stmt.cond, tabela);
            int verdadeiro = (cond.tipo == TIPO_BOOL || cond.tipo == TIPO_INT) ? cond.dado.i : 0;
            if (verdadeiro) {
                resultado = avaliar(no->u.if_stmt.thenBranch, tabela);
                if (has_returned) return return_val;
            } else if (no->u.if_stmt.elseBranch) {
                resultado = avaliar(no->u.if_stmt.elseBranch, tabela);
                if (has_returned) return return_val;
            }
            return resultado;
        }
        case NO_WHILE: {
            while (1) {
                Valor cond = avaliar(no->u.while_stmt.cond, tabela);
                int verdadeiro = (cond.tipo == TIPO_BOOL || cond.tipo == TIPO_INT) ? cond.dado.i : 0;
                if (!verdadeiro) break;
                avaliar(no->u.while_stmt.body, tabela);
                if (has_returned) return return_val;
            }
            return resultado;
        }
        case NO_FOR: {
            if (no->u.for_stmt.init) avaliar(no->u.for_stmt.init, tabela);
            while (1) {
                int verdadeiro = 1;
                if (no->u.for_stmt.cond) {
                    Valor cond = avaliar(no->u.for_stmt.cond, tabela);
                    verdadeiro = (cond.tipo == TIPO_BOOL || cond.tipo == TIPO_INT) ? cond.dado.i : 0;
                }
                if (!verdadeiro) break;
                avaliar(no->u.for_stmt.body, tabela);
                if (has_returned) return return_val;
                if (no->u.for_stmt.inc) avaliar(no->u.for_stmt.inc, tabela);
            }
            return resultado;
        }
        case NO_LOGICAL_AND: {
            Valor esq = avaliar(no->u.bin.esq, tabela);
            int v_esq = (esq.tipo == TIPO_BOOL || esq.tipo == TIPO_INT) ? esq.dado.i : 0;
            if (!v_esq) {
                resultado.tipo = TIPO_BOOL;
                resultado.dado.i = 0;
                return resultado;
            }
            Valor dir = avaliar(no->u.bin.dir, tabela);
            int v_dir = (dir.tipo == TIPO_BOOL || dir.tipo == TIPO_INT) ? dir.dado.i : 0;
            resultado.tipo = TIPO_BOOL;
            resultado.dado.i = v_dir;
            return resultado;
        }
        case NO_LOGICAL_OR: {
            Valor esq = avaliar(no->u.bin.esq, tabela);
            int v_esq = (esq.tipo == TIPO_BOOL || esq.tipo == TIPO_INT) ? esq.dado.i : 0;
            if (v_esq) {
                resultado.tipo = TIPO_BOOL;
                resultado.dado.i = 1;
                return resultado;
            }
            Valor dir = avaliar(no->u.bin.dir, tabela);
            int v_dir = (dir.tipo == TIPO_BOOL || dir.tipo == TIPO_INT) ? dir.dado.i : 0;
            resultado.tipo = TIPO_BOOL;
            resultado.dado.i = v_dir;
            return resultado;
        }
        case NO_NOT: {
            Valor v = avaliar(no->u.not.expr, tabela);
            int val = (v.tipo == TIPO_BOOL || v.tipo == TIPO_INT) ? v.dado.i : 0;
            resultado.tipo = TIPO_BOOL;
            resultado.dado.i = !val;
            return resultado;
        }
        case NO_BLOCO:
            for (int i = 0; i < no->u.bloco.count; i++) {
                resultado = avaliar(no->u.bloco.stmts[i], tabela);
                if (has_returned) return return_val;
            }
            return resultado;
        case NO_RETURN:
            return_val = avaliar(no->u.bin.esq, tabela);
            has_returned = 1;
            return return_val;
        case NO_PRINTF: {
            No *args = no->u.call.args;
            if (!args || args->u.bloco.count == 0) return resultado;
            Valor vfmt = avaliar(args->u.bloco.stmts[0], tabela);
            if (vfmt.tipo != TIPO_STR) {
                fprintf(stderr, "[Runtime] Erro: printf requer string literal como primeiro argumento\n");
                exit(1);
            }
            char *fmt = vfmt.dado.s;
            int arg_idx = 1;
            for (int i = 0; fmt[i]; i++) {
                if (fmt[i] == '%' && fmt[i+1]) {
                    i++;
                    if (arg_idx >= args->u.bloco.count) {
                        fprintf(stderr, "[Runtime] Erro: argumentos insuficientes para printf\n");
                        exit(1);
                    }
                    Valor arg_val = avaliar(args->u.bloco.stmts[arg_idx++], tabela);
                    if (fmt[i] == 'd' || fmt[i] == 'i') {
                        printf("%d", (arg_val.tipo == TIPO_FLOAT) ? (int)arg_val.dado.f : arg_val.dado.i);
                    } else if (fmt[i] == 'f') {
                        printf("%f", (arg_val.tipo == TIPO_INT) ? (float)arg_val.dado.i : arg_val.dado.f);
                    } else if (fmt[i] == 's') {
                        printf("%s", arg_val.dado.s);
                    } else if (fmt[i] == 'c') {
                        printf("%c", arg_val.dado.c);
                    } else {
                        putchar('%');
                        putchar(fmt[i]);
                    }
                } else {
                    if (fmt[i] == '\\' && fmt[i+1] == 'n') {
                        putchar('\n');
                        i++;
                    } else {
                        putchar(fmt[i]);
                    }
                }
            }
            return resultado;
        }
        case NO_FUNC_DECL: {
            Valor v = {0};
            v.tipo = TIPO_FUNC;
            v.dado.func_ast = no;
            inserirSimbolo(no->nome, TIPO_FUNC, v, tabela);
            return v;
        }
        case NO_FUNC_CALL: {
            Celula *c = buscarSimboloInterpretador(no->nome, tabela);
            if (!c || c->tipo != TIPO_FUNC) {
                fprintf(stderr, "[Runtime] Erro: '%s' nao e uma funcao\n", no->nome);
                exit(1);
            }
            No *func_ast = c->valor.dado.func_ast;
            Celula **tabela_local = criarTabelaSimbolos();
            
            No *params = func_ast->u.func_decl.params;
            No *args = no->u.call.args;
            int param_count = params ? params->u.bloco.count : 0;
            int arg_count = args ? args->u.bloco.count : 0;
            
            if (param_count != arg_count) {
                fprintf(stderr, "[Runtime] Erro: '%s' espera %d argumentos, recebeu %d\n", no->nome, param_count, arg_count);
                exit(1);
            }
            
            if (strcmp(no->nome, "sqrt") == 0 || strcmp(no->nome, "abs") == 0 ||
                strcmp(no->nome, "floor") == 0 || strcmp(no->nome, "ceil") == 0 ||
                strcmp(no->nome, "round") == 0) {
                Valor arg_val = avaliar(args->u.bloco.stmts[0], tabela);
                float val = (arg_val.tipo == TIPO_FLOAT) ? arg_val.dado.f : (float)arg_val.dado.i;
                Valor res = {0};
                res.tipo = TIPO_FLOAT;
                
                if (strcmp(no->nome, "sqrt") == 0) res.dado.f = math_sqrt(val);
                else if (strcmp(no->nome, "abs") == 0) res.dado.f = math_abs(val);
                else if (strcmp(no->nome, "floor") == 0) res.dado.f = math_floor(val);
                else if (strcmp(no->nome, "ceil") == 0) res.dado.f = math_ceil(val);
                else if (strcmp(no->nome, "round") == 0) res.dado.f = math_round(val);
                
                return res;
            }
            
            if (strcmp(no->nome, "pow") == 0) {
                Valor arg_base = avaliar(args->u.bloco.stmts[0], tabela);
                Valor arg_exp = avaliar(args->u.bloco.stmts[1], tabela);
                float b = (arg_base.tipo == TIPO_FLOAT) ? arg_base.dado.f : (float)arg_base.dado.i;
                float e = (arg_exp.tipo == TIPO_FLOAT) ? arg_exp.dado.f : (float)arg_exp.dado.i;
                Valor res = {0};
                res.tipo = TIPO_FLOAT;
                res.dado.f = math_pow(b, e);
                return res;
            }
            
            if (strcmp(no->nome, "atoi") == 0) {
                Valor arg = avaliar(args->u.bloco.stmts[0], tabela);
                if (arg.tipo != TIPO_STR) {
                    fprintf(stderr, "[Runtime] Erro: atoi espera string\n");
                    liberarTabelaSimbolos(tabela_local);
                    exit(1);
                }
                Valor res = {0};
                res.tipo = TIPO_INT;
                res.dado.i = stdlib_atoi(arg.dado.s);
                liberarTabelaSimbolos(tabela_local);
                return res;
            }

            if (strcmp(no->nome, "atof") == 0) {
                Valor arg = avaliar(args->u.bloco.stmts[0], tabela);
                if (arg.tipo != TIPO_STR) {
                    fprintf(stderr, "[Runtime] Erro: atof espera string\n");
                    liberarTabelaSimbolos(tabela_local);
                    exit(1);
                }
                Valor res = {0};
                res.tipo = TIPO_FLOAT;
                res.dado.f = stdlib_atof(arg.dado.s);
                liberarTabelaSimbolos(tabela_local);
                return res;
            }

            if (strcmp(no->nome, "exit") == 0) {
                Valor arg = avaliar(args->u.bloco.stmts[0], tabela);
                int code = (arg.tipo == TIPO_FLOAT) ? (int)arg.dado.f : arg.dado.i;
                liberarTabelaSimbolos(tabela_local);
                stdlib_exit(code);
            }

            for (int i = 0; i < param_count; i++) {
                Valor arg_val = avaliar(args->u.bloco.stmts[i], tabela);
                No *param_decl = params->u.bloco.stmts[i];
                arg_val = coercionar(arg_val, param_decl->tipoDeclarado);
                inserirSimbolo(param_decl->nome, param_decl->tipoDeclarado, arg_val, tabela_local);
            }

            int old_has_returned = has_returned;
            Valor old_return_val = return_val;
            has_returned = 0;
            
            avaliar(func_ast->u.func_decl.body, tabela_local);
            
            Valor res = has_returned ? return_val : (Valor){.tipo = func_ast->tipoDeclarado};
            
            has_returned = old_has_returned;
            return_val = old_return_val;
            
            liberarTabelaSimbolos(tabela_local);
            return coercionar(res, func_ast->tipoDeclarado);
        }
        default:
            fprintf(stderr, "[Runtime] Erro: no desconhecido tipo=%d\n", no->tipo);
            exit(1);
    }
}