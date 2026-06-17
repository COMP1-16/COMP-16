#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantico.h"

static int erros = 0;
static Celula **tabela_global = NULL;

static Celula *buscarSimboloSemantico(char *nome, Celula **tabela) {
    Celula *c = buscarSimbolo(nome, tabela);
    if (!c && tabela_global && tabela != tabela_global) {
        c = buscarSimbolo(nome, tabela_global);
    }
    return c;
}

/* ------------------------------------------------------------------ *
 * Helpers de relatório                                                *
 * ------------------------------------------------------------------ */
static void erroSem(const char *msg) {
    fprintf(stderr, "[Semantico] Erro: %s\n", msg);
    erros++;
}

static void avisoSem(const char *msg) {
    fprintf(stderr, "[Semantico] Aviso: %s\n", msg);
}

/* ------------------------------------------------------------------ *
 * Resolução de tipo de uma expressão                                  *
 *                                                                     *
 * Percorre o nó de expressão e retorna o tipo resultante (TIPO_*).   *
 * Não executa — apenas infere o tipo estaticamente.                  *
 * ------------------------------------------------------------------ */
static int resolverTipo(No *no, Celula **tabela) {
    if (!no) return TIPO_ERRO;
    switch (no->tipo) {
        case NO_INT:   return TIPO_INT;
        case NO_FLOAT: return TIPO_FLOAT;
        case NO_CHAR:  return TIPO_CHAR;
        case NO_STR:   return TIPO_STR;
        case NO_ID: {
            Celula *c = buscarSimboloSemantico(no->nome, tabela);
            if (!c) {
                char msg[128];
                snprintf(msg, sizeof(msg), "variavel '%s' nao declarada", no->nome);
                erroSem(msg);
                return TIPO_ERRO;
            }
            return c->tipo;
        }
        case NO_BINOP: {
            int te = resolverTipo(no->u.bin.esq, tabela);
            int td = resolverTipo(no->u.bin.dir, tabela);
            if (te == TIPO_ERRO || td == TIPO_ERRO) return TIPO_ERRO;
            if (te == TIPO_FLOAT || td == TIPO_FLOAT) return TIPO_FLOAT;
            return TIPO_INT;
        }
        case NO_RELACIONAL: {
            int te = resolverTipo(no->u.bin.esq, tabela);
            int td = resolverTipo(no->u.bin.dir, tabela);
            if (te == TIPO_ERRO || td == TIPO_ERRO) return TIPO_ERRO;
            if ((te == TIPO_STR && td != TIPO_STR) || (td == TIPO_STR && te != TIPO_STR)) {
                erroSem("comparacao invalida");
                return TIPO_ERRO;
            }
            return TIPO_BOOL;
        }
        case NO_LOGICAL_AND:
        case NO_LOGICAL_OR: {
            int te = resolverTipo(no->u.bin.esq, tabela);
            int td = resolverTipo(no->u.bin.dir, tabela);
            if ((te != TIPO_ERRO && te != TIPO_BOOL && te != TIPO_INT) ||
                (td != TIPO_ERRO && td != TIPO_BOOL && td != TIPO_INT)) {
                erroSem("operadores logicos exigem operandos escalares");
            }
            return TIPO_BOOL;
        }
        case NO_NOT: {
            int te = resolverTipo(no->u.not.expr, tabela);
            if (te != TIPO_ERRO && te != TIPO_BOOL && te != TIPO_INT) {
                erroSem("operador ! exige operando escalar");
            }
            return TIPO_BOOL;
        }
        case NO_FUNC_CALL: {
            Celula *c = buscarSimboloSemantico(no->nome, tabela);
            if (!c) {
                char msg[128];
                snprintf(msg, sizeof(msg), "funcao '%s' nao declarada", no->nome);
                erroSem(msg);
                return TIPO_ERRO;
            }
            if (c->tipo != TIPO_FUNC) {
                char msg[128];
                snprintf(msg, sizeof(msg), "'%s' nao e uma funcao", no->nome);
                erroSem(msg);
                return TIPO_ERRO;
            }
            No *func_ast = c->valor.dado.func_ast;
            No *params = func_ast->u.func_decl.params;
            No *args = no->u.call.args;
            int param_count = params ? params->u.bloco.count : 0;
            int arg_count = args ? args->u.bloco.count : 0;
            if (param_count != arg_count) {
                char msg[128];
                snprintf(msg, sizeof(msg), "funcao '%s' espera %d argumentos, recebeu %d", no->nome, param_count, arg_count);
                erroSem(msg);
                return TIPO_ERRO;
            }
            for (int i = 0; i < param_count; i++) {
                int tipoArg = resolverTipo(args->u.bloco.stmts[i], tabela);
                int tipoParam = params->u.bloco.stmts[i]->tipoDeclarado;
                if (tipoArg != TIPO_ERRO && tipoArg != tipoParam) {
                    if (!((tipoParam == TIPO_FLOAT && tipoArg == TIPO_INT) || (tipoParam == TIPO_INT && tipoArg == TIPO_FLOAT))) {
                        char msg[128];
                        snprintf(msg, sizeof(msg), "tipo incompativel para argumento %d da funcao '%s'", i + 1, no->nome);
                        erroSem(msg);
                    }
                }
            }
            return func_ast->tipoDeclarado;
        }
        default:
            return TIPO_ERRO;
    }
}

