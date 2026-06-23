# Organiza├º├úo da equipe

> A Equipe 16 (Turma 01, Semestre 2026.1) organizou o desenvolvimento do interpretador em sprints quinzenais, com acompanhamento semanal e rastreamento das atividades no [GitHub Projects](https://github.com/orgs/COMP1-16/projects) da organiza├º├úo.

## Processo de Trabalho

### Sprints de 2 semanas

- Planejamento das atividades (segundas-feiras);
- Implementa├º├úo dos m├│dulos;
- Revis├úo das entregas;
- Ajustes e corre├º├Áes.

### Reuni├Áes de acompanhamento semanais

- S├íbados ├á tarde;
- Discuss├úo de dificuldades;
- Defini├º├úo das pr├│ximas tarefas.

---

## Cronograma

O cronograma abaixo reflete o planejamento e o andamento das issues no GitHub Projects. Cada sprint tem dura├º├úo de duas semanas; issues marcadas como *(aberta)* permanecem em andamento ou aguardam conclus├úo.

### Sprint 1 (14/04 - 27/04)

| Issue | Atividade | Descri├º├úo | Respons├íveis |
|-------|-----------|-----------|--------------|
| [#1](https://github.com/COMP1-16/COMP-16/issues/1) | Doc - Sintaxe e sem├óntica C | Criar documento que identifica sintaxe e sem├óntica b├ísicas da linguagem C. | Pedro Henrique, Davi Severiano |
| [#2](https://github.com/COMP1-16/COMP-16/issues/2) | Doc - Escopo C que ser├í interpretado | Definir o escopo da linguagem C que ser├í interpretado pelo projeto. | Mateus Barreto, Camila Careli |
| [#3](https://github.com/COMP1-16/COMP-16/issues/3) | Configura├º├úo inicial reposit├│rio | Criar arquivos base definidos no reposit├│rio template da disciplina. | Vin├¡cius de Jesus, Davi Severiano |
| [#4](https://github.com/COMP1-16/COMP-16/issues/4) | Cria├º├úo regras l├®xicas iniciais (Flex) | Definir tokens b├ísicos no `lexer.l` conforme o escopo da linguagem. | Vin├¡cius de Jesus, Pedro Henrique |
| [#10](https://github.com/COMP1-16/COMP-16/issues/10) | Pesquisar testes automatizados - Escopo inicial | Levantar abordagens de testes automatizados para o escopo inicial. | Camila Careli, Vin├¡cius de Jesus, Mateus Barreto |

### Sprint 2 (28/04 - 11/05)

| Issue | Atividade | Descri├º├úo | Respons├íveis |
|-------|-----------|-----------|--------------|
| [#6](https://github.com/COMP1-16/COMP-16/issues/6) | Cria├º├úo gram├ítica inicial (Bison) | Definir regras sint├íticas b├ísicas: literais, declara├º├Áes, atribui├º├Áes e opera├º├Áes aritm├®ticas. | Vin├¡cius de Jesus, Davi Severiano, Mateus Barreto, Camila Careli |
| [#8](https://github.com/COMP1-16/COMP-16/issues/8) | Testes - gram├ítica inicial | Criar casos de teste para a gram├ítica inicial (v├ílidos e inv├ílidos). | Vin├¡cius de Jesus, Mateus Barreto, Camila Careli |
| [#15](https://github.com/COMP1-16/COMP-16/issues/15) | Implementar estrutura e cria├º├úo da AST | Definir tipos de n├│s e fun├º├Áes de cria├º├úo da AST (`ast.c` / `ast.h`). | Pedro Henrique |
| [#16](https://github.com/COMP1-16/COMP-16/issues/16) | Percorrer AST para executar c├│digo | Percorrer a AST anotada para executar os n├│s do programa. | Pedro Henrique |
| [#18](https://github.com/COMP1-16/COMP-16/issues/18) | Adicionar Makefile no README | Documentar no README os comandos `make` e a execu├º├úo de testes. | Vin├¡cius de Jesus, Camila Careli, Mateus Barreto |
| [#14](https://github.com/COMP1-16/COMP-16/issues/14) | Finalizar interpreta├º├úo do escopo inicial | Concluir interpreta├º├úo de literais, declara├º├Áes, atribui├º├Áes e opera├º├Áes aritm├®ticas. | Pedro Henrique, Davi Severiano |

### Sprint 3 (12/05 - 25/05)

| Issue | Atividade | Descri├º├úo | Respons├íveis |
|-------|-----------|-----------|--------------|
| [#17](https://github.com/COMP1-16/COMP-16/issues/17) | Implementar Tabela de S├¡mbolos | Implementar inser├º├úo, consulta e remo├º├úo de s├¡mbolos (`simbolos.c` / `simbolos.h`). | Davi Severiano |
| [#24](https://github.com/COMP1-16/COMP-16/issues/24) | An├ílise l├®xica + sint├ítica: Compara├º├Áes e Loops | Adicionar lexer e parser para operadores relacionais, l├│gicos e estruturas de loop. | Pedro Henrique, Davi Severiano |
| [#25](https://github.com/COMP1-16/COMP-16/issues/25) | An├ílise sem├óntica e interpreta├º├úo: Compara├º├Áes e Loops | Implementar sem├óntica e interpretador para operadores relacionais, l├│gicos e loops. | Vin├¡cius de Jesus, Camila Careli, Mateus Barreto |
| [#39](https://github.com/COMP1-16/COMP-16/issues/39) | Estrutura do interpretador | Documentar no GitPages a arquitetura e organiza├º├úo de pastas do projeto. | Camila Careli |

### Sprint 4 (26/05 - 08/06)

| Issue | Atividade | Descri├º├úo | Respons├íveis |
|-------|-----------|-----------|--------------|
| [#27](https://github.com/COMP1-16/COMP-16/issues/27) | Interpreta├º├úo b├ísica: Switch | Implementar interpreta├º├úo de `switch`, `case`, `default`, `break` e `return`. | Vin├¡cius de Jesus, Camila Careli |
| [#28](https://github.com/COMP1-16/COMP-16/issues/28) | Interpreta├º├úo b├ísica: Fun├º├Áes | Implementar declara├º├úo, chamada, par├ómetros, retorno e recurs├úo. | Pedro Henrique, Davi Severiano, Mateus Barreto |
| [#35](https://github.com/COMP1-16/COMP-16/issues/35) | Casos de teste - Sprint 4 | Documentar casos de teste por fase de an├ílise at├® o escopo da sprint 4. | Pedro Henrique, Camila Careli |

### Sprint 5 (09/06 - 22/06)

| Issue | Atividade | Descri├º├úo | Respons├íveis |
|-------|-----------|-----------|--------------|
| [#30](https://github.com/COMP1-16/COMP-16/issues/30) | Interpreta├º├úo b├ísica: libs (stdio.h, string.h) | Implementar `printf`/`scanf` e fun├º├Áes previstas de `string.h`. | Vin├¡cius de Jesus, Mateus Barreto |
| [#31](https://github.com/COMP1-16/COMP-16/issues/31) | Interpreta├º├úo b├ísica: libs (math.h) | Implementar `sqrt`, `pow`, `abs`, `floor`, `ceil` e `round`. | Davi Severiano |
| [#36](https://github.com/COMP1-16/COMP-16/issues/36) | Casos de teste - Sprint 5 | Documentar casos de teste por fase de an├ílise at├® o escopo da sprint 5. | Pedro Henrique, Mateus Barreto |
| [#40](https://github.com/COMP1-16/COMP-16/issues/40) | Como executar o interpretador | Documentar build, execu├º├úo e su├¡te de testes no GitPages. | Vin├¡cius de Jesus |

### Sprint 6 (23/06 - 06/07)

| Issue | Atividade | Descri├º├úo | Respons├íveis |
|-------|-----------|-----------|--------------|
| [#32](https://github.com/COMP1-16/COMP-16/issues/32) | Interpreta├º├úo: Bibliotecas extras *(aberta)* | ├ëpico: fun├º├Áes adicionais de `string.h`, `stdlib.h` e `ctype.h`. | ÔÇö |
| [#33](https://github.com/COMP1-16/COMP-16/issues/33) | Interpreta├º├úo b├ísica: libs extras (string.h, stdlib.h) *(aberta)* | Implementar `strcat`, `strchr`, `abs`, `rand` e `srand`. | Pedro Henrique, Davi Severiano, Mateus Barreto |
| [#34](https://github.com/COMP1-16/COMP-16/issues/34) | Interpreta├º├úo b├ísica: libs extras (stdlib.h, ctype.h) *(aberta)* | Implementar `atoi`, `atof`, `exit` e fun├º├Áes de `ctype.h`. | Vin├¡cius de Jesus, Camila Careli |
| [#37](https://github.com/COMP1-16/COMP-16/issues/37) | Casos de teste - Sprint 6 | Documentar casos de teste por fase de an├ílise at├® o escopo da sprint 6. | Camila Careli |
| [#46](https://github.com/COMP1-16/COMP-16/issues/46) | Criar estrutura b├ísica do otimizador | Implementar otimiza├º├Áes como constant folding e elimina├º├úo de c├│digo morto. | Pedro Henrique |

### Sprint 7 (07/07 - 20/07)

| Issue | Atividade | Descri├º├úo | Respons├íveis |
|-------|-----------|-----------|--------------|
| [#38](https://github.com/COMP1-16/COMP-16/issues/38) | Casos de teste - Sprint 7 *(aberta)* | Documentar casos de teste por fase de an├ílise at├® o escopo final do projeto. | ÔÇö |
| [#41](https://github.com/COMP1-16/COMP-16/issues/41) | Organiza├º├úo da equipe *(aberta)* | Documentar processo de trabalho, cronograma e pontos de controle no GitPages. | Pedro Henrique |

---

## Hist├│rico de Vers├úo

| Vers├úo | Data | Descri├º├úo | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 21/06/26 | Cria├º├úo da p├ígina com processo de trabalho e cronograma por sprint | Pedro Henrique |
