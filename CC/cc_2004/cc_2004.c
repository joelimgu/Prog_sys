
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define KEY 48
#define KEY_SHM 30

// see with ipcs
int main() {
    char * processus[] = {"bricomette", "terrasse", "camion", "monocolage"};
    key_t key = 50;
    int sem = semget(key,1, 0666|IPC_CREAT ); // key, nb_of_sem, droits|creer le se if not exist
    if ( sem == -1 ) {
        printf("error creating sem\n");
        exit(-1);
    }
    if (semctl(sem,0, SETVAL, 1) == -1) {
        printf("error1\n");
        exit(-1);
    }
    struct sembuf bufP = {.sem_op=-1,.sem_num=0,.sem_flg=0};
    struct sembuf bufV = {.sem_op=+1,.sem_num=0,.sem_flg=0};

    if (semop(sem,&bufV, 1) == -1 ) {
        perror("pbl semV");
        exit(-1);
    }

    if (semop(sem,&bufP, 1) == -1 ) { // blocant jusq'à liberé comme mutex
        printf("pbl semP\n");
        exit(-1);
    }
    key_t key_shm = 11;
    int shm_id = shmget(key_shm, sizeof(int)*4, 0666|IPC_CREAT);
    if ( shm_id == -1) {
        perror("error shm");
        exit(-1);
    }

    int * shm_pids = shmat(shm_id, NULL,0);
    if (shm_pids == (void*)-1) {
        perror("err shmat");
        exit(-1);
    }
    printf("addr: %p\n",shm_pids);

    for (int i = 0; i < 4; i++) {
        int id = fork();
        switch (id) {
            case-1:
                printf("errur dans la creation de processus fils\n");
                exit(-1);

            case 0:
                if (execl(processus[i],processus[i], NULL) == -1) {
                    printf("cant find the file\n");
                    exit(-1);
                }
                break;
            default:
                shm_pids[i] = id;
        }
    }
    if (semop(sem,&bufV, 1) == -1 ) {
        printf("pbl semP");
        exit(-1);
    }
    return 0;
}