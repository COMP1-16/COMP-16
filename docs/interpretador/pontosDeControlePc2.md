# Ponto de Controle 2 (PC2)

> Marco correspondente ├ás entregas das **Sprints 3 a 5** (12/05 ÔÇô 22/06/2026).

## Resumo das entregas

No PC2, foi finalizada a tabela de s├¡mbolos e o interpretador passou de um n├║cleo aritm├®tico para um subconjunto expressivo de C, controle de fluxo, fun├º├Áes e bibliotecas principais.

### Sprint 3 ÔÇö Tabela de s├¡mbolos, compara├º├Áes, condicionais e loops

- Tabela de s├¡mbolos para vari├íveis e escopos ([#17](https://github.com/COMP1-16/COMP-16/issues/17)).
- An├ílise l├®xica e sint├ítica de operadores relacionais e l├│gicos, `else if` e estruturas de repeti├º├úo ([#24](https://github.com/COMP1-16/COMP-16/issues/24)).
- An├ílise sem├óntica e interpreta├º├úo de `if`/`else`, `while` e `for` ([#25](https://github.com/COMP1-16/COMP-16/issues/25)).
- Documenta├º├úo da estrutura do interpretador no GitPages ([#39](https://github.com/COMP1-16/COMP-16/issues/39)).

### Sprint 4 ÔÇö Switch e fun├º├Áes

- Interpreta├º├úo de `switch`/`case`/`default`, `break` e `return` ([#27](https://github.com/COMP1-16/COMP-16/issues/27)).
- Fun├º├Áes com par├ómetros, retorno, escopo local e recurs├úo ([#28](https://github.com/COMP1-16/COMP-16/issues/28)).
- Documenta├º├úo de casos de teste at├® o escopo da sprint 4 ([#35](https://github.com/COMP1-16/COMP-16/issues/35)).

### Sprint 5 ÔÇö Bibliotecas principais e execu├º├úo

- Implementa├º├úo de fun├º├Áes de `math.h` (`sqrt`, `pow`, `abs`, `floor`, `ceil`, `round`) ([#31](https://github.com/COMP1-16/COMP-16/issues/31)).
- Suporte parcial a `stdio.h` com `printf` formatado ([#30](https://github.com/COMP1-16/COMP-16/issues/30)).
- Documenta├º├úo de como executar e testar o interpretador ([#40](https://github.com/COMP1-16/COMP-16/issues/40)).
- Documenta├º├úo de casos de teste da sprint 5 ([#36](https://github.com/COMP1-16/COMP-16/issues/36)).

## Cobertura de testes (PC2)

| Feature | Pasta em `testes/` |
|---------|-------------------|
| Condicionais | `if_else/` |
| Loops | `while/`, `for/` |
| Operadores l├│gicos | `operadores_logicos/` |
| Switch | `switch_case/` |
| Fun├º├Áes e recurs├úo | `recursao/` |
| Matem├ítica | `math/` |

## Pend├¬ncias ao final do PC2

- Issue [#23](https://github.com/COMP1-16/COMP-16/issues/23) (an├ílise sem├óntica isolada de compara├º├Áes e loops) permanece aberta como item de rastreamento.
- Issue [#29](https://github.com/COMP1-16/COMP-16/issues/29) (├®pico de bibliotecas principais) aguarda fechamento formal ap├│s consolida├º├úo das entregas parciais.

---

## Hist├│rico de Vers├úo

| Vers├úo | Data | Descri├º├úo | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 21/06/26 | Resumo das entregas das Sprints 3 a 5 (PC2) | Pedro Henrique |
