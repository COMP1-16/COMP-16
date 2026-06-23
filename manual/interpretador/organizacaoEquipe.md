# Organização da equipe

> A Equipe 16 (Turma 01, Semestre 2026.1) organizou o desenvolvimento do interpretador em sprints quinzenais, com acompanhamento semanal e rastreamento das atividades no [GitHub Projects](https://github.com/orgs/COMP1-16/projects) da organização.

## Processo de Trabalho

### Sprints de 2 semanas

- Planejamento das atividades (segundas-feiras);
- Implementação dos módulos;
- Revisão das entregas;
- Ajustes e correções.

### Reuniões de acompanhamento semanais

- Sábados à tarde;
- Discussão de dificuldades;
- Definição das próximas tarefas.

---

## Cronograma

O cronograma abaixo reflete o planejamento e o andamento das issues no GitHub Projects. Cada sprint tem duração de duas semanas; issues marcadas como *(aberta)* permanecem em andamento ou aguardam conclusão.

### Sprint 1 (14/04 - 27/04)

| Issue | Atividade | Descrição | Responsáveis |
|-------|-----------|-----------|--------------|
| [#1](https://github.com/COMP1-16/COMP-16/issues/1) | Doc - Sintaxe e semântica C | Criar documento que identifica sintaxe e semântica básicas da linguagem C. | Pedro Henrique, Davi Severiano |
| [#2](https://github.com/COMP1-16/COMP-16/issues/2) | Doc - Escopo C que será interpretado | Definir o escopo da linguagem C que será interpretado pelo projeto. | Mateus Barreto, Camila Careli |
| [#3](https://github.com/COMP1-16/COMP-16/issues/3) | Configuração inicial repositório | Criar arquivos base definidos no repositório template da disciplina. | Vinícius de Jesus, Davi Severiano |
| [#4](https://github.com/COMP1-16/COMP-16/issues/4) | Criação regras léxicas iniciais (Flex) | Definir tokens básicos no `lexer.l` conforme o escopo da linguagem. | Vinícius de Jesus, Pedro Henrique |
| [#10](https://github.com/COMP1-16/COMP-16/issues/10) | Pesquisar testes automatizados - Escopo inicial | Levantar abordagens de testes automatizados para o escopo inicial. | Camila Careli, Vinícius de Jesus, Mateus Barreto |
| [#11](https://github.com/COMP1-16/COMP-16/issues/11) | Pesquisar lacunas da interpretação - Escopo inicial | Identificar o que falta para a interpretação do escopo inicial. | Davi Severiano, Pedro Henrique |

### Sprint 2 (28/04 - 11/05)

| Issue | Atividade | Descrição | Responsáveis |
|-------|-----------|-----------|--------------|
| [#6](https://github.com/COMP1-16/COMP-16/issues/6) | Criação gramática inicial (Bison) | Definir regras sintáticas básicas: literais, declarações, atribuições e operações aritméticas. | Vinícius de Jesus, Davi Severiano, Mateus Barreto, Camila Careli |
| [#8](https://github.com/COMP1-16/COMP-16/issues/8) | Testes - gramática inicial | Criar casos de teste para a gramática inicial (válidos e inválidos). | Vinícius de Jesus, Mateus Barreto, Camila Careli |
| [#15](https://github.com/COMP1-16/COMP-16/issues/15) | Implementar estrutura e criação da AST | Definir tipos de nós e funções de criação da AST (`ast.c` / `ast.h`). | Pedro Henrique |
| [#16](https://github.com/COMP1-16/COMP-16/issues/16) | Percorrer AST para executar código | Percorrer a AST anotada para executar os nós do programa. | Pedro Henrique |
| [#17](https://github.com/COMP1-16/COMP-16/issues/17) | Implementar Tabela de Símbolos | Implementar inserção, consulta e remoção de símbolos (`simbolos.c` / `simbolos.h`). | Davi Severiano |
| [#18](https://github.com/COMP1-16/COMP-16/issues/18) | Adicionar Makefile no README | Documentar no README os comandos `make` e a execução de testes. | Vinícius de Jesus, Camila Careli, Mateus Barreto |
| [#14](https://github.com/COMP1-16/COMP-16/issues/14) | Finalizar interpretação do escopo inicial | Concluir interpretação de literais, declarações, atribuições e operações aritméticas. | Pedro Henrique, Davi Severiano |

### Sprint 3 (12/05 - 25/05)

| Issue | Atividade | Descrição | Responsáveis |
|-------|-----------|-----------|--------------|
| [#24](https://github.com/COMP1-16/COMP-16/issues/24) | Análise léxica + sintática: Comparações e Loops | Adicionar lexer e parser para operadores relacionais, lógicos e estruturas de loop. | Pedro Henrique, Davi Severiano |
| [#7](https://github.com/COMP1-16/COMP-16/issues/7) | Interpretação - Comparações e Loops | Implementar análise e execução de comparações, `if`/`else`, `while` e `for`. | — |
| [#23](https://github.com/COMP1-16/COMP-16/issues/23) | Análise semântica - Comparações e Loops *(aberta)* | Validar tipos e regras semânticas para comparações e estruturas de repetição. | — |
| [#25](https://github.com/COMP1-16/COMP-16/issues/25) | Análise semântica e interpretação: Comparações e Loops | Implementar semântica e interpretador para operadores relacionais, lógicos e loops. | Vinícius de Jesus, Camila Careli, Mateus Barreto |
| [#39](https://github.com/COMP1-16/COMP-16/issues/39) | Estrutura do interpretador | Documentar no GitPages a arquitetura e organização de pastas do projeto. | Camila Careli |

### Sprint 4 (26/05 - 08/06)

| Issue | Atividade | Descrição | Responsáveis |
|-------|-----------|-----------|--------------|
| [#26](https://github.com/COMP1-16/COMP-16/issues/26) | Interpretação: Switch e funções | Implementar `switch`/`case`/`default`, `break`/`return` e funções com recursão. | — |
| [#27](https://github.com/COMP1-16/COMP-16/issues/27) | Interpretação básica: Switch | Implementar interpretação de `switch`, `case`, `default`, `break` e `return`. | Vinícius de Jesus, Camila Careli |
| [#28](https://github.com/COMP1-16/COMP-16/issues/28) | Interpretação básica: Funções | Implementar declaração, chamada, parâmetros, retorno e recursão. | Pedro Henrique, Davi Severiano, Mateus Barreto |
| [#35](https://github.com/COMP1-16/COMP-16/issues/35) | Casos de teste - Sprint 4 | Documentar casos de teste por fase de análise até o escopo da sprint 4. | Pedro Henrique, Camila Careli |

### Sprint 5 (09/06 - 22/06)

| Issue | Atividade | Descrição | Responsáveis |
|-------|-----------|-----------|--------------|
| [#29](https://github.com/COMP1-16/COMP-16/issues/29) | Interpretação - Bibliotecas principais *(aberta)* | Épico: `stdio.h`, `string.h` e `math.h` no pipeline completo. | — |
| [#30](https://github.com/COMP1-16/COMP-16/issues/30) | Interpretação básica: libs (stdio.h, string.h) | Implementar `printf`/`scanf` e funções previstas de `string.h`. | Vinícius de Jesus, Mateus Barreto |
| [#31](https://github.com/COMP1-16/COMP-16/issues/31) | Interpretação básica: libs (math.h) | Implementar `sqrt`, `pow`, `abs`, `floor`, `ceil` e `round`. | Davi Severiano |
| [#36](https://github.com/COMP1-16/COMP-16/issues/36) | Casos de teste - Sprint 5 | Documentar casos de teste por fase de análise até o escopo da sprint 5. | Pedro Henrique, Mateus Barreto |
| [#40](https://github.com/COMP1-16/COMP-16/issues/40) | Como executar o interpretador | Documentar build, execução e suíte de testes no GitPages. | Vinícius de Jesus |

### Sprint 6 (23/06 - 06/07)

| Issue | Atividade | Descrição | Responsáveis |
|-------|-----------|-----------|--------------|
| [#32](https://github.com/COMP1-16/COMP-16/issues/32) | Interpretação: Bibliotecas extras *(aberta)* | Épico: funções adicionais de `string.h`, `stdlib.h` e `ctype.h`. | — |
| [#33](https://github.com/COMP1-16/COMP-16/issues/33) | Interpretação básica: libs extras (string.h, stdlib.h) *(aberta)* | Implementar `strcat`, `strchr`, `abs`, `rand` e `srand`. | Pedro Henrique, Davi Severiano, Mateus Barreto |
| [#34](https://github.com/COMP1-16/COMP-16/issues/34) | Interpretação básica: libs extras (stdlib.h, ctype.h) *(aberta)* | Implementar `atoi`, `atof`, `exit` e funções de `ctype.h`. | Vinícius de Jesus, Camila Careli |
| [#37](https://github.com/COMP1-16/COMP-16/issues/37) | Casos de teste - Sprint 6 | Documentar casos de teste por fase de análise até o escopo da sprint 6. | Camila Careli |
| [#46](https://github.com/COMP1-16/COMP-16/issues/46) | Criar estrutura básica do otimizador | Implementar otimizações como constant folding e eliminação de código morto. | Pedro Henrique |

### Sprint 7 (07/07 - 20/07)

| Issue | Atividade | Descrição | Responsáveis |
|-------|-----------|-----------|--------------|
| [#38](https://github.com/COMP1-16/COMP-16/issues/38) | Casos de teste - Sprint 7 *(aberta)* | Documentar casos de teste por fase de análise até o escopo final do projeto. | — |
| [#41](https://github.com/COMP1-16/COMP-16/issues/41) | Organização da equipe *(aberta)* | Documentar processo de trabalho, cronograma e pontos de controle no GitPages. | Pedro Henrique |

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 21/06/26 | Criação da página com processo de trabalho e cronograma por sprint | Pedro Henrique |
