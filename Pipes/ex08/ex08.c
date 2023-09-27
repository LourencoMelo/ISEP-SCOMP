#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int baby_maker(int child_number){
  int pid;
  int i;
  for(i = 0;i < child_number; i++){
    pid = fork();
    if(pid == 0)
      //child returns 1...*
      return i + 1;
    else if (pid == -1)
      // child creating error
      return -1;
  }
  //father returns 0
  return 0;
}

int main(void){
    const int SIZE_STRING = 50;
    const int NUMBER_OF_CHILDREN = 10;

    struct s1
    {
        char message[4];
        int round;
    };

    int fd[2];
    //Creates the pipe
    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    int id = baby_maker(NUMBER_OF_CHILDREN);

    if (id == 0){ //Father
        close(fd[0]);
        int i;

        for (i = 0; i < NUMBER_OF_CHILDREN; i++){
            struct s1 s;
            strcpy(s.message, "WIN");
            s.round = i + 1;
            sleep(1);
            write(fd[1], &s, sizeof(s));
        }

        close(fd[1]);

        int status;
        for (i = 0; i < NUMBER_OF_CHILDREN; i++){
          pid_t pid = wait(&status);
          if (WIFEXITED(status)){
            int exit_round = WEXITSTATUS(status);
            printf("Child with pid number: %d \n Won the round number: %d\n", pid, exit_round);
          }
        }

    }

    if (id > 0){ //Child
        close(fd[1]);//Closes writing
        struct s1 s;
        read(fd[0], &s, SIZE_STRING);
        close(fd[0]); //Closes reading
        printf(s.message);
        printf("Round: %d\n", s.round);
        exit(s.round);
    }
    return 0;
}