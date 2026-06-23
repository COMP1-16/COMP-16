
> Este documento descreve as estruturas escolhidas para implementação e suas especificações técnicas. Ele é parte da seção de **Linguagem e Escopo** da documentação do interpretador, que cobre o que é a linguagem C e o que o projeto se propõe a implementar.

# Gramática e Tokens do Interpretador

> Referência técnica das estruturas recuperadas pelo `lexer.l`, arranjadas pelo `parser.y` e interpretadas pelo `interpreter.c`. 

---

## 1. Tabela de Tokens (lexer.l)

<a id="lit"></a>
<a id="id"></a>
<a id="aritimeticos"></a>
<a id="relacionais"></a>
<a id="logicos"></a>

| Categoria | Token | Padrão / Exemplo |
|---|---|---|
| Includes | `INCLUDE_MATH`, `INCLUDE_STDLIB` | `#include <math.h>`, `#include <stdlib.h>` |
| Condicionais | `KW_IF`, `KW_ELSE`, `KW_WHILE`, `KW_FOR` | `if`, `else`, `while`, `for` |
| Switch | `KW_SWITCH`, `KW_CASE`, `KW_DEFAULT` | `switch`, `case`, `default` |
| Controle de fluxo | `KW_RETURN`, `KW_BREAK` | `return`, `break` |
| Saída | `KW_PRINTF` | `printf` |
| Tipos | `TYPE_INT`, `TYPE_FLOAT`, `TYPE_DOUBLE`, `TYPE_CHAR`, `TYPE_BOOL`, `TYPE_VOID`, `TYPE_STRING` | `int`, `float`, `double`, `char`, `bool`, `void`, `string` |
| Literais | `NUM`, `FLOAT_NUM`, `STRING`, `CHAR_LIT` | `42`, `3.14`, `"texto"`, `'A'` |
| Identificador | `ID` | `variavel` |
| Relacionais | `EQUAL`, `DIFF`, `LESS_EQ`, `GREAT_EQ`, `LESSER`, `GREATER` | `==`, `!=`, `<=`, `>=`, `<`, `>` |
| Atualização | `UPDT_PLUS`, `UPDT_MINUS`, `UPDT_TIMES`, `UPDT_DIVIDE`, `UPDT_INC`, `UPDT_DEC` | `+=`, `-=`, `*=`, `/=`, `++`, `--` |
| Atribuição | `INITVAR` | `=` |
| Aritméticos | `PLUS`, `MINUS`, `TIMES`, `DIVIDE`, `MOD` | `+`, `-`, `*`, `/`, `%` |
| Lógicos | `AND`, `OR`, `NOT` | `&&`, `\|\|`, `!` |
| Delimitadores | `L_PAREN`, `R_PAREN`, `L_CRLRBRACKET`, `R_CRLRBRACKET`, `COMMA`, `SEMICOLON`, `COLON` | `( ) { } , ; :` |

### Casos especiais (sem retorno de token)
- `[ \t\r\n]+` → ignora espaço em branco
- `"//"[^\n]*` → ignora comentário de linha
- `.` (qualquer outro caractere) → erro léxico reportado em `stderr`

---

## 2. Gramática

> Baseada nas produções de `parser.y`. Convenções: `?` = opcional, `*` = zero ou mais repetições, `+` = uma ou mais repetições.

### 2.1 Programa e blocos
<a id="stmt"></a>

