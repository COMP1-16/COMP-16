# Entrega final

> Marco correspondente ├ás entregas das **Sprints 6 e 7** (23/06 ÔÇô 20/07/2026) e consolida├º├úo do projeto para a entrega final da disciplina.

## Resumo das entregas

### Sprint 6 ÔÇö Otimiza├º├úo, bibliotecas extras e testes

- Estrutura do otimizador com constant folding e elimina├º├úo de c├│digo morto (DCE) ([#46](https://github.com/COMP1-16/COMP-16/issues/46)).
- Implementa├º├úo parcial de `stdlib.h`: `atoi`, `atof` e `exit` ([#34](https://github.com/COMP1-16/COMP-16/issues/34)).
- Su├¡te de testes padronizada em `testes/` com quatro camadas por feature (sint├ítico, sem├óntico, otimiza├º├úo e execu├º├úo) ÔÇö **361 casos** em 12 m├│dulos.
- Documenta├º├úo de casos de teste da sprint 6 ([#37](https://github.com/COMP1-16/COMP-16/issues/37)).
- P├ígina de [status do escopo](escopo-status.md) comparando documenta├º├úo, implementa├º├úo e cobertura de testes.

### Sprint 7 ÔÇö Documenta├º├úo final

- Organiza├º├úo da equipe: processo de trabalho, cronograma e pontos de controle ([#41](https://github.com/COMP1-16/COMP-16/issues/41)).
- Casos de teste da sprint 7 ([#38](https://github.com/COMP1-16/COMP-16/issues/38)) ÔÇö em andamento.
- Registro transparente do [uso de IA](../qualidade.md) no projeto.

## Bibliotecas e escopo

| Biblioteca | Status na entrega final |
|------------|-------------------------|
| `math.h` | Implementada (`sqrt`, `pow`, `abs`, `floor`, `ceil`, `round`) |
| `stdlib.h` | Parcial (`atoi`, `atof`, `exit`); `rand`/`srand` n├úo implementados |
| `stdio.h` | Parcial (`printf`); `scanf` n├úo implementado |
| `ctype.h` | N├úo implementada |
| `string.h` | N├úo implementada (ver destaque abaixo) |

!!! warning "string.h n├úo implementada"

    A biblioteca **`string.h`** (`strlen`, `strcpy`, `strcmp`, `strcat`, `strchr`) **n├úo teve interpreta├º├úo implementada** neste projeto. Essas fun├º├Áes dependem de um escopo maior de interpreta├º├úo ÔÇö em especial **vetores e ponteiros** ÔÇö do que o planejado e efetivamente executado pela equipe.

    O interpretador adota o tipo estendido `string` para literais e vari├íveis de texto, sem modelagem de `char[]` nem aritm├®tica de ponteiros. Detalhes na matriz de [status do escopo](escopo-status.md).

## Artefatos principais

| Componente | Localiza├º├úo |
|------------|-------------|
| Otimizador | `lib/analysis/otimizador.c` |
| An├ílise sem├óntica | `lib/analysis/semantico.c` |
| Interpretador | `lib/exec/interpreter.c` |
| Stdlib | `lib/libs/stdlib.c` |
| Su├¡te de testes | `testes/`, `tests.py` |
| Documenta├º├úo | `manual/` ÔåÆ GitHub Pages em `docs/` |

---

## Hist├│rico de Vers├úo

| Vers├úo | Data | Descri├º├úo | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 21/06/26 | Resumo das entregas finais (Sprints 6 e 7) | Pedro Henrique |
