
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


// commande ipcs pour voir les semaphores créés
// -1->attente
//
// create an IPC semaphore with the given key
// returns the semaphore id
int create_sem(key_t key) {
    int semid;
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }
    return semid;
}

// ask for user input and seve in the previous semaphore
void set_semaphore(int semid, int value) {
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = value;
    sops.sem_flg = 0;
    if (semop(semid, &sops, 1) == -1) {
        perror("semop");
        exit(1);
    }
}


void * P1(void *arg)
{

    return NULL;
}

int main() {

    return 0;
}