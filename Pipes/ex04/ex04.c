#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int baby_maker(int child_number){
    int pid;
    int i;
    for(i = 0; i < child_number; i++){
        pid = fork();
        if(pid == 0){
            return i + 1; //sends de id
        }else if (pid == -1){
            return -1;
        }
    }
    //father returns 0
    return 0;
}

int main(){
    int fd[2];
    //Pipe done
    pipe(fd);
    //Creates a pipe
    if(pipe(fd)==-1){
        printf("Pipe with errors\n");
    }

    int id = baby_maker(1);

    if(id == 0){
        close(fd[0]);
        if(dup2(fd[1],1) == -1){
            printf("Error in the copy the file");
        }
        close(fd[1]);
        int ret = execlp("cat", "cat", "file.txt", (char*) NULL);
        if(ret == -1){
            printf("Error in execlp");
        }
    }else{
        char str[300];
        close(fd[1]);
        read(fd[0], str, 300);
        close(fd[0]);
        printf("The content: \n %s", str);
        exit(0);
    }
    return 0;
}