| Estrutura | Descrição | O que agrega |
| --- | --- | --- |
| **programa** | raiz do programa | contém uma lista de declarações e funções |
| **bloco** | bloco de instruções delimitado por chaves ou ser um único comando | contém uma lista de comandos (stmts) |
| **stmts** | lista de comandos, podendo ser vazia | contém uma lista de comandos ([stmt](#stmt)) |


### 2.2 Statement (comando)

**stmt**:

- Declaração de variável ou função;
- Atualização de variável;
- Expressão seguida de ponto e vírgula;
- Comando de controle de fluxo (if, while, for, switch);
- Bloco de comandos;
- Comando de saída (printf);
- Inclusão de biblioteca (math.h, stdlib.h);
- Comando de retorno (return);
- Comando de quebra (break);
- Comando vazio (apenas ';');


### 2.3 Declarações e funções

| Estrutura | Descrição | Expande para... |
| --- | --- | --- |
| **tipo** | tipo de dado de variável/função | (1) `'int'`<br>(2)`'float'`<br>(3)`'double'`<br>(4)`'char'`<br>(5)`'bool'`<br>(6)`'void'`<br>(7)`'string'` |
| **declarador** | declarador de variável com ou sem atribuição inicial | (1)`ID '=' expr`<br>(2)`ID` |
| **declaradores** | lista de declaradores, separados por vírgula | `declarador` (`',' declarador`)* |
| **declaracao** | declaração de variável | (1) `tipo declarador;`<br>(2) `tipo declaradores;` |
| **funcao_decl** | declaração de função | `tipo ID '(' **param_list_opt** ')' bloco` |
| **param_list_opt** | lista de parâmetros opcional | (1) `{ }`<br>(2) `param_list` |
| **param_list** | lista de parâmetros | `param` (`',' param`)* |
| **param** | parâmetro de função | `tipo ID` |


### 2.4 Atualização de variáveis

| Estrutura | Descrição | Expande para... |
| --- | --- | --- |
| atualizacao | (1) atualização de variável<br>(2) atualização de variável com acréssimo<br>(3) atualização de variável com decremento<br>(4) atulização de variável com produto<br>(5) atualização de variável com quociente<br>(6) incremento pós-fixado<br>(7) decremento pós-fixado | (1) `ID '=' expr`<br>(2) `ID '+=' expr`<br>(3) `ID '-=' expr`<br>(4) `ID '*=' expr`<br>(5) `ID '/=' expr`<br>(6) `ID '++'`<br>(7) `ID '--'` |

### 2.5 Expressões

| Estrutura | Descrição | Expande para... |
| --- | --- | --- |
| **expr** | expressão | (1) [Literais](#lit)<br>(2) [Identificadores](#id)<br>(3) `ID '('arg_list_opt')'` <br>(4) `expr` [Aritméticos](#aritimeticos) `expr`<br>(5) `expr` [Relacionais](#relacionais) `expr`<br>(6) `expr` [Lógicos](#logicos) `expr`<br>(7) `'!' expr`<br>(8) `'-' expr`<br>(9) `ID'(' expr ')'`<br>(10) `'(' expr ')'`<br>(11) `ID'++'`<br>(12) `ID'--'`|
| **arg_list_opt** | lista de argumentos opcional | (1) `{ }`<br>(2) `arg_list` |
| **arg_list** | lista de argumentos | `expr` (`',' expr`)* |

### 2.6 Condicional
| Estrutura | Descrição | Expande para... |
| --- | --- | --- |
| **if_stmt** | comando condicional | (1) `'if' '(' expr ')' stmt`<br>(2) `'if' '(' expr ')' stmt 'else' stmt`<br>(3) `'if' '(' expr ')' stmt 'else' if_stmt` |

### 2.7 Switch / case
| Estrutura | Descrição | Expande para... |
| --- | --- | --- |
| **switch_stmt** | comando switch | `'switch' '(' expr ')' '{' case_stmt+ '}'` |
| **case_stmt** | comando case | (1) `'case' NUM ':' stmts`?<br>(2) `'case' CHAR_LIT ':' stmts`?<br>(3) `'default' ':' stmts`? |

> Observação: não há `break` obrigatório aqui por se expandir em `stmts`.

### 2.8 While

| Estrutura | Descrição | Expande para... |
| --- | --- | --- |
| **while_stmt** | comando while | `'while' '(' expr ')' stmt` |

### 2.9 For

| Estrutura | Descrição | Expande para... |
| --- | --- | --- |
| **for_stmt** | comando for | `'for' '(' for_init ';' for_cond ';' for_inc ')' stmt` |
| **for_init** | inicialização do for | (1) `tipo declarador (',' declarador)*`<br>(2) `ID '=' expr`<br>(3) `expr`<br>(4) /* vazio */ |
| **for_cond** | condição do for | (1) `expr`<br>(2) /* vazio */ |
| **for_inc** | incremento do for | (1) `expr`<br>(2) `ID '=' expr`<br>(3) `ID '++'`<br>(4) `ID '--'`<br>(5) /* vazio */ |

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 18/04/26 | Criação da página inicial | Pedro Henrique |
| 1.1 | 13/05/26 | Revisão do conteúdo e da estética da página | Camila Careli |
| 1.2 | 23/06/26 | Revisão de seção de acordo com os recursos implementados | Vinícius de Jesus |