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
#define PASSING_TIME 2     //Time that the car needs to pass to the other side of the bridge

typedef struct{
    int carsEastQueue;
    int carsWestQueue;
}shared_data_type;

int main(void){

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
    int error = ftruncate (fd, data_size);
    if(error == -1){
        perror("Error in ftruncate \n");
        return error;
    }

    //Maps the area in an address decided by the OS (NULL), requiring to use all the size (data_size), with read and write permissions(PROT_READ|PROT_WRITE), consistent with open(continues)
    shared_data = (shared_data_type *) mmap (NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        
    //Verification of error
    if(shared_data == MAP_FAILED) {
        return -1;
    }

    //SEMAFORES------------------------------------------
    sem_t *sem_east;
    sem_t *sem_west;

    if ((sem_east = sem_open("sem_east", O_CREAT, 0777, 1)) == SEM_FAILED){
        perror("Error in sem_open() east\n");
        exit(EXIT_FAILURE);
    }

    if ((sem_west = sem_open("sem_west", O_CREAT, 0777, 1)) == SEM_FAILED){
        perror("Error in sem_open() west\n");
        exit(EXIT_FAILURE);
    }

    //Menu
    printf("Direction : West\n Press enter to pass the bridge!\n");
    getchar();

    //Implementation------------------------------------------------
    sem_wait(sem_east); //block west direction
    sem_wait(sem_west); //only passing the cars that where in the sem
    printf("The cars are passing...\n");
    //Random number of cars;
    time_t t;
    srand ((unsigned) time (&t));
    int min_number=1; 
    int max_number=3;
    int cars_east_aux = 0;
    cars_east_aux = (rand()%(max_number+1-min_number)+min_number);
    int i;
    (shared_data->carsEastQueue) = cars_east_aux;
    for(i = 0; i <cars_east_aux; i++){
        sleep(PASSING_TIME);
        printf("One car passed!\n");
    }
    sem_post(sem_west); //allows the same direction(priority)
    sem_post(sem_east); //allows the opposite direction


    return 0;
}