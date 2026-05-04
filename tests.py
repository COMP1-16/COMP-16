import os
import subprocess

EXEC = "./lib/interpreter" # Certifique-se que o caminho está correto

# Cores para o terminal
GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
CYAN = "\033[96m"
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
    tipo_pasta = "INVÁLIDOS (Esperando Erro)" if expect_error else "VÁLIDOS (Sucesso)"
    print(f"\n{CYAN}--- Testando: {folder} [{tipo_pasta}] ---{RESET}")

    passed = 0
    failed = 0

    if not os.path.exists(folder):
        print(f"{YELLOW}[AVISO]{RESET} Pasta {folder} não encontrada.")
        return 0, 0

    for file in sorted(os.listdir(folder)):
        path = os.path.join(folder, file)
        
        # Ignora pastas, foca em arquivos .txt
        if os.path.isdir(path): continue

        out, err = run_test(path)
        err_lower = err.lower()
        
        # Identifica se houve erro por meio das tags que vocês criaram no C
        has_error = any(tag in err_lower for tag in ["lexico", "sintatico", "semantico", "syntax error", "runtime"])

        # Lógica de sucesso
        if expect_error:
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
                print(f"   {YELLOW}→ Motivo:{RESET} {motivo}")
            else:
                print(f"   {YELLOW}→ Motivo:{RESET} O compilador deveria ter falhado, mas passou.")
            
            if out.strip():
                print(f"   {CYAN}→ Saída Parcial:{RESET} {out.strip()[:50]}...")
            
            print("-" * 50)
            failed += 1

    print(f"Resumo {folder}: {GREEN}{passed} OK{RESET} | {RED}{failed} FAIL{RESET}")
    return passed, failed

if __name__ == "__main__":
    total_passed = 0
    total_failed = 0

    # Estrutura plana conforme sua solicitação
    categorias = [
        "testes/atualizacao/sintatico",
        "testes/atualizacao/semantico",
        "testes/declaracao/sintatico",
        "testes/declaracao/semantico",
        "testes/operadores-aritmeticos/sintatico"
        "testes/operadores-aritmeticos/semantico"
    ]

    for pasta in categorias:
        # Rodamos a pasta diretamente
        p, f = run_tests(pasta, expect_error=False)
        total_passed += p
        total_failed += f

    print(f"\nTOTAL: {total_passed} Passou | {total_failed} Falhou")