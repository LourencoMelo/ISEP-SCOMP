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
#include "struct.h"         //first structure
#include "data_base.h"      //Data_base structure

int main(void){

    int fd, data_size = sizeof(data_base);
    data_base *shared_data;
    personal_data *p;
    
    //Creates (O_CREAT) shared memory area (with error if exists because of O_EXCL) with name “/shmtest, and read/write permissons (O_RDWR), and open for user to read (S_IRUSR) and write (S_IWUSR).
    fd = shm_open("/shmtest", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    
    
    //Verification of error
    if(fd == -1) {
        return fd;
    }

    //Defines a data_size bytes size
    ftruncate (fd, data_size);


    //Maps the area in an address decided by the OS (NULL), requiring to use all the size (data_size), with read and write permissions(PROT_READ|PROT_WRITE), consistent with open(continues)
    shared_data = (data_base *) mmap (NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        
    //Verification of error
    if(shared_data == MAP_FAILED) {
        return -1;
    }

    //Declares sems
    sem_t *sem1;
    //opens and checks error
    if ((sem1 = sem_open("sem1", O_CREAT, 0644, 1)) == SEM_FAILED){
            perror("Error in sem_open()\n");
            exit(EXIT_FAILURE);
    }

    //INTERFACE
    sem_wait(sem1);

    int insertednumber;
    printf("Insert a number to be searched:\n");
    scanf("%d", &insertednumber);
    fflush(0);

    if(shared_data->savedNumber == 0){
        printf("The data base is empty, insert first please!");
    }else{
        int i;
        for(i = 0; i < shared_data->savedNumber; i++){
            p = &(shared_data->globalData[i]);
            if(p->number == insertednumber){
                printf("Participant: %d\n Number: %d\n Name:%s\n Address:%s \n \n", i+1,p->number, p->name, p->address);
            }
        }
    }
    sem_post(sem1);

    if(sem_unlink("sem1") == -1){
            perror("Error in the sem_unlink");
            exit(EXIT_FAILURE);
    }

    //Disconnects the shared memory area from the process address space (página 9 TP)
    if(munmap(shared_data, sizeof(data_base))<0){
        perror("No munmap()!\n");
        exit(0);
    }
    //Closes the file descriptor (página 9 TP)
    if(close(fd)<0){
        perror("No close()!\n");
        exit(0);
    }
    return 0;
}