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

#define NUMBER_OF_CHILDS 50
#define NUMBER_OF_STRINGS 50
#define NUMBER_OF_CHARACTERS 80

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


typedef struct{
    char frase[NUMBER_OF_CHARACTERS];
} shared_data_type_strings;

typedef struct{
    shared_data_type_strings array[NUMBER_OF_STRINGS];
    int index;
} shared_data_type;

void down(sem_t *sem) {
    if (sem_wait(sem) == -1) {
        perror("Error at sem_wait().");
        exit(0);
    }
}

void up(sem_t *sem) {
    if (sem_post(sem) == -1) {
        perror("Error at sem_post().");
        exit(0);
    }
}

int main(void){

    sem_unlink("sema1_ex03");

    shm_unlink("/shmtest");

    //Auxiliar variable for loops
    int i;

    //Initializes pid variable
    int pid;

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

    //Initializes semaphore
    sem_t *sem1;

    //Create semaphore with value 1
    if ((sem1 = sem_open("sema1_ex03", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()\n");
        exit(1);
    }

    //Sets the first index value to 0
    shared_data->index = 0;

    //Creates the childs
    pid = babymaker(NUMBER_OF_CHILDS);

    if (pid > 0)    //Childs codes
    {
        //Dercrements the semaphore so only one child can enter shared memory
        down(sem1);

        //Writes in shared memory strings the phrase asked
        sprintf(shared_data->array[shared_data->index].frase,"I'm the Father - with PID %d",getpid());

        //Increments index    
        shared_data->index++;

        //Waits for 2 seconds
        //sleep(1);
        
        //Increments semaphore so one more child can enter shared memory
        up(sem1);

        //Ends child
        exit(0);
    }else if (pid == 0)
    {
        //Waits for all childs to be over
        for ( i = 0; i < NUMBER_OF_CHILDS; i++)
        {
            wait(NULL);
        }

        printf("All proccesses are done!\n");

        //Prints all strings
        for ( i = 0; i < NUMBER_OF_STRINGS; i++)
        {
            printf("Array[%d] : %s.\n",i,shared_data->array[i].frase);
        }

        //EX04------------------------------------------
        int scanner;
        printf("Enter the array you wanna delete:\n");
        scanf("%d", &scanner);
        sleep(2);
        if(scanner >= NUMBER_OF_CHILDS || scanner < 0){
            perror("Enter a correct number of the array to delete!\n");
        }else{
            sprintf(shared_data->array[scanner].frase, "Cleared");
        }

        //Prints all strings
        for ( i = 0; i < NUMBER_OF_STRINGS; i++)
        {
            printf("Array[%d] : %s.\n",i,shared_data->array[i].frase);
        }


        //-----------------------------------------------

        
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

        //Removes the semaphore
        if (sem_unlink("sema1_ex03")<0)
        {
            perror("No sem unlink()\n");
            exit(1);
        }
    }
      
    return 0;
}