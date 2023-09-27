#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
    int status;
    pid_t pid;
    pid = fork();
    if(pid == 0){
        sleep(1);
        exit(1);
    }else{
        wait(&status); //espera o status do processo filho até tar pronto que o seu valor é status =1
        if(WIFEXITED(status)){
            printf("%d \n", WEXITSTATUS(status));
            pid = fork();
            if(pid ==0){
                sleep(2);
                exit(2);
            }else{
                wait(&status);
                printf("%d \n", WEXITSTATUS(status));
            }
        }
    
    }
    return 0;
}