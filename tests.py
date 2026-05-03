import os
import subprocess

EXEC = "lib/interpreter"

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
            success = (
                        "Lexico" in err.lower() or
                        "Sintatico" in err.lower() or
                        "Semantico" in err.lower() or
                        "syntax error" in err.lower() or
                        "erro sintático" in err.lower()
            )
        else:
            success = (
                        "Lexico" not in err.lower() and
                        "Sintatico" not in err.lower() and
                        "Semantico" not in err.lower() and
                        "syntax error" not in err.lower() and
                        "erro sintático" not in err.lower()
            )

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

    p, f = run_tests("tests/valid", expect_error=False)
    total_passed += p
    total_failed += f

    p, f = run_tests("tests/invalid", expect_error=True)
    total_passed += p
    total_failed += f

    print("\n=== TOTAL ===")
    print(f"{GREEN}Passou: {total_passed}{RESET}")
    print(f"{RED}Falhou: {total_failed}{RESET}")