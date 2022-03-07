

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5
pthread_once_t once = PTHREAD_ONCE_INIT;
pthread_key_t key;


int incrementation() {
    int * incr = (int *)pthread_getspecific(key);
    (*incr)++;
    return *incr;
}

void fonction_init() {
    printf("fonction init executé\n");
    pthread_key_create(&key, NULL);
}

void * thread(void * args) {
    pthread_once(&once, fonction_init);
    int * p = malloc(sizeof(int)); // malloc de la zone de memoire qu'on veut utiliser par thread, dans ce cas on veut garder qu'un int
    *p = 0;
    pthread_setspecific(key,p);
    for (int i = 0; i < N; i++ ) {
        printf("[%d] print incr %d\n",(int)pthread_self(), incrementation());
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t th1;
    pthread_t th2;
    pthread_create(&th1, NULL, thread, NULL);
    pthread_create(&th2, NULL, thread, NULL);

    pthread_once(&once, fonction_init);
    int * p = malloc(sizeof(int)); // malloc de la zone de memoire qu'on veut utiliser par thread, dans ce cas on veut garder qu'un int
    *p = 0;
    pthread_setspecific(key,p);
    for (int i = 0; i < N; i++ ) {
        printf("print incr %d\n", incrementation());
        sleep(1);
    }

    return 0;
}

