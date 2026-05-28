# Casos de teste por fase de análise

> Este documento organiza casos de teste para o interpretador, alinhados ao escopo definido em [Escopo do interpretador](escopo.md) e ao encadeamento natural do projeto: análise léxica, sintática, semântica e execução (interpretação).

## Perguntas que os testes respondem

| Sprint | Fase principal | Pergunta que os testes respondem |
|--------|----------------|----------------------------------|
| 1 | Léxica | A sequência de caracteres gera apenas tokens válidos para o subconjunto da linguagem? |
| 2 | Sintática | Os tokens respeitam a gramática (estrutura do programa, declarações, expressões, comandos)? |
| 3 | Semântica | O programa é coerente (tipos, escopos, assinaturas, regras de uso de `const`, arrays, funções)? |
| 4 | Execução | O interpretador produz o comportamento esperado em tempo de execução (E/S, fluxo, biblioteca, retornos)? |

Na raiz do repositório, a suíte automatizada atual (`make test` / `tests.py`) cobre principalmente a detecção de erro sintático (presença ou ausência da mensagem `Erro sintático` em `stderr`). Os blocos abaixo ampliam a matriz de casos para todas as fases, servindo de checklist para implementação e para testes manuais ou futuros scripts.

---

## Sprint 1 — Análise léxica

Objetivo: validar reconhecimento de tokens previstos no escopo: palavras-chave e tipos (`int`, `float`, `double`, `char`, `bool`, `void`, `return`, etc., conforme forem adicionados ao lexer), identificadores, literais (inteiro, ponto flutuante, caractere, string), operadores e delimitadores (`;` `,` `()` `[]` `{}`), além de espaços e comentários (se forem suportados).

### Casos válidos (exemplos)

| ID | Entrada resumida | Resultado esperado |
|----|------------------|---------------------|
| L1 | `int contador;` | Tokens de tipo, identificador e `;` sem erro léxico. |
| L2 | `3.14  42  'x'  "texto"` | Literais numéricos, caractere e string reconhecidos. |
| L3 | `a += 1;  b == c && d` | Operadores compostos e lógicos tokenizados corretamente (sem fundir `=` com `==`, etc.). |
| L4 | `_var1 __nome` | Identificadores com underscore aceitos, se a regra do lexer permitir. |

### Casos inválidos (léxico)

