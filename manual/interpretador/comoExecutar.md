# Como executar e testar o interpretador

> Esta seção descreve o fluxo recomendado para compilar o projeto, rodar o binário com código de entrada e validar o comportamento com a suíte de testes automatizada.

## Pre-requisitos

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

Isso invoca o Bison e o Flex sobre os arquivos da gramática e do lexer, gera os fontes intermediários e produz o executável em **`lib/interpreter`** (ou `lib/interpreter.exe` no Windows).

Para remover artefatos de build e o binário:

```bash
make clean
```

---

## Executar o interpretador

O ponto de entrada (`main`) lê o programa pela entrada padrão (stdin). O uso típico é redirecionar ou encanar um arquivo fonte.

**Exemplo com redirecionamento:**

```bash
./lib/interpreter < caminho/para/arquivo.txt
```

**Exemplo com pipe:**

```bash
cat caminho/para/arquivo.txt | ./lib/interpreter
```

O alvo `make run` apenas compila (se necessário) e executa `./lib/interpreter` sem redirecionar entrada; nesse caso você precisaria digitar o código (ou colar) e finalizar a entrada com **Ctrl+D** (EOF) no terminal.

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

Também é possível rodar testes de uma categoria específica:

```bash
make test-for
make test-while
make test-if-else
make test-switch
make test-recursao
make test-otimizador
make test-math
make test-stdlib
make test-decl
make test-atrib
make test-op-arit
make test-op-log
```

### O que o script verifica

Os testes estão organizados em `testes/<categoria>/<fase>/<validos|invalidos>/`. O script roda o interpretador para cada arquivo `.txt` e verifica o resultado de acordo com a fase:

| Fase | Argumento passado | Criterio de sucesso (validos) | Criterio de sucesso (invalidos) |
|------|-------------------|-------------------------------|----------------------------------|
| `sintatico` | `--parse-only` | Nenhum erro reportado | Erro lexico ou sintatico presente |
| `semantico` | `--semantic-only` | Nenhum erro reportado | Erro semantico presente |
| `execucao` | nenhum | Nenhum erro reportado | Qualquer erro presente |

Para os casos de `execucao/validos`, o script realiza tres passagens:

1. **Execucao principal** — verifica se o programa roda sem erros
2. **Saida esperada** — se existir um arquivo `.expected`, compara a saida do interpretador com ele
3. **Exit code** — se existir um arquivo `.exitcode`, compara o codigo de saida com o valor esperado
4. O arquivo `invalido_recursao_infinita.txt` e ignorado automaticamente pelo script pois causaria um loop infinito. Ele consta na lista `SKIP_TESTS` do `tests.py`.

### Boas praticas ao testar

1. Sempre rode `make test` a partir da raiz do repositório, para que `./lib/interpreter` e os caminhos de testes existam.
2. Após mudanças na gramática ou no lexer, execute `make clean && make test` para garantir que não restaram artefatos antigos de Bison/Flex.
3. Se um teste falhar, o script mostra trechos de `stdout` e `stderr`; use isso para comparar com uma execução manual `./lib/interpreter < testes/<categoria>/<fase>/<validos|invalidos>/arquivo.txt`.
4. Para isolar uma categoria com problema, use os alvos específicos como `make test-for` em vez de rodar toda a suíte.

---

## Historico de Versao

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 13/05/26 | Criação da página com seu respectivo conteúdo | Camila Careli |
| 1.1 | 21/06/26 | Atualização da seção de testes | Camila Careli |