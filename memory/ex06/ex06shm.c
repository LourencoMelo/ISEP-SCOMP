#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h> /* For constants O_* */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define sizeOfTheArray 100000

typedef struct
{
    int array[sizeOfTheArray];
}shared_data_type;


int main(void){
    int data_size = sizeof(shared_data_type); //size
    clock_t start, finish; //clocks
    int i, vec[data_size];
    int fd;
    int checkError;
    shared_data_type *shared_data;

     //initializing the array with random numbers up to 5
    srand((unsigned) time(NULL));
	for(i = 0; i < sizeOfTheArray; i++){
		vec[i] = rand() % 6;
	}

    shm_unlink ("/shmtest");

    fd = shm_open("/shmtest", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    /*Creates (O_CREAT) shared memory area (with error if
    existis because of O_EXCL) with name “/shmtest, and
    read/write permissons (O_RDWR), and open for user to
    read (S_IRUSR) and write (S_IWUSR).
    */
    if(fd == -1){
        perror("Error in the shm_open!");
        return -1;
    }
    checkError = ftruncate(fd,data_size);
    /*
    Defines a n bytes size
    */
    if(checkError == -1){
        perror("Error in the ftruncate!");
        return -1;
    }
    shared_data = (shared_data_type*)mmap(NULL,data_size, PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);
    /*
        Maps the area in na address decided by the OS
        (NULL), with the specific data_size, with
        read and write permissions (PROT_READ |
        PROT_WRITE) , consistente with open (continues)
    */
   if(shared_data == MAP_FAILED){
       perror("Error in the shared_data!\n");
       return -1;
    }

    start = clock();

    pid_t p; 
    p = fork();
    if(p == -1){
        perror("Error in fork!");
        exit(EXIT_FAILURE);
    }
    //CHILD
    if(p == 0){
        int j;
        for(j = 0; j < data_size; j++){
            shared_data->array[j] = vec[j];
        }
          //closes of the child
        if (munmap(shared_data, data_size)){
            perror("Error in munmap!");
            return 1;
        }
        if (close(fd) == -1){
            perror("Error in close!");
            return 1;
        }
        exit(0);
    }

    //PARENT
    finish = clock();
    float total_time = (float)(finish - start) / CLOCKS_PER_SEC;
    printf("Time of the shared memory process: %f\n", total_time);
    printf("Exiting the program...\n");

    //closes of the father
    if (munmap(shared_data, data_size)){
            perror("Error in munmap!");
            return 1;
    }
    if (close(fd) == -1){
        perror("Error in close!");
        return 1;
    }
    if(shm_unlink("/shmtest")){
        perror("Error in shm_unlink");
        return 1;
    }
    wait(NULL);
    return 0;
}