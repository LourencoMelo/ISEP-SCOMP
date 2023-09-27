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
#define WRITING_TIME 5
#define STRING_SIZE 200

int main()
{
    int n_readers;
    int n_writers;
    //declare string
    char str[STRING_SIZE];

    //declare time
    time_t rawtime;
    struct tm *timeinfo;

    //SEMAPHORES-------------------------------------------------------
    sem_t *sem_w;
    sem_t *sem_nw;
    sem_t *sem_nr;

    //Creates sem_w   
    if ((sem_w = sem_open("sem_w", O_CREAT, 0777, 1)) == SEM_FAILED){
        perror("Error in sem_open()\n");
        exit(EXIT_FAILURE);
    }
    //Creates sem_nw  
    if ((sem_nw = sem_open("sem_nw", O_CREAT, 0777, 0)) == SEM_FAILED){
        perror("Error in sem_open()\n");
        exit(EXIT_FAILURE);
    }
    //Creates sem_nr 
    if ((sem_nr = sem_open("sem_nr", O_CREAT, 0777, 0)) == SEM_FAILED){
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

    //TIME
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    //form string
    sprintf(str, "pid--%d--current time--%s", getpid(), asctime(timeinfo));

    printf("press enter to start!\n");
    getchar();
    //writing process
    sem_post(sem_nw);

    //waits for writing permission
    printf("waiting for a spot...\n");
    sem_wait(sem_w);

    sem_getvalue(sem_nr, &n_readers);

    //waits for all readers to finnish
    while (n_readers != 0)
    {
        sleep(1);
        printf("waiting for readers to finnish|\n");
        sem_getvalue(sem_nr, &n_readers);
    }

    printf("started writing! ----> ");
    //simulate writing name
    sleep(WRITING_TIME);

    //write
    strcpy(pointer, str);
    printf("%s", pointer);
    printf("ended writing!\n");

    sem_getvalue(sem_nr, &n_readers);
    sem_getvalue(sem_nw, &n_writers);
    printf("number of writers : %d\nnumber of readers : %d\n", n_writers, n_readers);

    //liberates writing permission
    sem_post(sem_w);
    sem_wait(sem_nw);

    return 0;
}