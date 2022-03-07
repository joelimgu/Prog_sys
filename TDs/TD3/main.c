
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NB_THREADS 50
#define RESSOURCES NB_THREADS+1


int ressources_disponibles = RESSOURCES;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

typedef struct {
    int id;
    int nb_res;
} thread_param;

void * thread(void * args) {
    thread_param thp = *(thread_param*)args;
    int id = thp.id;
    int nb_res = thp.nb_res;
    printf("starting thread %d\n",id);

    pthread_mutex_lock(&mutex);

    while ( ressources_disponibles < nb_res) {
        printf("failed condition for thread %d\n", id);
        pthread_cond_wait(&cond,&mutex);
    }
    ressources_disponibles -= nb_res;
    printf("thread %d set res to %d\n",id,ressources_disponibles );
    pthread_mutex_unlock(&mutex);

    printf("Thread %d used it's %d ressources\n", id, nb_res);
    sleep(1);

    pthread_mutex_lock(&mutex);
    ressources_disponibles += nb_res;
    printf("thread %d set res to %d\n",id,ressources_disponibles );
    pthread_cond_signal( &cond );
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {

    pthread_t th[NB_THREADS];
    thread_param * tab_param[NB_THREADS];
    for ( int i = 0; i < NB_THREADS; i++ ) {
        thread_param * thp = malloc(sizeof(thread_param));
        tab_param[i] = thp;
        thp->id = i+1;
        thp->nb_res = NB_THREADS - i;
        pthread_create(&th[i], NULL, thread, (void *)thp);
    }

    for (int i = 0; i < NB_THREADS; i ++){
        void * res = NULL;
        printf("waiting for %d threads\n", NB_THREADS-i);
        pthread_join(th[i],res);
        free(tab_param[i]);
    }


    return 0;
}