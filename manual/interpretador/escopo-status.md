# Status do escopo: documentado vs implementado vs testado

> Comparação item a item entre o [escopo alvo](escopo.md), o código atual (`lexer/`, `parser/`, `lib/`) e a suíte em `testes/`.  
> Legenda: **Doc** = previsto em `escopo.md` · **Impl** = presente no interpretador · **Teste** = casos em `testes/` (via `make test`).

| Símbolo | Significado |
|---------|-------------|
| ✅ | Coberto por completo |
| ⚠️ | Parcial (lexer/parser sem runtime, ou só testes indiretos) |
| ❌ | Ausente |
| ➖ | Não consta no escopo documentado, mas existe no projeto |

---

## Resumo executivo

| Métrica | Valor |
|---------|-------|
| Itens no escopo documentado | 38 agrupamentos |
| Implementados (total ou parcial) | 28 |
| Com suíte dedicada em `testes/` | 24 |
| Pastas de feature no `Makefile` | 12 (`FEATURES`) |
| Arquivos `.txt` de teste | **361** |

**Principais lacunas:** `const`, arrays, ponteiros, `do while`, `continue`, `scanf`, `rand`/`srand`, `string.h`, `ctype.h`, protótipos de função e tipo `char[]`/`char*` (em vez disso usa-se `string`).

---

## Tipos e variáveis

| Recurso | Doc | Impl | Teste | Observações |
|---------|:---:|:----:|:-----:|-------------|
| `int` | ✅ | ✅ | ✅ | `testes/declaracao_variaveis/` |
| `float` | ✅ | ✅ | ✅ | idem |
| `double` | ✅ | ⚠️ | ❌ | Token/gramática; sem testes dedicados |
| `char` | ✅ | ✅ | ✅ | `valido_char_init.txt` |
| `bool` | ✅ | ⚠️ | ❌ | Declarável; sem testes específicos |
| `void` | ➖ | ✅ | ✅ | Retorno de funções; `testes/recursao/` |
| `string` (extensão) | ❌* | ✅ | ✅ | *Doc prevê `char[]`/`char*`, não `string` |
| Declaração simples | ✅ | ✅ | ✅ | |
| Declaração com init | ✅ | ✅ | ✅ | |
| Declarações múltiplas (`int a, b;`) | ✅ | ✅ | ✅ | `valido_decl_composta.txt` |
| Modificador `const` | ✅ | ❌ | ❌ | |
| Arrays unidimensionais | ✅ | ❌ | ❌ | `[`/`]` no lexer, não na gramática |
| Strings como `char[]` / `char*` | ✅ | ❌ | ❌ | Substituído por tipo `string` |
| Escopo local / sombreamento | ✅ | ✅ | ✅ | `testes/recursao/` (`valido_sombreamento.txt`) |
| Redeclaração (erro) | ➖ | ✅ | ✅ | Vários módulos semânticos |
| Comentários `//` | ➖ | ✅ | ⚠️ | Impl; testes indiretos |

---

## Operadores

| Recurso | Doc | Impl | Teste | Observações |
|---------|:---:|:----:|:-----:|-------------|
| `+` `-` `*` `/` `%` | ✅ | ✅ | ✅ | `testes/operadores_aritmeticos/` |
| Menos unário | ➖ | ✅ | ✅ | `valido_unary_minus.txt` |
| `==` `!=` `<` `>` `<=` `>=` | ✅ | ✅ | ✅ | `operadores_logicos` + condicionais |
| `&&` `\|\|` `!` | ✅ | ✅ | ✅ | `testes/operadores_logicos/` |
| Curto-circuito (`&&`, `\|\|`) | ➖ | ✅ | ✅ | `valido_and_curto_circuito.txt`, `valido_or_curto_circuito.txt` |
| `=` | ✅ | ✅ | ✅ | `declaracao_variaveis`, `atualizacao_variaveis` |
| `+=` `-=` `*=` `/=` | ✅ | ✅ | ✅ | `testes/atualizacao_variaveis/` |
| `++` `--` | ✅ | ✅ | ✅ | idem + `testes/otimizador/` |

---

## Controle de fluxo

| Recurso | Doc | Impl | Teste | Observações |
|---------|:---:|:----:|:-----:|-------------|
| `if` / `else` | ✅ | ✅ | ✅ | `testes/if_else/` (34 casos) |
| `else if` (cadeia) | ✅ | ✅ | ✅ | Gramática recursiva em `parser.y` |
| `while` | ✅ | ✅ | ✅ | `testes/while/` (35 casos) |
| `do while` | ✅ | ❌ | ❌ | |
| `for` | ✅ | ✅ | ✅ | `testes/for/` (34 casos) |
| `switch` / `case` / `default` | ✅ | ✅ | ✅ | `testes/switch_case/` (31 casos) |
| Fall-through no `switch` | ➖ | ✅ | ✅ | `valido_fall-through.txt` |
| `case` com `char` | ➖ | ✅ | ✅ | `valido_switch_char*.txt` |
| `break` | ✅ | ✅ | ✅ | `while`, `for`, `switch_case` |
| `continue` | ✅ | ❌ | ❌ | |
| `return` | ✅ | ✅ | ✅ | `testes/recursao/` |

---

## Funções

