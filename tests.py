import os
import subprocess
import sys

EXEC = "./lib/interpreter.exe" if os.path.isfile("./lib/interpreter.exe") else "./lib/interpreter"

# Stack overflow em recursao infinita e erro de runtime, nao de compilacao
SKIP_TESTS = {
    "invalido_recursao_infinita.txt",
}

GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
CYAN = "\033[96m"
RESET = "\033[0m"

def _is_syntax_only_folder(folder):
    norm = folder.replace("\\", "/").rstrip("/")
    return norm.endswith("/sintatico") or "/sintatico/" in norm or norm.endswith("valido_auto_inc")

def _is_semantic_only_folder(folder):
    norm = folder.replace("\\", "/").rstrip("/")
    return norm.endswith("/semantico")

def _has_syntax_error(stderr):
    err_lower = stderr.lower()
    return "sintaxe" in err_lower or "syntax error" in err_lower

def _has_compiler_error(stderr, syntax_only=False):
    if syntax_only:
        return _has_syntax_error(stderr)
    err_lower = stderr.lower()
    return any(tag in err_lower for tag in ["lexico", "sintaxe", "semantico", "syntax error", "runtime"])

def run_test_full(file_path, extra_args=None, syntax_only=False, semantic_only=False):
    cmd = [EXEC, file_path]
    if syntax_only:
        cmd.append("--parse-only")
    elif semantic_only:
        cmd.append("--semantic-only")
    if extra_args:
        cmd.extend(extra_args)
    return subprocess.run(
        cmd,
        text=True,
        capture_output=True,
        timeout=15,
    )

def run_test(file_path, extra_args=None, syntax_only=False, semantic_only=False):
    try:
        result = run_test_full(file_path, extra_args, syntax_only=syntax_only, semantic_only=semantic_only)
        return result.stdout, result.stderr
    except subprocess.TimeoutExpired:
        return "", "[Runtime] Erro: tempo limite excedido\n"

def run_tests(folder, expect_error=False):
    print(f"\n{CYAN}--- Testando: {folder} ---{RESET}")

    passed = 0
    failed = 0
    syntax_only = _is_syntax_only_folder(folder)
    semantic_only = _is_semantic_only_folder(folder)

    if not os.path.exists(folder):
        print(f"{YELLOW}[AVISO]{RESET} Pasta {folder} não encontrada.")
        return 0, 0

    for file in sorted(os.listdir(folder)):
        path = os.path.join(folder, file)
        
        # Ignora pastas e arquivos que nao sao casos de teste (.txt)
        if os.path.isdir(path) or not file.endswith(".txt"):
            continue

        if file in SKIP_TESTS:
            continue

        if file.startswith("valido_"):
            expect = False
        elif file.startswith("invalido_"):
            expect = True
        else:
            continue

        if file.startswith("invalido_"):
            out, err = run_test(path)
        else:
            out, err = run_test(path, syntax_only=syntax_only, semantic_only=semantic_only)
        has_error = _has_compiler_error(err, syntax_only=syntax_only)

        if expect:
            if has_error:
                print(f"{GREEN}[OK]{RESET}   {file}")
                passed += 1
            else:
                print(f"{RED}[FAIL]{RESET} {file}")
                print(f"   {YELLOW}-> Motivo:{RESET} O compilador deveria ter falhado, mas passou.")
                if out.strip():
                    print(f"   {CYAN}-> Saida Parcial:{RESET} {out.strip()[:50]}...")
                print("-" * 50)
                failed += 1
        elif not has_error:
            print(f"{GREEN}[OK]{RESET}   {file}")
            passed += 1
        else:
            print(f"{RED}[FAIL]{RESET} {file}")
            linhas_erro = [l for l in err.split('\n') if '[' in l or 'error' in l.lower()]
            motivo = " | ".join(linhas_erro) if linhas_erro else err.strip()
            print(f"   {YELLOW}-> Motivo:{RESET} {motivo}")
            if out.strip():
                print(f"   {CYAN}-> Saida Parcial:{RESET} {out.strip()[:50]}...")
            print("-" * 50)
            failed += 1
    
    if folder.endswith("/execucao"):
        passed_output, failed_output = run_output_tests(folder)
        passed += passed_output
        failed += failed_output

    fail_total = failed
    print(f"Resumo {folder}: {GREEN}{passed} OK{RESET} | {RED}{fail_total} FAIL{RESET}")
    return passed, fail_total

