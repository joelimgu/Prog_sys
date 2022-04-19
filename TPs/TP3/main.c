
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define N 100
#define D 1000

struct thread_data {
    int start_index;
    int end_index;
    int *tab1;
    int **tab2;
};

void * methode1_th(void * arg) {
    struct thread_data td = *(struct thread_data *)arg;
    int * res = malloc(sizeof(int)*D);

    for (int i = 0; i < D; ++i) {
        res[i] = 0;
        for (int j = td.start_index; j < td.end_index; ++j) {
//            printf("j: %d\n",j);
//            printf("p: %p\n",td.tab2);
            int a = td.tab2[j+i];
//            printf("a: %d\n",a);
            res[i] += td.tab1[j]*a;
        }
    }
//    printf("return\n");
    return (void*)res;
}

void methode1(int *tab1, int * tab2[], int nb_of_threads) {
    pthread_t *threads = malloc(sizeof(pthread_t)*nb_of_threads);
    for (int i = 0; i < nb_of_threads; i++) {
        struct thread_data *data = malloc(sizeof(struct thread_data)); // ouais no ne fais pas free
        data->start_index = i * (N / nb_of_threads);
        data->end_index = (i + 1) * (N / nb_of_threads);
        data->tab1 = tab1;
        data->tab2 = tab2;
//        printf("start: %d ; end: %d\n", data->start_index, data->end_index);
        pthread_create(&threads[i], NULL, methode1_th, data);
    }


    int res_arr[D];
    int* res = NULL;
    for (int i = 0; i < nb_of_threads; i++) {
//        printf("TOTO\n");
        pthread_join(threads[0], (void*)&res);
//        printf("p %p\n",res);
        for (int j = 0; j < (D/nb_of_threads); ++j) {
            res_arr[i*(D/nb_of_threads)+j] = res[j];
        }
    }
//    printf("last num = %d\n", res_arr[D-1]);
    free(threads);
}

int main(int argc,char * argv[]) {
    int method = atoi(argv[1]);
    int nb_threads = atoi(argv[2]);
    int nb_iterations = atoi(argv[3]);
    int tab1[N];
    int tab2[N][D];
    for (int i = 0; i < N; i++) {
        tab1[i] = i;
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < D; ++j) {
            tab2[i][j] = i + j;
        }
    }
    switch (method) {
        case 1:
            for (int i = 0; i < nb_iterations; ++i) {
                methode1(tab1, tab2, nb_threads);
            }
            break;
    }
    printf("end %d \n", method);
}