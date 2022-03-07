#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NB_LOOP 2
#define MAX 6

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void exit_handler(){
    pthread_mutex_unlock(&mutex);
}


void * thread(void * args){
    pthread_cleanup_push(exit_handler,NULL);
    pthread_mutex_lock(&mutex);
    printf("J'ai le mutex\n");
    for ( int i = 0 ; i<MAX; i++ ){
        int tid = (int)pthread_self();
        printf("tid : %d\n",tid);
        sleep(1);
    }
    pthread_mutex_unlock(&mutex);
    pthread_cleanup_pop(0);
    return NULL;
}

int main() {
    pthread_t tab_tid[NB_LOOP];
    for (int i =0 ; i<NB_LOOP;i++){
        pthread_create(&tab_tid[i],NULL,thread,NULL);
        for (int j = 0 ; j < MAX/2; j++){
            printf("tid : %ld\n",tab_tid[i]);
            sleep(1);
        }
        pthread_cancel(tab_tid[i]);
        sleep(1);
    }
    return 0;
};