static int verificarAtribuicao(int tipoAlvo, int tipoExpr, const char *nome) {
    if (tipoAlvo == tipoExpr) return tipoAlvo;
    if (tipoAlvo == TIPO_FLOAT && tipoExpr == TIPO_INT) return TIPO_FLOAT;
    if (tipoAlvo == TIPO_INT && tipoExpr == TIPO_FLOAT) {
        char msg[128];
        snprintf(msg, sizeof(msg), "atribuicao de float para int em '%s' (perda de precisao)", nome);
        avisoSem(msg);
        return TIPO_INT;
    }
    char msg[128];
    snprintf(msg, sizeof(msg), "tipo incompativel na atribuicao de '%s'", nome);
    erroSem(msg);
    return TIPO_ERRO;
}

static void checarNo(No *no, Celula **tabela) {
    if (!no) return;

    switch (no->tipo) {
        case NO_BLOCO:
            for (int i = 0; i < no->u.bloco.count; i++)
                checarNo(no->u.bloco.stmts[i], tabela);
            break;
        case NO_DECL: {
            if (buscarSimbolo(no->nome, tabela)) {
                char msg[128];
                snprintf(msg, sizeof(msg), "redeclaracao de '%s'", no->nome);
                erroSem(msg);
                break;
            }
            Valor vazio = {0};
            vazio.tipo = no->tipoDeclarado;
            inserirSimbolo(no->nome, no->tipoDeclarado, vazio, tabela);
            break;
        }
        case NO_DECL_INIT: {
            if (buscarSimbolo(no->nome, tabela)) {
                char msg[128];
                snprintf(msg, sizeof(msg), "redeclaracao de '%s'", no->nome);
                erroSem(msg);
                break;
            }
            int tipoExpr = resolverTipo(no->u.bin.esq, tabela);
            if (tipoExpr != TIPO_ERRO)
                verificarAtribuicao(no->tipoDeclarado, tipoExpr, no->nome);
            Valor vazio = {0};
            vazio.tipo = no->tipoDeclarado;
            inserirSimbolo(no->nome, no->tipoDeclarado, vazio, tabela);
            break;
        }
        case NO_ATRIB: {
            Celula *c = buscarSimboloSemantico(no->nome, tabela);
            if (!c) {
                char msg[128];
                snprintf(msg, sizeof(msg), "variavel '%s' nao declarada", no->nome);
                erroSem(msg);
                break;
            }
            int tipoExpr = resolverTipo(no->u.bin.esq, tabela);
            if (tipoExpr != TIPO_ERRO)
                verificarAtribuicao(c->tipo, tipoExpr, no->nome);
            break;
        }
        case NO_ATRIB_OP:
        case NO_INC:
        case NO_DEC: {
            Celula *c = buscarSimboloSemantico(no->nome, tabela);
            if (!c) {
                char msg[128];
                snprintf(msg, sizeof(msg), "variavel '%s' nao declarada", no->nome);
                erroSem(msg);
                break;
            }
            if (no->tipo == NO_ATRIB_OP && no->u.bin.esq) {
                int tipoExpr = resolverTipo(no->u.bin.esq, tabela);
                if (tipoExpr != TIPO_ERRO)
                    verificarAtribuicao(c->tipo, tipoExpr, no->nome);
            }
            break;
        }
        case NO_IF: {
            int tipoCond = resolverTipo(no->u.if_stmt.cond, tabela);
            if (tipoCond != TIPO_BOOL && tipoCond != TIPO_INT && tipoCond != TIPO_ERRO) {
                erroSem("condicao do if invalida");
            }
            checarNo(no->u.if_stmt.thenBranch, tabela);
            if (no->u.if_stmt.elseBranch)
                checarNo(no->u.if_stmt.elseBranch, tabela);
            break;
        }
        case NO_ID: {
            if(!buscarSimboloSemantico(no->nome, tabela)){
                erroSem("variavel não declarada\n");
            }
            break;
        }
        case NO_SWITCH: {
            #define MAX_CASES 256
            checarNo(no->u.switch_stmt.value, tabela);
            int visto[MAX_CASES] = {0};
            int vistoCount = 0;
            int intToVerify;
            int count = no->u.switch_stmt.cases->u.bloco.count;

            int defaultCount = 0;

            for(int i = 0; i < count; i++ ){
                if(defaultCount >= 1){
                    erroSem("mais de um bloco default em switch_stmt");
                    break;
                }
                if(vistoCount >= MAX_CASES){
                    erroSem("Erro: numero de cases excede o limite.");
                }
                switch (no->u.switch_stmt.cases->u.bloco.stmts[i]->tipo) {
                    case NO_DEFAULT:
                        defaultCount++;
                        checarNo(no->u.switch_stmt.cases->u.bloco.stmts[i]->u.case_default.stmts, tabela);
                        break;
                    case NO_CASE_INT:
                        intToVerify = no->u.switch_stmt.cases->u.bloco.stmts[i]->u.case_int.value->ival;
                        for(int j = 0; j < vistoCount; j++){
                            if(visto[j] == intToVerify){
                                char msg[35];
                                snprintf(msg, sizeof(msg),"Case nº %d se repetiu", i+1);
                                erroSem(msg);
                                break;
                            }
                        }
                        visto[vistoCount++] = intToVerify;
                        checarNo(no->u.switch_stmt.cases->u.bloco.stmts[i]->u.case_int.stmts, tabela);
                        break;
                    case NO_CASE_CHAR:
                        intToVerify = (int) no->u.switch_stmt.cases->u.bloco.stmts[i]->u.case_char.value->cval;
                        for(int j = 0; j < vistoCount; j++){
                            if(visto[j] == intToVerify){
                                char msg[35];
                                snprintf(msg, sizeof(msg),"Case nº %d se repetiu", i);
                                erroSem(msg);
                                break;
                            }
                        }
                        visto[vistoCount++] = intToVerify;
                        checarNo(no->u.switch_stmt.cases->u.bloco.stmts[i]->u.case_char.stmts, tabela);
                        break;
                }
            }
            break;
        }
        case NO_WHILE: {
            int tipoCond = resolverTipo(no->u.while_stmt.cond, tabela);
            if (tipoCond != TIPO_BOOL && tipoCond != TIPO_INT && tipoCond != TIPO_ERRO) {
                erroSem("condicao do while deve ser booleana ou inteira");
            }
            checarNo(no->u.while_stmt.body, tabela);
            break;
        }
        case NO_FOR: {
            if (no->u.for_stmt.init) checarNo(no->u.for_stmt.init, tabela);
            if (no->u.for_stmt.cond) {
                int tipoCond = resolverTipo(no->u.for_stmt.cond, tabela);
                if (tipoCond != TIPO_BOOL && tipoCond != TIPO_INT && tipoCond != TIPO_ERRO)
                    erroSem("condicao do for deve ser booleana ou inteira");
            }
            if (no->u.for_stmt.inc) resolverTipo(no->u.for_stmt.inc, tabela);
            checarNo(no->u.for_stmt.body, tabela);
            break;
        }
        case NO_RETURN:
            resolverTipo(no->u.bin.esq, tabela);
            break;
        case NO_PRINTF:
            if (no->u.call.args) {
                for (int i = 0; i < no->u.call.args->u.bloco.count; i++) {
                    resolverTipo(no->u.call.args->u.bloco.stmts[i], tabela);
                }
            }
            break;
        case NO_FUNC_DECL: {
            if (buscarSimbolo(no->nome, tabela)) {
                char msg[128];
                snprintf(msg, sizeof(msg), "redeclaracao de '%s'", no->nome);
                erroSem(msg);
                break;
            }
            Valor v = {0};
            v.tipo = TIPO_FUNC;
            v.dado.func_ast = no;
            inserirSimbolo(no->nome, TIPO_FUNC, v, tabela);

            Celula **tabela_local = criarTabelaSimbolos();
            if (no->u.func_decl.params) {
                for (int i = 0; i < no->u.func_decl.params->u.bloco.count; i++) {
                    No *param = no->u.func_decl.params->u.bloco.stmts[i];
                    Valor vazio = {0};
                    vazio.tipo = param->tipoDeclarado;
                    inserirSimbolo(param->nome, param->tipoDeclarado, vazio, tabela_local);
                }
            }

            if (no->u.func_decl.body) {
                checarNo(no->u.func_decl.body, tabela_local);
            }

            liberarTabelaSimbolos(tabela_local);
            break;
        }
        case NO_FUNC_CALL: {
            resolverTipo(no, tabela);
            if (no->u.call.args) {
                for (int i = 0; i < no->u.call.args->u.bloco.count; i++) {
                    resolverTipo(no->u.call.args->u.bloco.stmts[i], tabela);
                }
            }
            break;
        }
        default:
            resolverTipo(no, tabela);
            break;
    }
}

int checar(No *raiz, Celula **tabela) {
    erros = 0;
    tabela_global = tabela;
    checarNo(raiz, tabela);
    return erros;
}