#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void solution_one(){
    int integer;
    char string[30] = "The message \n";
    int fd[2];
    pid_t pid;
    
    //Creates a pipe
    if(pipe(fd)==-1){
        printf("Pipe with errors\n");
    }
    //creates the child process
    pid = fork();
    if(pid > 0){ //If its the father
        close(fd[0]);//closes the stdin
        //giving values to the variable integer
        integer = 55;

        write(fd[1], &integer, 4);
        write(fd[1], string, 30);
        close(fd[1]);
    }else if(pid == -1){
        printf("Fork with errors\n");
    }else{
        close(fd[1]); //Closing stdout
        //Reading values
        read(fd[0], &integer, 4);
        read(fd[0], string, 30);
        close(fd[0]); //Closing stdin
        printf("Number: %d\n",integer);
        printf("String: %s\n", string);
        exit(0); //the child exits
    }
}

struct SolutionStruct{
    int integer;
    char string[30];
};

void solution_two(){
    int fd[2];
    pid_t pid;
    //definition of the values of the structure
    struct SolutionStruct values;
    values.integer = 4;
    strcpy(values.string, "The message\n");

    //Creates a pipe
    if(pipe(fd)==-1){
        printf("Pipe with errors\n");
    }
    //creates the child process
    pid = fork();
    if(pid > 0){ //If its the father
        close(fd[0]);//closes the stdin
        //giving values to the variables
        write(fd[1], values, 34);
        close(fd[1]);
    }else if(pid == -1){
        printf("Fork with errors\n");
    }else{
        close(fd[1]); //Closing stdout
        //Reading values
        read(fd[0], &values.integer, 4);
        read(fd[0], &values.string, 30);
        close(fd[0]); //Closing stdin
        printf("Number: %d\n",values.integer);
        printf("String: %s\n", values.string);
        exit(0); //the child exits
    }
}

int main(){
    solution_one();
    solution_two();
    return 0;
}