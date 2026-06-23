# Ponto de Controle 2 (PC2)

> Marco correspondente às entregas das **Sprints 3 a 5** (12/05 – 22/06/2026).

## Resumo das entregas

No PC2, o interpretador passou de um núcleo aritmético para um subconjunto expressivo de C, com controle de fluxo, funções e bibliotecas principais.

### Sprint 3 — Comparações, condicionais e loops

- Análise léxica e sintática de operadores relacionais e lógicos, `else if` e estruturas de repetição ([#24](https://github.com/COMP1-16/COMP-16/issues/24)).
- Análise semântica e interpretação de `if`/`else`, `while` e `for` ([#25](https://github.com/COMP1-16/COMP-16/issues/25)).
- Documentação da estrutura do interpretador no GitPages ([#39](https://github.com/COMP1-16/COMP-16/issues/39)).

### Sprint 4 — Switch e funções

- Interpretação de `switch`/`case`/`default`, `break` e `return` ([#27](https://github.com/COMP1-16/COMP-16/issues/27)).
- Funções com parâmetros, retorno, escopo local e recursão ([#28](https://github.com/COMP1-16/COMP-16/issues/28)).
- Documentação de casos de teste até o escopo da sprint 4 ([#35](https://github.com/COMP1-16/COMP-16/issues/35)).

### Sprint 5 — Bibliotecas principais e execução

- Implementação de funções de `math.h` (`sqrt`, `pow`, `abs`, `floor`, `ceil`, `round`) ([#31](https://github.com/COMP1-16/COMP-16/issues/31)).
- Suporte parcial a `stdio.h` com `printf` formatado ([#30](https://github.com/COMP1-16/COMP-16/issues/30)).
- Documentação de como executar e testar o interpretador ([#40](https://github.com/COMP1-16/COMP-16/issues/40)).
- Documentação de casos de teste da sprint 5 ([#36](https://github.com/COMP1-16/COMP-16/issues/36)).

## Cobertura de testes (PC2)

| Feature | Pasta em `testes/` |
|---------|-------------------|
| Condicionais | `if_else/` |
| Loops | `while/`, `for/` |
| Operadores lógicos | `operadores_logicos/` |
| Switch | `switch_case/` |
| Funções e recursão | `recursao/` |
| Matemática | `math/` |

## Pendências ao final do PC2

- Issue [#23](https://github.com/COMP1-16/COMP-16/issues/23) (análise semântica isolada de comparações e loops) permanece aberta como item de rastreamento.
- Issue [#29](https://github.com/COMP1-16/COMP-16/issues/29) (épico de bibliotecas principais) aguarda fechamento formal após consolidação das entregas parciais.

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 21/06/26 | Resumo das entregas das Sprints 3 a 5 (PC2) | Pedro Henrique |
