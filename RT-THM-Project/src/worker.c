#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "../include/project.h"

extern ProcessStat *shared_stats;
void lock_ipc(void);
void unlock_ipc(void);

// État local du worker
int is_paused = 0;

/**
 * Gestionnaire de signaux pour le Worker.
 * SIGUSR1 : Met le worker en pause.
 * SIGUSR2 : Relance le worker.
 */
void handle_worker_signals(int sig) {
    if (sig == SIGUSR1) is_paused = 1;
    if (sig == SIGUSR2) is_paused = 0;
}

/**
 * Logique principale du processus Worker.
 */
void worker_process(int id) {
    // Initialisation du générateur de nombres aléatoires unique par PID
    srand(time(NULL) ^ (getpid() << 16));
    
    // Configuration des signaux
    signal(SIGUSR1, handle_worker_signals);
    signal(SIGUSR2, handle_worker_signals);

    while (1) {
        if (is_paused) {
            sleep(1); 
            continue; 
        }

        // Simule un temps de travail
        sleep((rand() % 2) + 1); 

        lock_ipc(); // Début Section Critique
        
        // Vérification si le parent demande l'arrêt
        if (shared_stats[id].command == CMD_STOP) {
            unlock_ipc();
            exit(0);
        }

        // Mise à jour des statistiques en mémoire partagée
        shared_stats[id].pid = getpid();
        shared_stats[id].tasks_completed++;
        shared_stats[id].health_score = rand() % 100;
        shared_stats[id].last_update_time = (long)time(NULL);
        
        unlock_ipc(); // Fin Section Critique

        // Simulation d'une erreur critique (Crash)
        if (shared_stats[id].health_score < 5) {
            printf("[WORKER %d] Crash imminent (Santé basse)!\n", id);
            exit(EXIT_FAILURE);
        }
    }
}
