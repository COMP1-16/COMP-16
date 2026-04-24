import os
import subprocess

EXEC = "./interpretador"

# cores
GREEN = "\033[92m"
RED = "\033[91m"
RESET = "\033[0m"


def run_test(file_path):
    with open(file_path, "r") as f:
        content = f.read()

    result = subprocess.run(
        [EXEC],
        input=content,
        text=True,
        capture_output=True
    )

    return result.stdout, result.stderr


def run_tests(folder, expect_error=False):
    print(f"\n--- Testando: {folder} ---")

    passed = 0
    failed = 0

    for file in sorted(os.listdir(folder)):
        path = os.path.join(folder, file)

        out, err = run_test(path)

        if expect_error:
            success = "Erro sintático" in err
        else:
            success = "Erro sintático" not in err

        if success:
            print(f"{GREEN}[OK]{RESET}   {file}")
            passed += 1
        else:
            print(f"{RED}[FAIL]{RESET} {file}")
            print("  → Saída:", out.strip())
            print("  → Erro:", err.strip())
            print("-" * 30)
            failed += 1

    print(f"\nResumo {folder}: {GREEN}{passed} OK{RESET} | {RED}{failed} FAIL{RESET}")

    return passed, failed


if __name__ == "__main__":
    total_passed = 0
    total_failed = 0

    p, f = run_tests("tests/operadores-aritmeticos/validos", expect_error=False)
    total_passed += p
    total_failed += f

    p, f = run_tests("tests/operadores-aritmeticos/invalidos", expect_error=True)
    total_passed += p
    total_failed += f

    p, f = run_tests("tests/declaracao/validos", expect_error=False)
    total_passed += p
    total_failed += f

    p, f = run_tests("tests/declaracao/invalidos", expect_error=True)
    total_passed += p
    total_failed += f

    p, f = run_tests("tests/declaracao/questao-semantica", expect_error=False)
    total_passed += p
    total_failed += f

    print("\n=== TOTAL ===")
    print(f"{GREEN}Passou: {total_passed}{RESET}")
    print(f"{RED}Falhou: {total_failed}{RESET}")