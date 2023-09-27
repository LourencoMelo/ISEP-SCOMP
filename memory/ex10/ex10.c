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

#define NUMBER_OF_CHILDS 4
#define ARRAY_SIZE 1000
#define SHARED_ARRAY_SIZE 10
#define MAXIMUM_NUMBER 1000
#define MINIMUM_NUMBER 0

typedef struct{
    int shared_array[SHARED_ARRAY_SIZE];
} shared_data_type;

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

        clock_t start_t, end_t;
        int i;
        start_t = clock();
        //Initializes pid variable
        int pid;

        //Aux variable to random numbers generator
        time_t t;

        int array[ARRAY_SIZE];

        srand ((unsigned) time (&t));

        for ( i = 0; i < ARRAY_SIZE; i++)
        {
            array[i] = (rand() % MAXIMUM_NUMBER) + MINIMUM_NUMBER;   //Random number between 1 and 20
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

        //Creates childs
        pid = babymaker(NUMBER_OF_CHILDS);   

        if (pid > 0)        //Child code.
        {
            //Calculates the limits for each child
            int superior_limit = pid*(ARRAY_SIZE/NUMBER_OF_CHILDS);
            int inferior_limit = superior_limit-(ARRAY_SIZE/NUMBER_OF_CHILDS);

            int maximum = 0;

            //Checks the maximum value of 100 integers
            for (i = inferior_limit; i < superior_limit; i++)
            {
                if (array[i] > maximum)
                {
                    maximum = array[i];
                }
            }

            //Saves the maximum value through the shared memory pointer on the final array
            shared_data->shared_array[pid-1] = maximum;
            

        }else if (pid == 0)     //Parent code.
        {   
            //Waits for all childs to end
            for (i = 0; i < NUMBER_OF_CHILDS; i++)
            {
                wait(NULL);
            }

            //Variable that will save the maximum of all numbers
            int global_maximum=0;

            //Checks all maximums returned by the childs processes and saves the maximum
            for (i = 0; i < SHARED_ARRAY_SIZE; i++)
            {
                printf("\nMaximums[%d]: %d;\n", i,shared_data->shared_array[i]);
                if (shared_data->shared_array[i] > global_maximum)
                {
                    global_maximum = shared_data->shared_array[i];
                }
            }
            
            printf("\nGlobal maximum = %d.\n",global_maximum);

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

        }
        end_t = clock();
        float total_t = (float)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("Starting of the program, start_t = %ld\n", start_t);
        printf("End of the big loop, end_t = %ld\n", end_t);
        printf("Total time taken by CPU: %f\n", total_t); //should be %f but creates an error ???
        return 0;
        //If the the numbers of childs is to high the time is going to be higher
        //If the number of childs is two low the time is going to be higher
        //The ideal number of child would be the number of cores on this virtual machine
}
