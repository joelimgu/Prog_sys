
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    int num;
    if (argc < 2) {
        printf("vous devez fournir un numero à afficher comme argument: ./question_a 2 \n");
        exit(-1);
    }
    num = atoi(argv[1]);
    for (int i = 0; i < 30; i++) {
        printf("%d\n",num);
        sleep(1);
    }
    return 0;
}