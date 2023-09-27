#include <stdio.h>    //I/O
#include <stdlib.h>   //exit etc
#include <unistd.h>   //pipes
#include <sys/wait.h> //wait
#include <sys/types.h>
#include <string.h>    //strlen
#include <time.h>      //random numbers
#include <sys/mman.h>  //declarations related with management of shared memory : shm_* and mmap()
#include <sys/stat.h>  //constants used for opening
#include <sys/types.h> //data types used by the API
#include <fcntl.h>     //file control options
#include <semaphore.h> //semaphores
#define READING_TIME 2
#define STRING_SIZE 200

int main(){
    
    int n_readers;
    int writer_value;

    //declare string
    char str[STRING_SIZE];

    //SEMAPHORES-------------------------------------------------------
    sem_t *sem_nr;
    sem_t *sem_w;

    //Creates sem_w   
    if ((sem_nr = sem_open("sem_nr", O_CREAT, 0777, 0)) == SEM_FAILED){ //antes "sem_r"
        perror("Error in sem_open()\n");
        exit(EXIT_FAILURE);
    }
     //Creates sem_nr 
    if ((sem_w = sem_open("sem_w", O_CREAT, 0777, 1)) == SEM_FAILED){
        perror("Error in sem_open()\n");
        exit(EXIT_FAILURE);
    }

    //SHARED MEMORY---------------------------------------------------
    int memory_size = STRING_SIZE;
    int descriptor = shm_open("shared_memory", O_CREAT | O_RDWR, 0777);

    int error = ftruncate(descriptor, memory_size);
    if (error == -1)
    {
        printf("%s\n", "error in ftruncate!");
        return error;
    }
    char *pointer = (char *)mmap(NULL, memory_size, PROT_READ | PROT_WRITE, MAP_SHARED, descriptor, 0);
    if(pointer == MAP_FAILED){
        return -1;
    }

    printf("press enter to read!\n");
    getchar();
    //read process

    //waits for no writers to be present
    sem_getvalue(sem_w,&writer_value);

    while(writer_value == 0){
        sleep(1);
        printf("waiting for writer to leave!\n");
        sem_getvalue(sem_w,&writer_value);
    }
    //increases number of readers
    sem_post(sem_nr);

    //reads
    strcpy(str,pointer);
    sem_getvalue(sem_nr,&n_readers);

    //simulated time to read
    sleep(READING_TIME);
    printf("%s\n",pointer);
    printf("process with pid : %d done reading! string : %s,number of readers now : %d\n",getpid(),str,n_readers);

    sem_wait(sem_nr);

    return 0;
}