| Recurso | Doc | Impl | Teste | Observações |
|---------|:---:|:----:|:-----:|-------------|
| Definição com corpo | ✅ | ✅ | ✅ | `testes/recursao/` |
| Chamada | ✅ | ✅ | ✅ | idem |
| Parâmetros | ✅ | ✅ | ✅ | `valido_func_multi_params.txt` |
| Recursão | ✅ | ✅ | ✅ | fatorial, fibonacci, MDC |
| Escopo de parâmetros/locais | ✅ | ✅ | ✅ | `valido_sombreamento_param.txt` |
| Protótipo (declaração sem corpo) | ➖ | ❌ | ❌ | Apenas `tipo nome(params) { ... }` |
| Passagem por valor | ✅ | ✅ | ✅ | Sem ponteiros/referência |
| `main` obrigatório | ➖ | ❌ | ➖ | Programa = sequência de `stmt` no topo |

---

## Saída e bibliotecas

### `stdio.h`

| Recurso | Doc | Impl | Teste | Observações |
|---------|:---:|:----:|:-----:|-------------|
| `printf` | ✅ | ✅ | ⚠️ | Palavra-chave do lexer; `%d` `%i` `%f` `%s` `%c`, `\n`; sem pasta própria |
| `scanf` | ✅ | ❌ | ❌ | |

### `math.h` (requer `#include <math.h>`)

| Recurso | Doc | Impl | Teste | Observações |
|---------|:---:|:----:|:-----:|-------------|
| `sqrt` | ✅ | ✅ | ✅ | `testes/math/` |
| `pow` | ✅ | ✅ | ✅ | idem |
| `abs` | ✅ | ✅ | ✅ | idem |
| `floor` | ✅ | ✅ | ✅ | `valido_floor_ceil.txt` |
| `ceil` | ✅ | ✅ | ✅ | idem |
| `round` | ✅ | ✅ | ✅ | `valido_round_literal.txt` |

### `stdlib.h` (requer `#include <stdlib.h>`)

| Recurso | Doc | Impl | Teste | Observações |
|---------|:---:|:----:|:-----:|-------------|
| `atoi` | ✅ | ✅ | ✅ | `testes/stdlib/` |
| `atof` | ✅ | ✅ | ✅ | idem |
| `exit` | ✅ | ✅ | ✅ | `.exitcode` em `valido_stdlib_exit_*.txt` |
| `abs` (stdlib) | ✅ | ⚠️ | ⚠️ | Disponível via `math.h` (`abs`); não via stdlib |
| `rand` | ✅ | ❌ | ❌ | Rejeitado em `invalido_stdlib_funcao_nao_suportada.txt` |
| `srand` | ✅ | ❌ | ❌ | |

### `string.h`

| Recurso | Doc | Impl | Teste |
|---------|:---:|:----:|:-----:|
| `strlen` `strcpy` `strcmp` `strcat` `strchr` | ✅ | ❌ | ❌ |

### `ctype.h`

| Recurso | Doc | Impl | Teste |
|---------|:---:|:----:|:-----:|
| `isalpha` `isdigit` `isspace` | ✅ | ❌ | ❌ |

---

## Pré-processador e pipeline

| Recurso | Doc | Impl | Teste | Observações |
|---------|:---:|:----:|:-----:|-------------|
| `#include <math.h>` | ➖ | ✅ | ✅ | Diretiva literal no lexer |
| `#include <stdlib.h>` | ➖ | ✅ | ✅ | idem |
| `#include` genérico / `stdio.h` | ➖ | ❌ | ❌ | |
| Análise léxica | ➖ | ✅ | ⚠️ | Coberta indiretamente por todos os testes sintáticos |
| Análise sintática | ➖ | ✅ | ✅ | Pastas `sintatico/` |
| Análise semântica | ➖ | ✅ | ✅ | Pastas `semantico/` |
| Otimização (constant folding, DCE) | ➖ | ✅ | ✅ | `testes/otimizador/` (38 casos) |
| Execução / interpretação | ➖ | ✅ | ✅ | Pastas `execucao/` |

---

## Cobertura de testes por feature

Contagem de arquivos `.txt` por pasta (jun/2026):

| Feature | Sint. ✅ | Sint. ❌ | Sem. ✅ | Sem. ❌ | Exec. ✅ | Exec. ❌ | **Total** |
|---------|----------|----------|---------|---------|----------|----------|-----------|
| `atualizacao_variaveis` | 4 | 3 | 3 | 3 | 10 | 5 | **28** |
| `declaracao_variaveis` | 4 | 3 | 5 | 5 | 10 | 5 | **32** |
| `for` | 5 | 5 | 5 | 4 | 10 | 5 | **34** |
| `if_else` | 6 | 5 | 3 | 5 | 10 | 5 | **34** |
| `while` | 5 | 5 | 5 | 5 | 10 | 5 | **35** |
| `operadores_aritmeticos` | 3 | 4 | 2 | 3 | 10 | 5 | **27** |
| `operadores_logicos` | 5 | 4 | 5 | 4 | 10 | 5 | **33** |
| `math` | 1 | 5 | 1 | 2 | 6 | 2 | **17** |
| `stdlib` | 1 | 5 | 1 | 2 | 8 | 2 | **19** |
| `switch_case` | 6 | 6 | 2 | 3 | 9 | 5 | **31** |
| `recursao` | 5 | 5 | 5 | 4 | 9 | 5 | **33** |
| `otimizador` | 5 | 5 | 5 | 5 | 13 | 5 | **38** |
| **Soma** | **50** | **49** | **42** | **42** | **115** | **54** | **361** |

Cada feature segue quatro camadas: léxico/sintaxe → semântica → execução, com casos válidos e inválidos (exceto otimizador, que reutiliza a mesma estrutura).

---

## Histórico de versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 21/06/26 | Criação da matriz documentado/implementado/testado | Cursor Agent |
