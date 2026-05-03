import os
import subprocess
import sys

EXEC = "./interpretador"

GREEN  = "\033[92m"
RED    = "\033[91m"
RESET  = "\033[0m"

def run_test(file_path):
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()
    result = subprocess.run([EXEC], input=content, text=True, capture_output=True)
    return result.stdout, result.stderr, result.returncode

def tem_erro(stderr, returncode):
    stderr = stderr.lower()

    if "erro sint" in stderr: return True
    if "erro semant" in stderr: return True
    if "aviso semant" in stderr: return True
    if returncode != 0: return True

    return False

def run_tests(folder, expect_error):
    if not os.path.isdir(folder):
        return 0, 0

    arquivos = sorted(f for f in os.listdir(folder) if os.path.isfile(os.path.join(folder, f)))
    if not arquivos:
        return 0, 0

    print(f"\n--- {'[ERRO]' if expect_error else '[OK]'} {folder} ---")
    passed = failed = 0

    for filename in arquivos:
        path = os.path.join(folder, filename)
        out, err, code = run_test(path)

        success = tem_erro(err, code) if expect_error else not tem_erro(err, code)

        if success:
            print(f"  {GREEN}[OK]{RESET}   {filename}")
            if expect_error and err.strip():
                print(f"         stderr : {err.strip()[:200]}")
            passed += 1
        else:
            print(f"  {RED}[FAIL]{RESET} {filename}")
            if err.strip():
                print(f"         stderr : {err.strip()[:200]}")
            print(f"         exit   : {code}")
            failed += 1

    print(f"  Resumo: {GREEN}{passed} OK{RESET} | {RED}{failed} FAIL{RESET}")
    return passed, failed


SUITES = [
    ("testes/declaracao/validos",                         False),
    ("testes/declaracao/invalidos",                       True),
    ("testes/declaracao/questao-semantica",               True),

    ("testes/atualizacao/validos",                        False),
    ("testes/atualizacao/invalidos",                      True),
    ("testes/atualizacao/semantico/validos",              False),
    ("testes/atualizacao/semantico/invalidos",            True),

    ("testes/operadores-aritmeticos/validos",             False),
    ("testes/operadores-aritmeticos/invalidos",           True),
    ("testes/operadores-aritmeticos/semantico/validos",   False),
    ("testes/operadores-aritmeticos/semantico/invalidos", True),
]


if __name__ == "__main__":
    if not os.path.isfile(EXEC):
        print(f"{RED}Executável '{EXEC}' não encontrado.{RESET}")
        sys.exit(1)

    total = {
        "sintatico": {"ok": 0, "fail": 0},
        "semantico": {"ok": 0, "fail": 0}
    }

    for folder, expect_error in SUITES:
        p, f = run_tests(folder, expect_error)

        # classifica pelo nome da pasta
        tipo = "semantico" if "semantico" in folder else "sintatico"

        total[tipo]["ok"] += p
        total[tipo]["fail"] += f

    # resumo final
    print("\n=== RESUMO FINAL ===")

    print(f"\n[SINTÁTICO]")
    print(f"{GREEN}{total['sintatico']['ok']} OK{RESET} | {RED}{total['sintatico']['fail']} FAIL{RESET}")

    print(f"\n[SEMÂNTICO]")
    print(f"{GREEN}{total['semantico']['ok']} OK{RESET} | {RED}{total['semantico']['fail']} FAIL{RESET}")

    total_ok = total["sintatico"]["ok"] + total["semantico"]["ok"]
    total_fail = total["sintatico"]["fail"] + total["semantico"]["fail"]

    print(f"\n=== TOTAL GERAL: {GREEN}{total_ok} OK{RESET} | {RED}{total_fail} FAIL{RESET} ===")

    sys.exit(0 if total_fail == 0 else 1)