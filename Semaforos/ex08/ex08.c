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

#define NUMBER_OF_CHILDS 1

typedef struct{
    int differs;
}shared_data_type;

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

    const char NAME_SEM[2][30] = {"sem1", "sem2"};

    //Declares sems
    sem_t *sem1;
    sem_t *sem2;
    //Creates sems
    if ((sem1 = sem_open("sem1", O_CREAT, 0644, 1)) == SEM_FAILED){
            perror("Error in sem_open()\n");
            exit(EXIT_FAILURE);
    }
    if ((sem2 = sem_open("sem2", O_CREAT, 0644, 0)) == SEM_FAILED){
            perror("Error in sem_open()\n");
            exit(EXIT_FAILURE);
    }

    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    //Creates (O_CREAT) shared memory area (with error if exists because of O_EXCL) with name “/shmtest, and read/write permissons (O_RDWR), and open for user to read (S_IRUSR) and write (S_IWUSR).
    fd = shm_open("/shmtest", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    
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

    //Declares the 
    shared_data-> differs = 0;

    //Creates child
    int pid;
    pid = babymaker(NUMBER_OF_CHILDS);

    //Child code
    if(pid > 0){
        int i;
        while(i < 8){
            sem_wait(sem1);
            while(shared_data->differs < 1){
                printf("S \n");
                fflush(stdout); //Doesn't get trapped if there is no \n
                shared_data->differs += 1;
            }
            sem_post(sem2);
            i++;
        }
        exit(0);
    }else if(pid == 0){ //FATHER CODE
        int i;
        while(i < 8){
            sem_wait(sem2);
            while(shared_data->differs > -1){
                printf("C \n");
                fflush(stdout); //Doesn't get trapped if there is no \n
                shared_data->differs -= 1;
            }
            sem_post(sem1);
            i++;
        }
    }

    //Unlink sems
    if(sem_unlink(NAME_SEM[0]) == -1){
        perror("Error in the sem_unlink");
        exit(EXIT_FAILURE);
    }
    if(sem_unlink(NAME_SEM[1]) == -1){
            perror("Error in the sem_unlink");
            exit(EXIT_FAILURE);
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
    //Removes the memory area from the file system. Marks it to be deleted, as soon as all processes using it close it.(página 11 TP)
    if(shm_unlink("/shmtest")<0){
        perror("No unlink()\n");
        exit(1);
    }

    return 0;
}