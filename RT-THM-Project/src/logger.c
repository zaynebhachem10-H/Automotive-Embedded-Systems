/**  écrit les événements du système dans un fichier log /historique du système (logs précis + traçabilité)*/
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

void log_event(const char *message) {
    FILE *f = fopen("system.log", "a");
    if (f == NULL) return;

    struct timeval tv;
    gettimeofday(&tv, NULL); /**  Précision microseconde */
    
    struct tm *tm_info = localtime(&tv.tv_sec);
    char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    /** Format: [Date Heure.micro] [PID] Message */
    fprintf(f, "[%s.%06ld] [PID %d] %s\n", buffer, tv.tv_usec, getpid(), message);

    fclose(f);
}