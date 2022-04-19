#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

struct param {
    char * path;
    char * name;
    int count;
};
/*
 get_word(p.name,f);
 getc(f);
 get_word(p.path,f);
 .
 .
 .
 */


int main() {
    FILE *f;
    f = fopen("commands.txt","r");
    if ( f == NULL) {
        printf("error opening file\n");
        exit(-1);
    }
    char c = getc(f);
    struct param p = {malloc(sizeof(char)*255), malloc(sizeof(char)*255), 0};
    while (c != EOF) { // end of file
        while (c != '\n') { // end of line
            int i = 0;
            for ( int k = 0; k < 255; k++) {
                p.name[k] = '\0';
            }
            for ( int k = 0; k < 255; k++) {
                p.path[k] = '\0';
            }
            while (c != ' ') { // end of word
                p.path[i] = c;
                i++;
                c = getc(f);
            }
            printf("parsed 1st\n");
            c = getc(f);
            i = 0;
            while (c != ' ') { // end of word
                p.name[i] = c;
                i++;
                c = getc(f);
            }
            printf("parsed 2st\n");
            i = 0;
            char str[255];
            while (c != '\n') { // end of word
                str[i] = c;
                i++;
                c = getc(f);
            }
            printf("parsed 3st\n");

            p.count = atoi(str);
            printf("path: %s ; name: %s; count: %d\n",p.path,p.name,p.count);
            // exec
            for ( int j = 0; j<p.count; j++){
                int pid = fork();
                switch (pid) {
                    case 0:

                        if (execl(p.path,p.name, NULL) == -1) {
                            printf("error execl\n");
                            exit(-1);
                        }
                        break;
                    case -1:
                        printf("error fork\n");
                        exit(-1);
                    default:
                        printf("fork succeful, pid: %d\n",pid);
                }
            }
        }
        if ( c != EOF ) {
            c = getc(f);
        }
    }
    return 0;
}

