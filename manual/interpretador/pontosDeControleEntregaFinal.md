# Entrega final

> Marco correspondente às entregas das **Sprints 6 e 7** (23/06 – 20/07/2026) e consolidação do projeto para a entrega final da disciplina.

## Resumo das entregas

### Sprint 6 — Otimização, bibliotecas extras e testes

- Estrutura do otimizador com constant folding e eliminação de código morto (DCE) ([#46](https://github.com/COMP1-16/COMP-16/issues/46)).
- Implementação parcial de `stdlib.h`: `atoi`, `atof` e `exit` ([#34](https://github.com/COMP1-16/COMP-16/issues/34)).
- Suíte de testes padronizada em `testes/` com quatro camadas por feature (sintático, semântico, otimização e execução) — **361 casos** em 12 módulos.
- Documentação de casos de teste da sprint 6 ([#37](https://github.com/COMP1-16/COMP-16/issues/37)).

### Sprint 7 — Documentação final

- Organização da equipe: processo de trabalho, cronograma e pontos de controle ([#41](https://github.com/COMP1-16/COMP-16/issues/41)).
- Casos de teste da sprint 7 ([#38](https://github.com/COMP1-16/COMP-16/issues/38)) — em andamento.
- Registro transparente do [uso de IA](../qualidade.md) no projeto.

## Bibliotecas e escopo

| Biblioteca | Status na entrega final |
|------------|-------------------------|
| `math.h` | Implementada (`sqrt`, `pow`, `abs`, `floor`, `ceil`, `round`) |
| `stdlib.h` | Parcial (`atoi`, `atof`, `exit`); `rand`/`srand` não implementados |
| `stdio.h` | Parcial (`printf`); `scanf` não implementado |
| `ctype.h` | Não implementada |
| `string.h` | Não implementada (ver destaque abaixo) |

!!! warning "string.h não implementada"

    A biblioteca **`string.h`** (`strlen`, `strcpy`, `strcmp`, `strcat`, `strchr`) **não teve interpretação implementada** neste projeto. Essas funções dependem de um escopo maior de interpretação — em especial **vetores e ponteiros** — do que o planejado e efetivamente executado pela equipe.

    O interpretador adota o tipo estendido `string` para literais e variáveis de texto, sem modelagem de `char[]` nem aritmética de ponteiros. Detalhes no [escopo do interpretador](escopo.md).

## Artefatos principais

| Componente | Localização |
|------------|-------------|
| Otimizador | `lib/analysis/otimizador.c` |
| Análise semântica | `lib/analysis/semantico.c` |
| Interpretador | `lib/exec/interpreter.c` |
| Stdlib | `lib/libs/stdlib.c` |
| Suíte de testes | `testes/`, `tests.py` |
| Documentação | `manual/` → GitHub Pages em `docs/` |

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 21/06/26 | Resumo das entregas finais (Sprints 6 e 7) | Pedro Henrique |
