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

# Cores para o terminal
GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
CYAN = "\033[96m"
RESET = "\033[0m"

def run_test(file_path, extra_args=None):
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()

    cmd = [EXEC]
    if extra_args:
        cmd.extend(extra_args)

    result = subprocess.run(
        cmd,
        input=content,
        text=True,
        capture_output=True
    )
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
    failed = 0

    if not os.path.exists(folder):
        print(f"{YELLOW}[AVISO]{RESET} Pasta {folder} não encontrada.")
        return 0, 0

    for file in sorted(os.listdir(folder)):
        path = os.path.join(folder, file)
        
        # Ignora pastas, foca em arquivos .txt
        if os.path.isdir(path): continue

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

        # Lógica de sucesso
        if expect:
            success = has_error # Sucesso em pasta inválida é encontrar o erro
        else:
            success = not has_error # Sucesso em pasta válida é não ter erro

        if success:
            print(f"{GREEN}[OK]{RESET}   {file}")
            passed += 1
        else:
            print(f"{RED}[FAIL]{RESET} {file}")
            # Mostrar o motivo específico do erro
            if err.strip():
                # Tenta extrair apenas a linha do erro para não poluir o terminal
                linhas_erro = [l for l in err.split('\n') if '[' in l or 'error' in l.lower()]
                motivo = " | ".join(linhas_erro) if linhas_erro else err.strip()
                print(f"   {YELLOW}-> Motivo:{RESET} {motivo}")
            else:
                print(f"   {YELLOW}-> Motivo:{RESET} O compilador deveria ter falhado, mas passou.")
            
            if out.strip():
                print(f"   {CYAN}-> Saida Parcial:{RESET} {out.strip()[:50]}...")
            
            print("-" * 50)
            failed += 1

    print(f"Resumo {folder}: {GREEN}{passed} OK{RESET} | {RED}{failed} FAIL{RESET}")
    return passed, failed

if __name__ == "__main__":
    total_passed = 0
    total_failed = 0

    # Estrutura plana conforme sua solicitação
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
    ]

    for pasta in categorias:
        p, f = run_tests(pasta)
        total_passed += p
        total_failed += f

    p, f = run_optimizer_tests()
    total_passed += p
    total_failed += f

    print(f"\nTOTAL: {total_passed} Passou | {total_failed} Falhou")
    if total_failed:
        sys.exit(1)