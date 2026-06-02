/** gère la communication entre processus (mémoire + sécurité) */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/project.h"

int shmid;
int semid;
/**
 * @brief Initialise les ressources IPC (Mémoire partagée et Sémaphores). 
 * @details Cette fonction alloue la mémoire une seule fois au démarrage (Règle MISRA 21.3).
 * @param workers Nombre de processus fils à gérer.
 * @return Un pointeur vers la mémoire partagée.
 */
ProcessStat* init_ipc(int workers) {

    /** 1. Création de la mémoire partagée avec la clé SHM_KEY */ 
    shmid = shmget(SHM_KEY, sizeof(ProcessStat) * workers, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Échec shmget");
        exit(EXIT_FAILURE);
    }

    /**  2. Attachement de la mémoire */
    ProcessStat* stats = (ProcessStat*)shmat(shmid, NULL, 0);
    if (stats == (void*)-1) {
        perror("Échec shmat");
        exit(EXIT_FAILURE);
    }

    /** 3. Création d'un sémaphore binaire (Mutex) */ 
    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Échec semget");
        exit(EXIT_FAILURE);
    }
    
    /** Initialisation du sémaphore à 1 (Libre)*/ 
    semctl(semid, 0, SETVAL, 1);

    return stats;
}

/**
 * Fonctions de synchronisation (P et V) 
 */
void lock_ipc() {
    struct sembuf sb = {0, -1, 0}; /**  Opération P (Wait)*/
    semop(semid, &sb, 1);
}

void unlock_ipc() {
    struct sembuf sb = {0, 1, 0};  /** Opération V (Signal)*/ 
    semop(semid, &sb, 1);
}