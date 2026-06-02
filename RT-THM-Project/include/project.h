/**le langage commun du projet */
#ifndef PROJECT_H
#define PROJECT_H

#include <sys/types.h>

/**
 * Structure ProcessStat : État de santé d'un worker[cite: 27, 31].
 * Conformité MISRA : Utilisation de types explicites[cite: 156].
 */

typedef struct {
    int pid;              /**< PID du processus fils [cite: 28] */
    int health_score;     /**< Score de santé entre 0 et 100 [cite: 29] */
    int tasks_completed;  /**< Nombre de tâches réussies [cite: 30] */
    int command;          /**< Commande reçue du superviseur [cite: 137] */
} ProcessStat;

/** Constantes pour les commandes et l'IPC */
#define CMD_RUN   0
#define CMD_PAUSE 1
#define CMD_STOP  2

#define SHM_KEY 0x1234
#define SEM_KEY 0x5678

#endif