def run_output_tests(folder):
    print(f"\n{CYAN}--- Testando saida esperada: {folder} ---{RESET}")

    passed = 0
    failed = 0

    if not os.path.exists(folder):
        print(f"{YELLOW}[AVISO]{RESET} Pasta {folder} nao encontrada.")
        return 0, 0

    for file in sorted(os.listdir(folder)):
        if not file.startswith("valido_") or not file.endswith(".txt"):
            continue

        path = os.path.join(folder, file)
        expected_path = os.path.join(folder, file[:-4] + ".expected")
        if not os.path.isfile(expected_path):
            continue

        with open(expected_path, "r", encoding="utf-8") as f:
            expected = f.read()

        out, err = run_test(path)
        err_lower = err.lower()
        has_error = any(tag in err_lower for tag in ["lexico", "sintaxe", "semantico", "syntax error", "runtime"])

        if has_error:
            print(f"{RED}[FAIL]{RESET} {file}")
            linhas_erro = [l for l in err.split('\n') if '[' in l or 'error' in l.lower()]
            motivo = " | ".join(linhas_erro) if linhas_erro else err.strip()
            print(f"   {YELLOW}-> Motivo:{RESET} {motivo}")
            print("-" * 50)
            failed += 1
            continue

        if out == expected:
            print(f"{GREEN}[OK]{RESET}   {file}")
            passed += 1
        else:
            print(f"{RED}[FAIL]{RESET} {file}")
            print(f"   {YELLOW}-> Motivo:{RESET} saida difere do esperado")
            print(f"   {CYAN}-> obtido:{RESET} {repr(out)}")
            print(f"   {CYAN}-> esperado:{RESET} {repr(expected)}")
            print("-" * 50)
            failed += 1

    print(f"Resumo {folder}: {GREEN}{passed} OK{RESET} | {RED}{failed} FAIL{RESET}")
    return passed, failed

def run_exitcode_tests(folder):
    print(f"\n{CYAN}--- Testando codigo de saida (exit): {folder} ---{RESET}")

    passed = 0
    failed = 0

    if not os.path.exists(folder):
        print(f"{YELLOW}[AVISO]{RESET} Pasta {folder} nao encontrada.")
        return 0, 0

    for file in sorted(os.listdir(folder)):
        if not file.startswith("valido_") or not file.endswith(".txt"):
            continue

        path = os.path.join(folder, file)
        exitcode_path = os.path.join(folder, file[:-4] + ".exitcode")
        if not os.path.isfile(exitcode_path):
            continue

        with open(exitcode_path, "r", encoding="utf-8") as f:
            expected = int(f.read().strip())

        result = run_test_full(path)

        if _has_compiler_error(result.stderr):
            print(f"{RED}[FAIL]{RESET} {file}")
            linhas_erro = [l for l in result.stderr.split('\n') if '[' in l or 'error' in l.lower()]
            motivo = " | ".join(linhas_erro) if linhas_erro else result.stderr.strip()
            print(f"   {YELLOW}-> Motivo:{RESET} {motivo}")
            print("-" * 50)
            failed += 1
            continue

        if result.returncode == expected:
            print(f"{GREEN}[OK]{RESET}   {file} (exit={expected})")
            passed += 1
        else:
            print(f"{RED}[FAIL]{RESET} {file}")
            print(f"   {YELLOW}-> Motivo:{RESET} codigo de saida difere do esperado")
            print(f"   {CYAN}-> obtido:{RESET} {result.returncode}")
            print(f"   {CYAN}-> esperado:{RESET} {expected}")
            print("-" * 50)
            failed += 1

    print(f"Resumo {folder}: {GREEN}{passed} OK{RESET} | {RED}{failed} FAIL{RESET}")
    return passed, failed

if __name__ == "__main__":
    total_passed = 0
    total_failed = 0
    total_regressoes = 0

    # Estrutura plana conforme sua solicitação
    categorias = [
        # "testes/atualizacao_variaveis/semantico",
        # "testes/atualizacao_variaveis/valido_auto_inc",
        # "testes/declaracao_variaveis/sintatico",
        # "testes/declaracao_variaveis/semantico",
        # "testes/operadores_aritmeticos/sintatico",
        # "testes/operadores_aritmeticos/semantico",
        # "testes/operadores_logicos/sintatico",
        # "testes/operadores_logicos/semantico",
        # "testes/for/sintatico",
        # "testes/for/semantico",
        # "testes/while/sintatico",
        # "testes/while/semantico",
        # "testes/if_else/sintatico",
        # "testes/if_else/semantico",
        "testes/switch_case/sintatico/validos",
        "testes/switch_case/sintatico/invalidos",
        "testes/switch_case/semantico",
        "testes/switch_case/execucao",
        # "testes/recursao",
        # "testes/otimizador",
        # "testes/math/sintatico",
        # "testes/math/semantico",
        # "testes/math/execucao",
        # "testes/stdlib/sintatico",
        # "testes/stdlib/semantico",
        # "testes/stdlib/execucao",
    ]

    if len(sys.argv) > 1:
        categorias = sys.argv[1:]

    for pasta in categorias:
        # Rodamos a pasta diretamente
        p, f = run_tests(pasta)
        total_passed += p
        total_failed += f
        total_regressoes += f

    print(f"\nTOTAL: {total_passed} Passou | {total_failed} Falhou")
    if total_regressoes:
        sys.exit(1)