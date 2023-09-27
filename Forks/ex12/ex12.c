#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define CHILD_NUMBER 6

int spawn_childs(int n){
    int pid = 1;
    for(int i = 0; i < n; i++){
        if(pid > 0 ){
            pid = fork();
        }
        if(pid == 0){
            return i;
        }
    }
    return 0;
}



int main(){
    int status;
    int index = spawn_child(CHILD_NUMBER);

    if(index > 0){
        exit(index * 2);
    }else{
        for(int i = 0; i < CHILD_NUMBER; i++){
            wait(&status);
            printf("Status : %d \n ", WEXISTATUS(status));
        }
    }
}