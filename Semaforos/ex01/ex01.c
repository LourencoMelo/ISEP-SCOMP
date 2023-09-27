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

#define NUMBER_OF_CHILDS 8
#define MAXIMUM_NUMBER 100
#define MINIMUM_NUMBER 0
#define ARRAY_SIZE 1600

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

    //Auxiliar variable for loops
    int i;

    //File pointer for reading
    FILE *reader;

    //File pointer for writing
    FILE *writer;

    //Auxiliar variable for random number generator
    time_t t;

    //Initializes pid variable
    int pid;

    //Initializes semaphore variable
    sem_t *sem1;

    //Create semaphore with value 1
    if ((sem1 = sem_open("semaphore1", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open1()\n");
        exit(1);
    }

    //Open file with write permissions
    FILE *numbers_ptr = fopen("Numbers.txt","w");

    //Verification of fopen
    if (numbers_ptr == NULL)
    {
        perror("Error in Numbers fopen().\n");
        exit(0);
    }

    //Random number generator
    srand ((unsigned) time (&t));

    //Fills file with random numbers between 0 and 100
    for ( i = 0; i < ARRAY_SIZE; i++)
    {
        fprintf(numbers_ptr,"%d\n",(rand() % MAXIMUM_NUMBER) + MINIMUM_NUMBER);   //Random number between 1 and 20
    }

    //Closes file pointer
    fclose(numbers_ptr);

    //Opens both reader and writer pointers in order to child copy numbers for the output txt
    reader = fopen("Numbers.txt","r");
    writer = fopen("Output.txt","w");

    //Creates childs
    pid = babymaker(NUMBER_OF_CHILDS);   

    if (pid > 0)        //Childs code.
    {
        //Decrements semaphore allowing only one child to read/write files
        down(sem1);

        //Auxiliar integer for saving numbers from the Numbers txt
        int aux;

        //Calculates the limits for each child
        int superior_limit = pid*(ARRAY_SIZE/NUMBER_OF_CHILDS);
        int inferior_limit = superior_limit-(ARRAY_SIZE/NUMBER_OF_CHILDS);

        while (!feof(reader) && inferior_limit < superior_limit)
        {
            fscanf(reader,"%d\n",&aux);  //Saves number in aux variable
            fprintf(writer,"%d\n",aux); //Prints number on output txt         
            inferior_limit ++;   
        }

        //Increments semaphore so one more child can read/write files
        up(sem1);

        //Finishes child
        exit(0);
    }else if(pid == 0){ //Father code

        //Waits for all childs
        for ( i = 0; i < NUMBER_OF_CHILDS; i++)
        {
            wait(NULL);
        }

        //Closes both file pointers
        fclose(writer);
        fclose(reader);

        //Opens output file with read permissions
        FILE *output_ptr = fopen("Output.txt","r");

        //Auxiliar string
        int aux2;

        //Verification of fopen
        if (output_ptr == NULL)
        {
            perror("Error in Output fopen().\n");
            exit(0);
        }

        while (!feof(output_ptr))
        {
            //Saves number to auxiliar variable
            fscanf(output_ptr,"%d\n",&aux2);
            //Prints number on screen
            printf("%d\n",aux2);
            fflush(stdout);
        }
            
        //Closes file pointer
        fclose(output_ptr);
            
        //Removes the semaphore
        if (sem_unlink("semaphore1")<0)
        {
            perror("No sem unlink()\n");
            exit(1);
        }
          
    }
    return 0;
}
