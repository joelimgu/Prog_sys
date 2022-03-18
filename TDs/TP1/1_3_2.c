
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
    int val = 0;

    if (mkfifo("/tmp/one_to_zero",S_IRUSR | S_IWUSR | S_IRGRP)) {
        printf("unable to create the pipe\n");
        exit(-1);
    }
    printf("1: created file\n");
    sleep(1);

    if (!(tx = open("/tmp/one_to_zero", O_WRONLY))){
        printf("pb open\n");
        exit(-1);
    }
    printf("1: opened tx\n");


    if (!(rx=open("/tmp/zero_to_one_tube", O_RDONLY))) {
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
    int val = 0;

    if (mkfifo("/tmp/zero_to_one_tube",S_IRUSR | S_IWUSR | S_IRGRP)) {
        printf("unable to create the pipe\n");
        exit(-1);
    }
    printf("0: created file...\n");
    printf("0: slept\n");

    if (!(tx = open("/tmp/zero_to_one_tube", O_WRONLY))){
        printf("pb open\n");
        exit(-1);
    }
    printf("0: opened tx\n");

    int c = 0;
    while (!(rx=open("/tmp/one_to_zero", O_RDONLY)) || c < 50) {
        printf("pb open\n");
        sleep(1);
        c++;
    }
    printf("0: one_to_zero fifo found!\n");


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
    if (argc < 2) { printf("must pass the mode (0|1) as parameter\n"); exit(-1);}
    int mode = atoi(argv[1]);
    switch (mode) {
        case 0:
            zero_func();
            break;
        case 1:
            one_func();
            break;
        default:
            printf("value must be either 1 or 0\n");
    }
    return 0;
}