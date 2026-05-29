# Uso de Inteligência Artificial no projeto

> Este documento registra de forma transparente como a Equipe 16 utilizou ferramentas de Inteligência Artificial (IA) durante o desenvolvimento do interpretador de C e da documentação do projeto. O objetivo é deixar claro o que foi apoiado por IA, o que foi produzido integralmente pela equipe e quais cuidados foram adotados para preservar a integridade acadêmica do trabalho.

## Contexto

Ferramentas de IA generativa podem acelerar tarefas auxiliares, como redação de documentação, revisão de texto ou exploração de alternativas, mas **não substituem** a compreensão do código, das decisões de projeto nem a responsabilidade pelos entregáveis.

A equipe adota IA como **assistente**, nunca como autor único do interpretador.

---

## Ferramentas utilizadas

| Ferramenta | Finalidade no projeto |
|------------|------------------------|
| **Cursor IA** | Assistente integrado ao VS Code; utilizado para acelerar organização de arquivos, inclusão de conteúdo no GitPages/documentação e auxílio em refatoração |
| **ChatGPT** | Utilizado para análise e entendimento de código, explicação de partes não compreendidas, esclarecimento de dúvidas técnicas, entendimento da arquitetura do interpretador, auxílio em implementação e organização do fluxo de execução |
| **Gemini** | Utilizado para perguntas conceituais e textuais, entendimento do funcionamento do interpretador, direcionamento sobre arquitetura, integração entre componentes e definição de próximos passos do projeto |

---

## Onde a IA foi utilizada

### Documentação

- Estruturação inicial de páginas do manual (MkDocs);
- Revisão ortográfica e padronização de tom em textos já redigidos pelos integrantes;
- Sugestões de organização de seções e links entre páginas relacionadas;
- Organização do deploy quando estava com conflito.

### Desenvolvimento do interpretador

- Consulta sobre sintaxe de Flex e Bison;
- Apoio na escrita de casos de teste e na interpretação de mensagens de erro do compilador.

### O que **não** foi delegado à IA

- Definição do escopo da linguagem C aceita pelo interpretador;
- Decisões de arquitetura (organização de `lexer/`, `parser/`, tipos semânticos);
- Validação final de correção: todo código commitado foi lido, testado e aprovado por pelo menos um integrante da equipe;
- Execução e interpretação dos resultados de `make test` e depuração de falhas nos pontos de controle.

---

## Boas práticas adotadas pela equipe

1. **Revisão humana obrigatória** — Nenhum trecho gerado por IA entra no repositório sem revisão, adaptação ao estilo do projeto e execução dos testes.
2. **Compreensão do código** — Cada integrante deve ser capaz de explicar o que o código faz, não apenas reproduzi-lo.
3. **Sem dados sensíveis** — Não enviamos credenciais, tokens ou informações pessoais a serviços de IA.
4. **Alinhamento com a disciplina** — Respeitamos as orientações do Prof. Dr. Sergio Freitas e as regras da Universidade de Brasília sobre uso de IA em trabalhos acadêmicos.

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 28/05/26 | Criação da página sobre uso de IA no projeto | Camila Careli |
