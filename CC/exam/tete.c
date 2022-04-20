
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

#define TAILLE_MES sizeof(int)
#define FORKS 3 // todo make as cmd arg
void err(char * s) {
    printf("%s", s);
    exit(-1);
}

int main(int argc, char * argv[]) {
    int tube[2];
    int pid;
    if (argc < 2) { err("you mist provode the number of child vehicles ./lanuch 3\n");}
    int nb_of_forks = atoi(argv[1]);
    if (pipe(tube) == -1) { err("error creating the pipe");}
    
    pid = fork();
    if (pid == 0) { // fils
        char tub[10];
        char forks[10];
        sprintf(tub,"%d",tube[0]);
        sprintf(forks,"%d",nb_of_forks);
        execl("intermediaire","intermediaire",tub,forks,NULL);
    } else {
        int buff = 1;
        for ( int i = 0; i < 10; i++) {
            write(tube[1],&i, TAILLE_MES);
            sleep(2);
        }
    }
    return 0;
}