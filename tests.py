import os
import subprocess
import sys

EXEC = "./lib/interpreter.exe" if os.path.isfile("./lib/interpreter.exe") else "./lib/interpreter"

SKIP_TESTS = {
    "invalido_recursao_infinita.txt",
}

FEATURES = [
    "atualizacao_variaveis",
    "declaracao_variaveis",
    "for",
    "if_else",
    "while",
    "operadores_aritmeticos",
    "operadores_logicos",
    "math",
    "stdlib",
    "ctype",
    "switch_case",
    "recursao",
    "otimizador",
]

LAYERS = ("sintatico", "semantico", "execucao")
KINDS = ("validos", "invalidos")

TIMEOUT_VALID = 15
TIMEOUT_INVALID = 3

GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
CYAN = "\033[96m"
RESET = "\033[0m"


def all_test_folders():
    folders = []
    for feature in FEATURES:
        for layer in LAYERS:
            for kind in KINDS:
                folders.append(f"testes/{feature}/{layer}/{kind}")
    return folders


def _norm(folder):
    return folder.replace("\\", "/").rstrip("/")


def _layer(folder):
    norm = _norm(folder)
    for layer in LAYERS:
        if f"/{layer}/" in norm:
            return layer
    return None


def _kind(folder):
    norm = _norm(folder)
    if norm.endswith("/validos"):
        return "validos"
    if norm.endswith("/invalidos"):
        return "invalidos"
    return None


def _check_naming(folder, file):
    kind = _kind(folder)
    if kind == "validos" and not file.startswith("valido_"):
        return "arquivo em validos/ deve comecar com valido_"
    if kind == "invalidos" and not file.startswith("invalido_"):
        return "arquivo em invalidos/ deve comecar com invalido_"
    return None


def _has_syntax_error(stderr):
    err_lower = stderr.lower()
    return "sintaxe" in err_lower or "syntax error" in err_lower


def _has_semantic_error(stderr):
    return "[semantico] erro:" in stderr.lower()


def _has_lexico_or_syntax_error(stderr):
    err_lower = stderr.lower()
    return any(tag in err_lower for tag in ["lexico", "sintaxe", "syntax error"])


def _has_compiler_error(stderr, syntax_only=False):
    if syntax_only:
        return _has_syntax_error(stderr)
    err_lower = stderr.lower()
    return any(tag in err_lower for tag in ["lexico", "sintaxe", "semantico", "syntax error", "runtime"])


def _test_flags(folder, file):
    layer = _layer(folder)

    if layer == "sintatico":
        return True, False
    if layer == "semantico":
        return False, True
    return False, False


def run_test_full(file_path, syntax_only=False, semantic_only=False, timeout=TIMEOUT_VALID):
    cmd = [EXEC, file_path]
    if syntax_only:
        cmd.append("--parse-only")
    elif semantic_only:
        cmd.append("--semantic-only")
    return subprocess.run(
        cmd,
        text=True,
        capture_output=True,
        timeout=timeout,
    )


def run_test(file_path, syntax_only=False, semantic_only=False, timeout=TIMEOUT_VALID):
    try:
        result = run_test_full(
            file_path,
            syntax_only=syntax_only,
            semantic_only=semantic_only,
            timeout=timeout,
        )
        return result.stdout, result.stderr, result.returncode
    except subprocess.TimeoutExpired:
        return "", "[Runtime] Erro: tempo limite excedido\n", 1


def run_output_tests(folder, cached_results=None):
    print(f"\n{CYAN}--- Testando saida esperada: {folder} ---{RESET}")

    passed = 0
    failed = 0
    cached_results = cached_results or {}

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

        if file in cached_results:
            out, err, _ = cached_results[file]
        else:
            out, err, _ = run_test(path)

        if _has_compiler_error(err):
            print(f"{RED}[FAIL]{RESET} {file}")
            linhas_erro = [l for l in err.split("\n") if "[" in l or "error" in l.lower()]
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


def run_exitcode_tests(folder, cached_results=None):
    print(f"\n{CYAN}--- Testando codigo de saida (exit): {folder} ---{RESET}")

    passed = 0
    failed = 0
    cached_results = cached_results or {}

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

        if file in cached_results:
            out, err, returncode = cached_results[file]
        else:
            out, err, returncode = run_test(path)

        if _has_compiler_error(err):
            print(f"{RED}[FAIL]{RESET} {file}")
            linhas_erro = [l for l in err.split("\n") if "[" in l or "error" in l.lower()]
            motivo = " | ".join(linhas_erro) if linhas_erro else err.strip()
            print(f"   {YELLOW}-> Motivo:{RESET} {motivo}")
            print("-" * 50)
            failed += 1
            continue

        if returncode == expected:
            print(f"{GREEN}[OK]{RESET}   {file} (exit={expected})")
            passed += 1
        else:
            print(f"{RED}[FAIL]{RESET} {file}")
            print(f"   {YELLOW}-> Motivo:{RESET} codigo de saida difere do esperado")
            print(f"   {CYAN}-> obtido:{RESET} {returncode}")
            print(f"   {CYAN}-> esperado:{RESET} {expected}")
            print("-" * 50)
            failed += 1

    print(f"Resumo {folder}: {GREEN}{passed} OK{RESET} | {RED}{failed} FAIL{RESET}")
    return passed, failed


