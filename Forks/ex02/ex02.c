#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
    pid_t pid;
    printf("I'm..\n");
    pid = fork();
    if(pid == -1){
        perror("Erro ao criar o processo:\n");
        exit(1);
    }
    if(pid > 0){
        printf("the... \n");
        pid = 0;
        pid = fork();
        if(pid == -1){
            perror("Erro ao criar o processo:\n");
            exit(1);
        }
        if(pid > 0){
            printf("father! \n");
            pid = 0;
            pid = fork();
            if(pid == -1){
            perror("Erro ao criar o processo:\n");
            exit(1);
            }
            if(pid > 0){
                printf("Pai final \n");
            }else{
                printf("I'll never join you!\n");
            }
        }else{
            printf("I'll never join you! \n");
        }
    }else{
        printf("I'll never join you! \n");
    }
    exit(0);

    //exercicio com waits
    printf("I'm... \n");
    pid_t pid;
    pid = fork();
    if(pid<0){
        perror("Fork falhou");
        exit(1);
    }
    if(pid == 0){
        printf("I 'll never join you! \n");
    }else{
        wait(NULL);
        printf("the ... \n");
        pid = fork();
        if(pid<0){
            perror("Fork falhou");
            exit(1);
        }
        if(pid==0){
            printf("I 'll never join you! \n");
        }else{
            wait(NULL);
            printf("father! \n");
            pid = fork();
        }if(pid <0){
            perror("Fork falhou");
        }
        if(pid == 0){
            printf("I 'll never join you! \n");
        }else{
            wait(NULL);
        }
    }
    return 0;
}