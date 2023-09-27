#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
    int x=1;
    int id;
    pid_t p = fork();
    if (p == 0) {
        x = x+1;
        printf("1. x = %d\n", x);
        id = getpid();
    }else{
        x = x-1;
        printf("2. x = %d \n", x);
        id = getpid();
    }
    printf("3. %d; x = %d, id = %d \n", p , x, id);
    exit(0);
    return 0;
}