
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex_px = PTHREAD_MUTEX_INITIALIZER;
int * px = NULL;

pthread_mutex_t mutex_py = PTHREAD_MUTEX_INITIALIZER;
int * py = NULL;

void * thread1(void * args) {
    int * max = (int*)args;
    for ( int i = 0; i < *max; i++ ) {
        pthread_mutex_lock(&mutex_px);
        printf("px thread1: %d\n",*px);
        pthread_mutex_unlock(&mutex_px);
        sleep(1);
    }
    return (void*)1;
}

void * thread2(void * args) {
    int * n = (int*)args;
    for ( int i = 0; i < *n; i++ ) {
        pthread_mutex_lock(&mutex_py);
        printf("py thread2:%d\n",*py);
        pthread_mutex_unlock(&mutex_py);

        sleep(1);
    }
    return (void*)NULL;
}

void * thread3(void * args) {
    int * n = (int*)args;
    for ( int i = 0; i < *n; i++ ) {
        pthread_mutex_lock(&mutex_px);
        pthread_mutex_lock(&mutex_py);
        printf("thread3 px py: %d %d\n",*px, *py);
        pthread_mutex_unlock(&mutex_py);
        pthread_mutex_unlock(&mutex_px);

        sleep(1);
    }
    return (void*)NULL;
}

int main() {
   // int m = 1;
   // int n = 3;
    int x = 1;
    int max = 15;
    px = &x;
    py = &x;

    pthread_t thread_id1;
    pthread_t thread_id2;
    pthread_t thread_id3;
    pthread_create(&thread_id1, NULL, thread1, (void *)&max);
    pthread_create(&thread_id2, NULL, thread2, (void *)&max);
    pthread_create(&thread_id3, NULL, thread3, (void *)&max);
//    pthread_create(&thread_id2, NULL, aff_courroux, (void *)&n);
//    pthread_cancel(thread_id2);

    for ( int i = 0; i < max; i++ ) {
        pthread_mutex_lock(&mutex_px);
        px = NULL;
        printf("i main: %d\n",i);
        px = &x;
        pthread_mutex_unlock(&mutex_px);
        printf("px main: %d\n",*px);
        sleep(1);
    }

    long res = 0;
//    pthread_join(thread_id1,(void*)&res);
    printf("finished with status: %ld\n",res);

    return 0;
}

/*
 * pthread_exit() tue le processus dans lequel est appelé amis seulement celui la
 * alors si on l'appelle dans un thread il le tue, si on l'appelle dans le main il tue
 * le process main et comme il n'arrive pas à la fin execution les autres processus en cours sont encore vivants
 */