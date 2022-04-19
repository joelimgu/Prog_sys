
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define TEST_LEN 1000

#define TRUE 1
#define FALSE 0


void one_func() {
    int tx;
    int rx;
    if (mkfifo("/tmp/one_to_zero",S_IRUSR | S_IWUSR | S_IRGRP)) {
        printf("unable to create the pipe\n");
        exit(-1);
    }
    if (!(tx = open("/tmp/one_to_zero", O_WRONLY))){
        printf("pb open\n");
        exit(-1);
    }

    if (!(d_lect=open("/tmp/zero_to_one_tube", O_RDONLY))) {
        printf("pb open\n");
        exit(-1);
    }

    while (val <= TEST_LEN) {
        if (read(rx,&val,4)!=4){
            printf("pb lec\n");
            exit(-1);
        }

        val += 1;

        if (write(tx,&val,4) != 4) {
            printf("pb ecr.\n");
            exit(-1);
        }
    }
    exit(0);
}

void zero_func() {
    int tx;
    int rx;
    if (mkfifo("/tmp/zero_to_one_tube",S_IRUSR | S_IWUSR | S_IRGRP)) {
        printf("unable to create the pipe\n");
        exit(-1);
    }
    if (!(tx = open("/tmp/zero_to_one_tube", O_WRONLY))){
        printf("pb open\n");
        exit(-1);
    }

    int c = 0;
    while (!(rx=open("/tmp/one_to_zero", O_RDONLY)) || c < 50) {
        printf("pb open\n");
        sleep(1);
        c++;
    }
    printf("one_to_zero fifo found!\n");


    int val = 0;
    while (val <= TEST_LEN) {
        if (write(tx,&val,4) != 4) {
            printf("pb ecr.\n");
            exit(-1);
        }

        val += 1;

        if (read(rx,&val,4)!=4){
            printf("pb lec\n");
            exit(-1);
        }
    }
}

int main(int argc, char * argv[]) {
    if (argc < 2) { printf("must pass the mode (0|1) as parameter"); exit(-1);}
    int mode = atoi(argv[1]);
    switch (mode) {
        case 0:
            zero_func();
            break;
        case 1:
            one_func();
            break;
        default:
            printf("value must be either 1 or 0");
    }



    int child_to_parent_pipe[2];
    if (mkfifo("/tmp/child_to_parent_pipe",S_IRUSR | S_IWUSR | S_IRGRP)) {
        printf("unable to create the pipe\n");
        exit(-1);
    }
    if (!(d_ecr = open("/tmp/child_to_parent_pipe", O_WRONLY))){
        printf("pb open\n");
        exit(-1);
    }

    int f = fork();
    struct timeval start_time;
    gettimeofday(&start_time, 0);
    switch (f) {
        case 0:
            child_process_func(parent_to_child_pipe[0],child_to_parent_pipe[1]);
            break;
        default:
            parent_process_func(child_to_parent_pipe[0], parent_to_child_pipe[1]);
    }
    struct timeval end_time;
    gettimeofday(&end_time, 0);
    long int udiff = end_time.tv_usec - start_time.tv_usec;
    printf("Took %ld s and %ld us\n", end_time.tv_sec - start_time.tv_sec, udiff);
    long int debit = (TEST_LEN*2*4*8)/udiff;
    printf("with a bandwidth of: %ld Mb/s\n", debit);
//    int x = 10;
//    int y = 0;
//    write(parent_to_child_pipe[1], &x, 4);
//    printf("read: %ld \n", read(parent_to_child_pipe[0], &y, 4));
//    printf("value red %d\n", y);
    return 0;
}