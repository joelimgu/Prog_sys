
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

/* Coment communiquer?
Je vais creer un pipe pour chaque processus.
Le programme lanceur disteribuira aux programmes les ids des pipes où lire et écrire.
Pour communiquer entre les thread j'utiliserai une variable globale portege par un mutex
*/
struct buffer {
    int is_mess_sent;
    int mess;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct buffer read_buff = {1,0};

void err(char * s) {
    printf("%s", s);
    exit(-1);
}

void * listen(void * args) {
    // listen for message
    int pipe_rx = *(int*)args;
    int buff;
    printf("listening\n");
    while (1) {
        if(read(pipe_rx,&buff, sizeof(int)) != 0) { // tode recieve chars until '\0' instyed of an int
            printf("[%d]received at fin %d\n",getpid(), buff);
            pthread_mutex_lock(&mutex);
            read_buff.mess = buff;
            read_buff.is_mess_sent = 0;
            pthread_mutex_unlock(&mutex);
        }
    }
}

// utiliser des wait cond pour ne pas faire des waits actives
void * repeat(void * args) {
    int tube_tx = *(int*)args;
    
    while (1) {
        pthread_mutex_lock(&mutex);
        if (read_buff.is_mess_sent == 0) {
            read_buff.is_mess_sent = 1;
            write(tube_tx,&read_buff.mess, sizeof(int)); // todo make a char* avec un for
        }
        pthread_mutex_unlock(&mutex);
        usleep(10);
        
    }

    return NULL;
}

/* params: a b
    a: id du pipe de lecture
    b: nb de vehicules à creer derrière
*/
int main(int argc, char * argv[]) {
    printf("created child with pid %d\n", getpid());
    int pipe_rx = atoi(argv[1]);
    int forks = atoi(argv[2]);
    void * res;
    pthread_t listener;
    pthread_t repeater;


    // creatiion fin ( juste reçevoir rx et ne pas creer plsu de fork )
    pthread_create(&listener, NULL, listen, &pipe_rx);
    pthread_join(listener,&res);

    
    return 0;
}