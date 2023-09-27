#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int pid = 1;
    int hundred = 100;
    for(int i = 0; i <10; i++){
        if (pid>0){
            pid = fork();
            wait(0);
        }
        if(pid ==0){
            for(int j = hundred *i; j < hundred * (j+1);j++){
                printf("%d", j);
            }
            exit(0); 
        }
    }
    return 0;
}  