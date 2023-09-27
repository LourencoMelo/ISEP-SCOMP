#include <stdio.h>          //I/O
#include <stdlib.h>         //exit etc
#include <unistd.h>         //pipes
#include <sys/wait.h>       //wait    
#include <sys/types.h>
#include <string.h>         //strlen
#include <time.h>           //random numbers
#include <sys/mman.h>       //declarations related with management of shared memory : shm_* and mmap()
#include <sys/stat.h>       //constants used for opening
#include <sys/types.h>      //data types used by the API
#include <fcntl.h>          //file control options 
#include <semaphore.h>      //semaphores

//Number of childs to be created
#define NUMBER_OF_CHILDS 1
#define NUMBER_OF_SEMAFOROS 2
#define NUMBER_OF_MESSAGES 15

//Creates n child processes
int babymaker(int n)
{
    int p, i;
    for (i = 0; i < n; i++)
    {
        p = fork();
        if (p < 0)
        {
            exit(-1);
        }
        else if (p == 0)
        {
            return i + 1;
        }
    }
    return 0;
}

int main(void){

    const char NAME_SEM[2][80] = {"WITHOUT_FATHER", "SEM2"};
    const int WITHOUT_FATHER = 0;
    const int SEM2 = 1;
    const int INITIUAL_VALUES[2] = {0,1};

    //Pid
    int pid;
    int i;

    //Unlinking sems
    for(i=0; i < NUMBER_OF_SEMAFOROS; i++){
        if(sem_unlink(NAME_SEM[i]) == -1){
            perror("Error in the sem_unlink");
            exit(EXIT_FAILURE);
        }
    }
    


    //Initializes semaphore
    sem_t *sem1[NUMBER_OF_SEMAFOROS];

    for(i = 0; i < NUMBER_OF_SEMAFOROS; i ++){
        //Create semaphore with value 0
        if ((sem1[i] = sem_open(NAME_SEM[i], O_CREAT, 0644, INITIUAL_VALUES[i])) == SEM_FAILED)
        {
            perror("Error in sem_open()\n");
            exit(EXIT_FAILURE);
        }
    }

    //Creates child
    pid = babymaker(NUMBER_OF_CHILDS);

    if (pid > 0)  //Child code
    {
        int k;
        for(k = 0; k < (NUMBER_OF_MESSAGES/2); k++){
            if(sem_wait(sem1[SEM2])){   //Decrements sem
                perror("Error in the sem_wait SEM2");
                exit(EXIT_FAILURE);
            }
            printf("I’m the child\n");
            if(sem_post(sem1[WITHOUT_FATHER])){ //Increments sem
                perror("Error in the sem_post WITHOUT_FATHER");
                exit(EXIT_FAILURE);
            }  
        }
    }else if (pid == 0) //Father code
    {
        int k;
        for(k=0; k < (NUMBER_OF_MESSAGES/2); k++){
            if(sem_wait(sem1[WITHOUT_FATHER])){ //Decrements sem
                perror("Error in the sem_wait WITHOUT_FATHER");
                exit(EXIT_FAILURE);
            }  
            printf("I’m the father\n");
            if(sem_post(sem1[SEM2])){
                perror("Error in the sem_post SEM2");
                exit(EXIT_FAILURE);
            }
        }
    }
    //Closing sems
    for(i=0; i < NUMBER_OF_SEMAFOROS; i++){
        if(sem_close(sem1[i]) == -1){
            perror("Error in the sem_close");
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}