#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 2000

//ENCHER ARRAY
void create_array(int numbers[]){
    time_t t;
    int k;

    srand((unsigned) time(&t));
    for(k=0; k< ARRAY_SIZE; k++){
        numbers [k] = rand() %20;
    }
}

//Encontrar o valor pedido
int search_value(int n, int position, int positionPlus, int numbers[]){
    int j = position;
    for(j; j < positionPlus;j++){
        if(numbers[i] == n){
            return i - position;
        }
    }
    return 255;
}

int main(void){
    int numbers[ARRAY_SIZE];
    create_array(numbers);

    int n=15;
    int status;
    int position = 0, positionPlus = 200;
    int pid = 1; //Para confirmar q é o pai
    int array_pids[10];
    int i;
    for(i=0; i<10; i++){
        if(pid > 0){
            pid = fork();
            array_pids[10]= pid;
            if(pid ==0){
                exit(search_value(n,position,positionPlus,));
            }
            position+=200;
            positionPlus+=200;
        }
    }

    for(int j = 0; j<10; j++){
        waitpid(array_pids[],&status,0);
        IF(WIFEXCITED(status)){
           printf("Process: %d, INDEX: %d \n",j+1,) 
        }
    }
    return 0;
}