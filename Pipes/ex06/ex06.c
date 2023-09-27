#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

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


void fill_array(int * array,int size,int limit_number){
  time_t t;
  int i;

  srand ((unsigned) time (&t));

  for (i = 0; i < size; i++)
    array[i] = rand () % limit_number;
}


int main() {
  int tmp = 0;
  int sum = 0;
  int vec1[1000];
  int vec2[1000];
  int fd[2];

  pipe(fd);

  fill_array(vec1,1000,10);
  fill_array(vec2,1000,10);

  int id = baby_maker(5);
  if(id == -1){
    printf("error creating child(s)\n");
  }
  if(id == 0){ //father
    close(fd[1]);//closing write
    int j;
    for(j = 0; j < 5;j++){
      read(fd[0],&tmp,4);
      sum += tmp;
    }
    close(fd[0]); //closing read

  }else{ //child
    close(fd[0]);//closing read
    int end_position = id * 200; //id starts in 1
    int start_position = end_position - 200;
    int i;

    for( i = start_position; i < end_position;i++){
      tmp += vec1[i] + vec2[i]; //sums the 2 arryas at the same time
    }
    write(fd[1],&tmp,4); //write to the father
    close(fd[1]); // closing write
    exit(0); //exit with no erros
  }

  //TESTING MODE
  int sum_test = 0;
  int f;
    //verify if the working right
  for(f = 0 ; f < 1000;f++){
    sum_test += vec1[f] + vec2[f];
  }

  printf("sum : %d\nsum_test :%d\n",sum,sum_test);

  return 0;
}
