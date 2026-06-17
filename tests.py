import os
import subprocess
import sys

def _resolve_exec():
    candidates = [
        "./lib/interpreter",
        "lib/interpreter",
        "./lib/interpreter.exe",
        "lib/interpreter.exe",
    ]
    for path in candidates:
        if os.path.isfile(path):
            return path
    return "./lib/interpreter"

EXEC = _resolve_exec()

GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
CYAN = "\033[96m"
RESET = "\033[0m"

def run_test_full(file_path, extra_args=None):
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()

    cmd = [EXEC]
    if extra_args:
        cmd.extend(extra_args)

    return subprocess.run(
        cmd,
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

def run_optimizer_tests(folder="testes/otimizador"):
    print(f"\n{CYAN}--- Testando equivalencia do otimizador: {folder} ---{RESET}")

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
    fail_esperado = 0
    fail_regressao = 0

    if not os.path.exists(folder):
        print(f"{YELLOW}[AVISO]{RESET} Pasta {folder} não encontrada.")
        return 0, 0, 0

    if os.path.isfile(folder):
        arquivos = [os.path.basename(folder)]
        pasta = os.path.dirname(folder) or "."
    else:
        arquivos = sorted(os.listdir(folder))
        pasta = folder

    for file in arquivos:
        path = os.path.join(pasta, file)

        if os.path.isdir(path):
            continue

        if file.endswith(".expected"):
            txt = file[:-9] + ".txt"
            if not os.path.isfile(os.path.join(pasta, txt)):
                print(f"{RED}[FAIL]{RESET} {file}")
                print(f"   {YELLOW}-> Motivo:{RESET} arquivo .expected sem par valido_*.txt correspondente")
                print("-" * 50)
                fail_regressao += 1
            continue

        if file.endswith(".exitcode"):
            txt = file[:-9] + ".txt"
            if not os.path.isfile(os.path.join(pasta, txt)):
                print(f"{RED}[FAIL]{RESET} {file}")
                print(f"   {YELLOW}-> Motivo:{RESET} arquivo .exitcode sem par valido_*.txt correspondente")
                print("-" * 50)
                fail_regressao += 1
            continue

        if not file.endswith(".txt"):
            continue

        if file.startswith("valido_"):
            expect = False
        elif file.startswith("invalido_"):
            expect = True
        else:
            continue

        result = run_test_full(path)
        out, err = result.stdout, result.stderr
        has_error = _has_compiler_error(err)

        if expect:
            if has_error:
                print(f"{RED}[FAIL]{RESET}   {file}")
                fail_esperado += 1
            else:
                print(f"{RED}[FAIL]{RESET} {file}")
                print(f"   {YELLOW}-> Motivo:{RESET} O compilador deveria ter falhado, mas passou.")
                if out.strip():
                    print(f"   {CYAN}-> Saida Parcial:{RESET} {out.strip()[:50]}...")
                print("-" * 50)
                fail_regressao += 1
        elif not has_error:
            falhas_extra = []

            expected_path = os.path.join(pasta, file[:-4] + ".expected")
            if os.path.isfile(expected_path):
                with open(expected_path, "r", encoding="utf-8") as f:
                    expected = f.read()
                if out != expected:
                    falhas_extra.append("saida difere do .expected")
                    print(f"{RED}[FAIL]{RESET} {file} (.expected)")
                    print(f"   {CYAN}-> obtido:{RESET} {repr(out)}")
                    print(f"   {CYAN}-> esperado:{RESET} {repr(expected)}")
                    print("-" * 50)

            exitcode_path = os.path.join(pasta, file[:-4] + ".exitcode")
            if os.path.isfile(exitcode_path):
                with open(exitcode_path, "r", encoding="utf-8") as f:
                    expected_code = int(f.read().strip())
                if result.returncode != expected_code:
                    falhas_extra.append("exit code difere do .exitcode")
                    print(f"{RED}[FAIL]{RESET} {file} (.exitcode)")
                    print(f"   {CYAN}-> obtido:{RESET} {result.returncode}")
                    print(f"   {CYAN}-> esperado:{RESET} {expected_code}")
                    print("-" * 50)

            if falhas_extra:
                fail_regressao += 1
            else:
                extras = []
                if os.path.isfile(expected_path):
                    extras.append(".expected")
                if os.path.isfile(exitcode_path):
                    extras.append(f".exitcode={result.returncode}")
                sufixo = f" ({', '.join(extras)})" if extras else ""
                print(f"{GREEN}[OK]{RESET}   {file}{sufixo}")
                passed += 1
        else:
            print(f"{RED}[FAIL]{RESET} {file}")
            linhas_erro = [l for l in err.split('\n') if '[' in l or 'error' in l.lower()]
            motivo = " | ".join(linhas_erro) if linhas_erro else err.strip()
            print(f"   {YELLOW}-> Motivo:{RESET} {motivo}")
            if out.strip():
                print(f"   {CYAN}-> Saida Parcial:{RESET} {out.strip()[:50]}...")
            print("-" * 50)
            fail_regressao += 1

    fail_total = fail_esperado + fail_regressao
    print(f"Resumo {folder}: {GREEN}{passed} OK{RESET} | {RED}{fail_total} FAIL{RESET}")
    return passed, fail_esperado, fail_regressao

if __name__ == "__main__":
    total_passed = 0
    total_failed = 0
    total_regressoes = 0

    categorias = [
        "testes/atualizacao_variaveis/semantico",
        "testes/atualizacao_variaveis/valido_auto_inc.txt",
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
        "testes/ctype/sintatico",
        "testes/ctype/semantico",
    ]

    if len(sys.argv) > 1:
        categorias = sys.argv[1:]

    run_full_suite = len(sys.argv) == 1
    run_math_execucao = run_full_suite or any("testes/math" in pasta for pasta in categorias)
    run_stdlib_execucao = run_full_suite or any("testes/stdlib" in pasta for pasta in categorias)
    run_ctype_execucao = run_full_suite or any("testes/ctype" in pasta for pasta in categorias)

    for pasta in categorias:
        p, fe, r = run_tests(pasta)
        total_passed += p
        total_failed += fe + r
        total_regressoes += r

    if run_math_execucao:
        p, fe, r = run_tests("testes/math/execucao")
        total_passed += p
        total_failed += fe + r
        total_regressoes += r

    if run_stdlib_execucao:
        p, fe, r = run_tests("testes/stdlib/execucao")
        total_passed += p
        total_failed += fe + r
        total_regressoes += r

    if run_ctype_execucao:
        p, fe, r = run_tests("testes/ctype/execucao")
        total_passed += p
        total_failed += fe + r
        total_regressoes += r

    if run_full_suite:
        p, f = run_optimizer_tests()
        total_passed += p
        total_failed += f
        total_regressoes += f

    print(f"\nTOTAL: {total_passed} Passou | {total_failed} Falhou")
    if total_regressoes:
        sys.exit(1)