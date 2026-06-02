import re
import os

def check_misra_basics(file_path):
    report = []
    if not os.path.exists(file_path):
        return [f"Erreur : Le fichier {file_path} est introuvable."]
    with open(file_path, 'r') as f:
        content = f.readlines()
        for i, line in enumerate(content):
            # Règle 17.7 : Vérification du retour de fork() [cite: 160, 182]
            if "fork()" in line and "pids[" not in line and "if (" not in line:
                report.append(f"Ligne {i+1}: ALERTE MISRA 17.7 - Valeur de fork() non capturée ou vérifiée.")
            
            # Règle 21.3 : Interdiction de malloc() en runtime [cite: 162, 186]
            if "malloc(" in line:
                report.append(f"Ligne {i+1}: ALERTE MISRA 21.3 - malloc() détecté. Utilisez l'allocation initiale.")

    return report

def generate_report(results):
    with open("misra_interpretation.txt", "w") as f:
        f.write("=== RAPPORT D'INTERPRÉTATION ET DE CONFORMITÉ ===\n")
        if not results:
            f.write("Félicitations : Aucune violation majeure détectée.\n")
        else:
            for error in results:
                f.write(error + "\n")
    print("Audit terminé. Consultez 'misra_interpretation.txt'.")

# Exécution sur le fichier principal
violations = check_misra_basics("src/main.c")
generate_report(violations)