def run_tests(folder):
    print(f"\n{CYAN}--- Testando: {folder} ---{RESET}")

    passed = 0
    failed = 0
    norm = _norm(folder)
    is_exec_validos = norm.endswith("/execucao/validos")
    cached_results = {}

    if not os.path.exists(folder):
        print(f"{YELLOW}[AVISO]{RESET} Pasta {folder} nao encontrada.")
        return 0, 0

    for file in sorted(os.listdir(folder)):
        path = os.path.join(folder, file)

        if os.path.isdir(path) or not file.endswith(".txt"):
            continue

        if file in SKIP_TESTS:
            continue

        naming_error = _check_naming(folder, file)
        if naming_error:
            print(f"{RED}[FAIL]{RESET} {file}")
            print(f"   {YELLOW}-> Motivo:{RESET} {naming_error}")
            print("-" * 50)
            failed += 1
            continue

        if file.startswith("valido_"):
            expect = False
        elif file.startswith("invalido_"):
            expect = True
        else:
            continue

        syntax_only, semantic_only = _test_flags(folder, file)
        timeout = TIMEOUT_INVALID if expect else TIMEOUT_VALID
        is_semantico_invalidos = norm.endswith("/semantico/invalidos")

        out, err, returncode = run_test(
            path,
            syntax_only=syntax_only,
            semantic_only=semantic_only,
            timeout=timeout,
        )
        has_error = _has_compiler_error(err, syntax_only=syntax_only)

        if is_exec_validos and not expect:
            cached_results[file] = (out, err, returncode)

        if expect:
            if is_semantico_invalidos:
                if _has_semantic_error(err):
                    print(f"{GREEN}[OK]{RESET}   {file}")
                    passed += 1
                elif _has_lexico_or_syntax_error(err):
                    print(f"{RED}[FAIL]{RESET} {file}")
                    print(f"   {YELLOW}-> Motivo:{RESET} erro lexico/sintatico em semantico/invalidos")
                    print("-" * 50)
                    failed += 1
                elif has_error:
                    print(f"{RED}[FAIL]{RESET} {file}")
                    linhas_erro = [l for l in err.split("\n") if "[" in l or "error" in l.lower()]
                    motivo = " | ".join(linhas_erro) if linhas_erro else err.strip()
                    print(f"   {YELLOW}-> Motivo:{RESET} {motivo}")
                    print("-" * 50)
                    failed += 1
                else:
                    print(f"{RED}[FAIL]{RESET} {file}")
                    print(f"   {YELLOW}-> Motivo:{RESET} O compilador deveria ter falhado, mas passou.")
                    if out.strip():
                        print(f"   {CYAN}-> Saida Parcial:{RESET} {out.strip()[:50]}...")
                    print("-" * 50)
                    failed += 1
            elif has_error:
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
            linhas_erro = [l for l in err.split("\n") if "[" in l or "error" in l.lower()]
            motivo = " | ".join(linhas_erro) if linhas_erro else err.strip()
            print(f"   {YELLOW}-> Motivo:{RESET} {motivo}")
            if out.strip():
                print(f"   {CYAN}-> Saida Parcial:{RESET} {out.strip()[:50]}...")
            print("-" * 50)
            failed += 1

    if is_exec_validos:
        p_out, f_out = run_output_tests(folder, cached_results)
        passed += p_out
        failed += f_out
        p_exit, f_exit = run_exitcode_tests(folder, cached_results)
        passed += p_exit
        failed += f_exit

    print(f"Resumo {folder}: {GREEN}{passed} OK{RESET} | {RED}{failed} FAIL{RESET}")
    return passed, failed


if __name__ == "__main__":
    total_passed = 0
    total_failed = 0

    categorias = all_test_folders()

    if len(sys.argv) > 1:
        categorias = sys.argv[1:]

    for pasta in categorias:
        p, f = run_tests(pasta)
        total_passed += p
        total_failed += f

    print(f"\nTOTAL: {total_passed} Passou | {total_failed} Falhou")
    if total_failed:
        sys.exit(1)
