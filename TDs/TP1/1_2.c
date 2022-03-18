#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1

void signal_handler(int sig) {
    printf("received signal %d, calling from %d\n", sig, getpid());
    sleep(1);
}

int main() {
    int f = fork();
    struct sigaction action;
    action.sa_handler = signal_handler;
    action.sa_flags = 0;
    if (sigaction(SIGUSR1, &action, NULL)) {
        printf("pb dans la mise en place du signal\n");
        exit(-1);
    }
    switch (f) {
        case 0: // we execute the child code
            printf("executing child in pid: %d\n", getpid());
            while (TRUE) { }
    }
    printf("executing parent in pid: %d\n", getpid());

    sleep(30);
    kill(f,SIGUSR1);
    kill(getpid(),SIGUSR1);
    return 0;
}