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


typedef struct{
    int nTicket;
    int sellerActive;
}shared_data_type;

int main(void){

    //SEMAPHORES------------------------------------
    //sem_unlink("semaforoSeller");
    //sem_unlink("semaforoDois");
    //shm_unlink("shmtest");
    
    //SHARED MEMORY---------------------------------
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    

    //Creates (O_CREAT) shared memory area with name “/shmtest, and read/write permissons (O_RDWR), and open for user to read (S_IRUSR) and write (S_IWUSR).
    fd = shm_open("/shmtest", O_CREAT |O_RDWR, S_IRUSR|S_IWUSR);
    
    //Verification of error
    if(fd == -1) {
        return fd;
    }
    
    //Defines a data_size bytes size
    ftruncate (fd, data_size);
    

    //Maps the area in an address decided by the OS (NULL), requiring to use all the size (data_size), with read and write permissions(PROT_READ|PROT_WRITE), consistent with open(continues)
    shared_data = (shared_data_type *) mmap (NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        
       
    //Verification of error
    if(shared_data == MAP_FAILED) {
        return -1;
    }

    //Declares sems
    sem_t *sem1;
    //Creates sems

    if ((sem1 = sem_open("semaforoSeller", O_CREAT, 0644, 0)) == SEM_FAILED){
        perror("Error in sem_open()\n");
        exit(EXIT_FAILURE);
    }
    
    //IMPLEMENTATION--------------------------------
    
    shared_data->nTicket = 1;
    shared_data->sellerActive = 1;
    while(1){
        printf("The seller is waiting...\n");
        sem_wait(sem1);
        shared_data->nTicket++;
    }


    //Disconnects the shared memory area from the process address space (página 9 TP)
    if(munmap(shared_data, sizeof(shared_data_type))<0){
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