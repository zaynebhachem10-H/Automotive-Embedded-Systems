#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "../include/project.h"
#include "../include/config.h"

ProcessStat *shared_stats;
int WORKERS = 3;
int REFRESH_RATE = 2;

// Prototypes externes
ProcessStat* init_ipc(int workers);
void worker_process(int id);

/**
 * Charge la configuration depuis le fichier texte.
 */
void load_config(void) {
    FILE *file = fopen("config.txt", "r");
    if (!file) {
        return; // Utilise les valeurs par défaut si le fichier n'existe pas
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "workers=%d", &WORKERS) == 1) continue;
        if (sscanf(line, "refresh_rate=%d", &REFRESH_RATE) == 1) continue;
    }
    fclose(file);
}

/**
 * Nettoyage et fermeture propre de Ncurses.
 */
void handle_sigint(int sig) {
    (void)sig;
    endwin();
    exit(0);
}

int main(void) {
    signal(SIGINT, handle_sigint);
    
    // Appel de la fonction qui posait problème
    load_config();
    
    shared_stats = init_ipc(WORKERS);

    initscr();
    noecho();
    curs_set(0);

    pid_t pids[100];
    for (int i = 0; i < WORKERS; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            worker_process(i);
        }
        shared_stats[i].command = CMD_RUN;
    }

    while (1) {
        clear();
        mvprintw(1, 2, "=== RT-THM DASHBOARD (Surveillance) ===");
        mvprintw(3, 2, "ID\tPID\tSANTE\tTACHES\tETAT");
        
        for (int i = 0; i < WORKERS; i++) {
            char *status = (shared_stats[i].health_score < 20) ? "ALERTE" : "OK";
            mvprintw(5 + i, 2, "%d\t%d\t%d%%\t%d\t[%s]", 
                     i, shared_stats[i].pid, shared_stats[i].health_score, 
                     shared_stats[i].tasks_completed, status);

            int s;
            if (waitpid(pids[i], &s, WNOHANG) != 0) {
                pids[i] = fork();
                if (pids[i] == 0) worker_process(i);
            }
        }
        refresh();
        sleep(REFRESH_RATE);
    }
    endwin();
    return 0;
}
