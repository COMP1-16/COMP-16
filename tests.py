import os
import subprocess
import sys

EXEC = "./lib/interpreter" # Certifique-se que o caminho está correto
TEST = "./parser/parser_test"

GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
CYAN = "\033[96m"
RESET = "\033[0m"

def run_test_full(file_path, extra_args=None):
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()

    result = subprocess.run(
        [TEST],
        input=content,
        text=True,
        capture_output=True
    )

def _has_compiler_error(stderr):
    err_lower = stderr.lower()
    return any(tag in err_lower for tag in ["lexico", "sintaxe", "semantico", "syntax error", "runtime"])

def run_test(file_path, extra_args=None):
    result = run_test_full(file_path, extra_args)
    return result.stdout, result.stderr

def run_tests(folder):

    if "validos" in folder:
        expect_error = False

    if "invalidos" in folder:
        expect_error = True

    tipo_pasta = "INVÁLIDOS (Esperando Erro)" if expect_error else "VÁLIDOS (Sucesso)"
    print(f"\n{CYAN}--- Testando: {folder} [{tipo_pasta}] ---{RESET}")

    passed = 0
    failed = 0

    if not os.path.exists(folder):
        print(f"{YELLOW}[AVISO]{RESET} Pasta {folder} nao encontrada.")
        return 0, 0

    for file in sorted(os.listdir(folder)):
        if not file.startswith("valido_") or not file.endswith(".txt"):
            continue

        path = os.path.join(folder, file)
        out_opt, err_opt = run_test(path)
        out_raw, err_raw = run_test(path, extra_args=["--no-opt"])

        err_opt_l = err_opt.lower()
        err_raw_l = err_raw.lower()
        has_error_opt = any(tag in err_opt_l for tag in ["lexico", "sintaxe", "semantico", "syntax error", "runtime"])
        has_error_raw = any(tag in err_raw_l for tag in ["lexico", "sintaxe", "semantico", "syntax error", "runtime"])

        if has_error_opt or has_error_raw:
            print(f"{RED}[FAIL]{RESET} {file}")
            print(f"   {YELLOW}-> Motivo:{RESET} erro em execucao (opt={has_error_opt}, sem_opt={has_error_raw})")
            if err_opt.strip():
                print(f"   {YELLOW}-> stderr opt:{RESET} {err_opt.strip()}")
            if err_raw.strip():
                print(f"   {YELLOW}-> stderr raw:{RESET} {err_raw.strip()}")
            print("-" * 50)
            failed += 1
            continue

        if out_opt == out_raw:
            print(f"{GREEN}[OK]{RESET}   {file}")
            passed += 1
        else:
            print(f"{RED}[FAIL]{RESET} {file}")
            print(f"   {YELLOW}-> Motivo:{RESET} saida difere com e sem otimizacao")
            print(f"   {CYAN}-> com opt:{RESET} {repr(out_opt.strip())}")
            print(f"   {CYAN}-> sem opt:{RESET} {repr(out_raw.strip())}")
            print("-" * 50)
            failed += 1

    print(f"Resumo {folder}: {GREEN}{passed} OK{RESET} | {RED}{failed} FAIL{RESET}")
    return passed, failed

def run_tests(folder, expect_error=False):
    print(f"\n{CYAN}--- Testando: {folder} ---{RESET}")

    passed = 0
    failed = 0

    if not os.path.exists(folder):
        print(f"{YELLOW}[AVISO]{RESET} Pasta {folder} não encontrada.")
        return 0, 0, 0

    for file in sorted(os.listdir(folder)):
        path = os.path.join(folder, file)
        
        # Ignora pastas e arquivos que nao sao casos de teste (.txt)
        if os.path.isdir(path) or not file.endswith(".txt"):
            continue

        if file.startswith("valido_"):
            expect = False
        elif file.startswith("invalido_"):
            expect = True
        else:
            continue

        out, err = run_test(path)
        err_lower = err.lower()
        
        # Identifica se houve erro por meio das tags que vocês criaram no C
        has_error = any(tag in err_lower for tag in ["lexico", "sintaxe", "semantico", "syntax error", "runtime"])

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

    fail_total = failed
    print(f"Resumo {folder}: {GREEN}{passed} OK{RESET} | {RED}{fail_total} FAIL{RESET}")
    return passed, 0, failed

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
        "testes/atualizacao_variaveis/semantico",
        "testes/atualizacao_variaveis/valido_auto_inc",
        "testes/declaracao_variaveis/sintatico",
        "testes/declaracao_variaveis/semantico",
        "testes/operadores_aritmeticos/sintatico",
        "testes/operadores_aritmeticos/semantico",
        "testes/operadores_logicos/sintatico",
        "testes/operadores_logicos/semantico",
        "testes/for/sintatico",
        "testes/for/semantico",
        "testes/while/sintatico",
        "testes/while/semantico",
        "testes/if_else/sintatico",
        "testes/if_else/semantico",
        "testes/recursao",
        "testes/math/sintatico",
        "testes/math/semantico",
        "testes/stdlib/sintatico",
        "testes/stdlib/semantico",
        "testes/switch_case/sintatico/invalidos",
        "testes/switch_case/sintatico/validos",
    ]

    if len(sys.argv) > 1:
        categorias = sys.argv[1:]

    run_full_suite = len(sys.argv) == 1
    run_math_execucao = run_full_suite or any("testes/math" in pasta for pasta in categorias)
    run_stdlib_execucao = run_full_suite or any("testes/stdlib" in pasta for pasta in categorias)

    for pasta in categorias:
        # Rodamos a pasta diretamente
        p, f = run_tests(pasta)
        total_passed += p
        total_failed += f
        total_regressoes += f

    print(f"\nTOTAL: {total_passed} Passou | {total_failed} Falhou")
    if total_regressoes:
        sys.exit(1)