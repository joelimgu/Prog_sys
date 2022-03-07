

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5
pthread_once_t  once = PTHREAD_ONCE_INIT;

void fonction_init() {
    printf("fonction init executé\n");
}

void * thread(void * args) {
    pthread_once(&once, fonction_init);
    for (int i = 0; i < N; i++ ) {
        printf("print i %d\n", i);
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
    for (int i = 0; i < N; i++ ) {
        printf("print i %d\n", i);
        sleep(1);
    }

    return 0;
}

