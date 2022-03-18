#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>


void signal_handler(int sig) {
    printf("signal %d\n", sig);
}

void signal_handler2(int sig) {
    printf("signal main %d\n", sig);
    exit(-1);
}


void * thread(){
    sigset_t mask;
    // disable all signals minus SUGUSR1 handeling errors
    if ( sigfillset(&mask) ) {
        exit(-1);
    }
    if ( sigdelset(&mask, SIGUSR1) ) {
        exit(-1);
    }
    if ( pthread_sigmask(SIG_SETMASK, &mask, NULL) ) {
        exit(-1);
    }

    // create a handler for signal
    struct sigaction action;
    action.sa_handler = signal_handler;
    action.sa_flags = 0;
    if (sigaction(SIGUSR1, &action, NULL)) {
        printf("pb dans la mise en place de la reception du signal\n");
        exit(-1);
    }

    while (1) { }

    return NULL;
}

int main() {

    pthread_t t;
    pthread_create(&t, NULL, thread, NULL);

    printf("executing main in pid: %d\n", getpid());

    sigset_t mask;
    // disable all signals minus SUGUSR1 handeling errors
    if ( sigfillset(&mask) ) {
        exit(-1);
    }
    if ( sigdelset(&mask, SIGUSR2) ) {
        exit(-1);
    }
    if ( pthread_sigmask(SIG_SETMASK, &mask, NULL) ) {
        exit(-1);
    }


    struct sigaction action;
    action.sa_handler = signal_handler2;
    action.sa_flags = 0;
    if (sigaction(SIGUSR2, &action, NULL)) {
        printf("pb dans la mise en place de la reception du signal dans main \n");
        exit(-1);
    }

    sleep(1);
    pthread_kill(t, SIGUSR2);
    printf("woke up\n");

    while (1){}
    return 0;
};