| ID | Entrada resumida | Resultado esperado |
|----|------------------|---------------------|
| LX1 | Caracteres fora do alfabeto da linguagem (ex.: `` ` `` ou `$` fora de string) | Erro ou aviso léxico explícito (ex.: caractere inválido); **não** seguir como se fosse token válido. |
| LX2 | String não fechada `"abc` | Comportamento definido (erro léxico ou recuperação documentada). |
| LX3 | Literal de caractere vazio ou malformado `''` ou `'ab'` | Rejeição ou regra documentada. |

**Critério de aceitação (Sprint 1):** para entradas válidas, não há mensagem de caractere/token inválido; para inválidas, falha antes ou no lexer, com mensagem clara e linha quando aplicável.

---

## Sprint 2 — Análise sintática

Objetivo: validar a gramática sobre o escopo: declarações (com e sem inicialização, vírgulas), modificador `const`, arrays unidimensionais, expressões com operadores do escopo, estruturas de controle (`if` / `else`, `while`, `do while`, `for`, `switch` / `case` / `default`), `break`, `continue`, `return` (incluindo `return 0;`), blocos, protótipos e definições de função, chamadas.

### Casos válidos (estrutura)

| ID | Construção | Notas |
|----|------------|--------|
| S1 | Declaração simples e múltipla | `int a;` `float x = 1.0f, y;` |
| S2 | Array 1D | `int v[10];` `char s[256];` |
| S3 | Expressão com parênteses e precedência | `(a + b) * c` |
| S4 | `if` aninhado e `else` associado | Estrutura conforme gramática escolhida. |
| S5 | `for` completo e com partes vazias (se permitido) | Conforme subconjunto definido na gramática. |
| S6 | `switch` com `case`, `break`, `default` | Pelo menos um fluxo linear e um com `break`. |
| S7 | Função com lista de parâmetros e corpo em bloco | Incluir `return` opcional no fim. |

### Casos inválidos (sintaxe)

| ID | Exemplo típico | Erro esperado |
|----|----------------|---------------|
| SY1 | `int = 5;` | Falta de identificador após tipo. |
| SY2 | `if x > 0 { }` | Falta de parênteses em torno da condição (se a gramática exige estilo C clássico). |
| SY3 | `float ;` | Declarador ausente. |
| SY4 | `x + * y;` | Expressão malformada. |
| SY5 | Chaves ou parênteses não balanceados | Erro sintático com linha útil. |

**Critério de aceitação (Sprint 2):** programas válidos não geram `Erro sintático` em `stderr`; inválidos geram (alinhado ao que `tests.py` verifica em `tests/valid` e `tests/invalid`).

---

## Sprint 3 — Análise semântica

Objetivo: regras que dependem de contexto e da tabela de símbolos, não só da forma das frases.

### Tipos e expressões

| ID | Cenário | Resultado esperado |
|----|---------|---------------------|
| SEM1 | Atribuir `float` a `int` ou mistura em operadores | Conversão definida **ou** erro semântico explícito. |
| SEM2 | Condicionais de `if` / `while` / `for` não escalares | Erro ou regra documentada (ex.: exigir tipo compatível com “verdadeiro/falso”). |
| SEM3 | Uso de `++` / `--` em expressão inválida (ex.: literal) | Erro semântico. |

### Variáveis, `const` e escopo

| ID | Cenário | Resultado esperado |
|----|---------|---------------------|
| SEM4 | Variável usada antes da declaração no mesmo escopo | Erro (“não declarada”). |
| SEM5 | Redeclaração no mesmo escopo | Erro (“já declarada”). |
| SEM6 | Atribuição a identificador `const` | Erro semântico. |
| SEM7 | Variável local oculta global com mesmo nome | Resolução conforme escopo aninhado documentado. |

### Arrays e ponteiros (no subconjunto do escopo)

| ID | Cenário | Resultado esperado |
|----|---------|---------------------|
| SEM8 | Índice não inteiro em `v[i]` | Erro semântico. |
| SEM9 | Tamanho de array inválido em declaração (negativo ou não constante, se exigido) | Erro conforme regra do projeto. |
| SEM10 | Tipos incompatíveis em ponteiro/aritmética (se suportado) | Erro ou regra estrita documentada. |

### Funções

| ID | Cenário | Resultado esperado |
|----|---------|---------------------|
| SEM11 | Chamada com número errado de argumentos | Erro semântico. |
| SEM12 | Chamada com tipos incompatíveis com o protótipo | Erro semântico. |
| SEM13 | `return` em função `void` com expressão (ou ausência de `return` em função não-`void`) | Erro conforme regra adotada. |
| SEM14 | Recursão infinita “benigna” para teste de pilha | Comportamento definido (limite ou execução até interrupção). |

**Critério de aceitação (Sprint 3):** mensagens de erro semânticas estáveis (texto e, se possível, linha/coluna); programas semanticamente corretos seguem para a fase de execução sem falsos positivos.

---

## Sprint 4 — Execução (interpretação)

Objetivo: validar saída e estado ao interpretar o subconjunto do escopo, incluindo biblioteca padrão reduzida e fluxo de controle.

### Expressões e E/S

| ID | Cenário | Verificação |
|----|---------|-------------|
| EX1 | Aritmética e relacionais com valores conhecidos | Resultado impresso ou armazenado igual ao esperado (ex.: divisão inteira vs ponto flutuante, conforme regra do interpretador). |
| EX2 | `printf` com formatos usados no projeto (`%d`, `%f`, `%c`, `%s`) | Saída idêntica à esperada (incluindo `\n`). |
| EX3 | `scanf` lendo para variáveis de tipos suportados | Estado da memória / saída subsequente corretos com entrada de teste controlada. |

### Controle de fluxo

| ID | Cenário | Verificação |
|----|---------|-------------|
| EX4 | `if` / `else if` / `else` — todos os ramos | Cobrir cada ramo com asserts de saída. |
| EX5 | `while` e `do while` — zero iterações e múltiplas iterações | Contagem de impressões ou valor final de acumulador. |
| EX6 | `for` — laços aninhados | Resultado determinístico (ex.: tabela pequena). |
| EX7 | `switch` com `break` e sem `break` (fall-through) | Comportamento conforme C no subconjunto suportado. |
| EX8 | `break` / `continue` em laços aninhados | Salto ao alvo correto. |
| EX9 | `return` e `return 0` | Encerramento do fluxo da função `main` ou auxiliar com valor esperado. |

### Funções e recursão

| ID | Cenário | Verificação |
|----|---------|-------------|
| EX10 | Chamada com parâmetros por valor | Callee não altera cópia vista pelo caller além do definido. |
| EX11 | Função recursiva (ex.: fatorial, Fibonacci pequeno) | Resultado correto para entradas na tabela de teste. |

### Biblioteca (subconjunto do escopo)

| Cabeçalho | Funções (exemplos de caso) |
|-----------|----------------------------|
| `math.h` | `sqrt`, `pow`, `abs`, `floor`, `ceil`, `round` — comparar com valores de referência para poucos argumentos. |
| `string.h` | `strlen`, `strcpy`, `strcmp`, `strcat`, `strchr` — strings curtas com e sem caractere encontrado. |
| `stdlib.h` | `atoi`, `atof`, `rand`/`srand` determinístico, `exit`. |
| `ctype.h` | `isalpha`, `isdigit`, `isspace` — alguns caracteres ASCII de fronteira. |

**Critério de aceitação (Sprint 4):** para cada programa de teste, saída padrão (e código de saída, se aplicável) se coincide; não há vazamento de comportamento indefinido do C real dentro do que o projeto declara como suportado.

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 13/05/26 | Criação da página com seu respectivo conteúdo | Camila Careli |
