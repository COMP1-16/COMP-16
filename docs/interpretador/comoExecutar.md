# Como executar e testar o interpretador

> Este guia descreve o fluxo recomendado para compilar o projeto, rodar o binário com código de entrada e validar o comportamento com a suíte de testes automatizada.

## Pré-requisitos

Instale as ferramentas na versão mínima indicada no repositório (ou superior compatível):

| Ferramenta | Uso |
|------------|-----|
| **GCC** | Compilação do interpretador |
| **Flex** | Geração do analisador léxico a partir de `lexer/lexer.l` |
| **Bison** | Geração do analisador sintático a partir de `parser/parser.y` |
| **GNU Make** | Orquestração dos alvos `build`, `run` e `test` |
| **Python 3** | Execução de `tests.py` |

O projeto foi pensado para ambientes no estilo Unix (Linux, macOS ou **WSL** no Windows). Em máquinas Windows sem WSL, é preciso ter um ambiente equivalente (por exemplo MSYS2 com `make`, `gcc`, `flex` e `bison`) ou usar WSL para seguir os mesmos comandos abaixo.

Na raiz do repositório, confira se as ferramentas estão no `PATH`:

```bash
gcc --version
flex --version
bison --version
make --version
python3 --version
```

---

## Compilar

Na raiz do clone do repositório:

```bash
make
```

ou explicitamente:

```bash
make build
```

Isso invoca o Bison e o Flex sobre os arquivos da gramática e do lexer, gera os fontes intermediários e produz o executável **`interpretador`** na raiz do projeto.

Para remover artefatos de build e o binário:

```bash
make clean
```

---

## Executar o interpretador

O ponto de entrada (`main`) chama `yyparse()` e lê o programa pela entrada padrão (stdin). Não é obrigatório passar o nome do arquivo como argumento; o uso típico é redirecionar ou encanar um arquivo fonte.

**Exemplo com redirecionamento:**

```bash
./interpretador < caminho/para/arquivo.c
```

**Exemplo com pipe:**

```bash
cat caminho/para/arquivo.c | ./interpretador
```

O alvo `make run` apenas compila (se necessário) e executa `./interpretador` sem redirecionar entrada; nesse caso você precisaria digitar o código (ou colar) e finalizar a entrada com **Ctrl+D** (EOF) no terminal.

Para depuração rápida, um arquivo pequeno em C no subconjunto aceito pode ser mantido na raiz ou em `tests/valid/` e usado com `<` ou `cat`.

---

## Testar de forma automatizada

A suíte é acionada pelo Make, que compila e em seguida chama o script Python:

```bash
make test
```

Equivalente manual, após `make build`:

```bash
python3 tests.py
```

### O que o script verifica

O executável é sempre `./interpretador` (caminho relativo à raiz do repositório, como no `Makefile`).

- **`tests/valid/`** — para cada arquivo, o conteúdo é enviado ao interpretador via stdin. O teste passa se não aparecer a mensagem `Erro sintático` na saída de erro padrão (`stderr`).
- **`tests/invalid/`** — o teste passa se aparecer `Erro sintático` em `stderr`, ou seja, o analisador deve rejeitar entradas inválidas de forma consistente com o que o script espera.

No final, o script imprime um resumo por pasta e o total de casos que passaram ou falharam.

### Boas práticas ao testar

1. Sempre rode `make test` a partir da raiz do repositório, para que `./interpretador` e os caminhos `tests/valid` e `tests/invalid` existam.
2. Após mudanças na gramática ou no lexer, execute `make clean && make test` para garantir que não restaram artefatos antigos de Bison/Flex.
4. Se um teste falhar, o script mostra trechos de `stdout` e `stderr`; use isso para comparar com uma execução manual `./interpretador < tests/valid/seu_arquivo.txt`.

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 13/05/26 | Criação da página com seu respectivo conteúdo | Camila Careli |