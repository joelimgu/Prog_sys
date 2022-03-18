#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define D 5.0
#define NB_THREADS 1000
#define NB_ARROWS (100000*NB_THREADS)


typedef enum {false, true} bool;

double randomFloat(double min, double max){
    return ((max - min) * ((double)rand() / RAND_MAX)) + min;
}

bool is_point_in_circle(double x, double y) {
    return (x*x + y*y) < (D/2)*(D/2);
}

void* throw_arrows(void * args) {
    int nb_of_arrows = *(int *)args;
    double x = 0;
    double y = 0;
    int *res = malloc(sizeof(int));
    *res = 0;
    char *result = malloc(sizeof(char)*nb_of_arrows);

    for (int i = 0; i < nb_of_arrows; ++i) {
        x = randomFloat(-D/2,D/2);
        y = randomFloat(-D/2,D/2);
        result[i] = is_point_in_circle(x,y);
    }
    for (int i = 0; i < nb_of_arrows; ++i) {
        *res += result[i];
    }
    free(result);
    return (void*)res;
}

int main() {
    int arrows_per_thread = NB_ARROWS/NB_THREADS;
    pthread_t threads[NB_THREADS];
    int results[NB_THREADS];
    int fc = 0;
    double pi = 0;

    for (int i = 0; i < NB_THREADS; ++i) {
        pthread_create(&threads[i],NULL, throw_arrows,(void *)(&arrows_per_thread));
    }
    printf("created threads\n");
    for (int i = 0; i < NB_THREADS; ++i) {
        int * res = NULL;
        pthread_join(threads[i], (void *)&res);
        results[i] = *res;
        free(res);
    }
    printf("got responses \n");
    for (int i = 0; i < NB_THREADS; ++i) {
        fc += results[i];
    }
    pi = 4*(double)fc/NB_ARROWS;
    printf("PI = %f \n", pi);
//    printf("arrows: %d \n", throw_arrows(&arrows_per_thread)));
    return 0;
}