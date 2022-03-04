
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int * px = NULL;

void * aff_coucou(void * args) {
    int * max = (int*)args;
    for ( int i = 0; i < *max; i++ ) {
        pthread_mutex_lock(&mutex);
        printf("px coucou: %d\n",*px);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return (void*)1;
}

void * aff_courroux(void * args) {
    int * n = (int*)args;
    for ( int i = 0; i < *n; i++ ) {
        printf("%d: et mon courroux\n",i);
        sleep(1);
    }
    return (void*)NULL;
}

int main() {
    int m = 1;
    int n = 3;
    int x = 1;
    int max = 15;
    px = &x;

    pthread_t thread_id1;
//    pthread_t thread_id2;
    pthread_create(&thread_id1, NULL, aff_coucou, (void *)&max);
//    pthread_create(&thread_id2, NULL, aff_courroux, (void *)&n);
//    pthread_cancel(thread_id2);

    for ( int i = 0; i < max; i++ ) {
        pthread_mutex_lock(&mutex);
        px = NULL;
        printf("i main: %d\n",i);
        px = &x;
        pthread_mutex_unlock(&mutex);
        printf("px main: %d\n",*px);
        sleep(1);
    }

    long res = 0;
//    pthread_join(thread_id1,(void*)&res);
    printf("finishedS with status: %ld\n",res);

    return 0;
}

/*
 * pthread_exit() tue le processus dans lequel est appelé amis seulement celui la
 * alors si on l'appelle dans un thread il le tue, si on l'appelle dans le main il tue
 * le process main et comme il n'arrive pas à la fin execution les autres processus en cours sont encore vivants
 */