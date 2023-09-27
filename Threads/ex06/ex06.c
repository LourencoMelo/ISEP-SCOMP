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
#include <pthread.h>   //threads

void *thread_func(void *arg)
{
    int *numberaux = (int *)arg;//Converts the parameter
    int n = *numberaux;
    int i, m=0, flag = 0;
    //Function to check if the number is prime or not
    m = n / 2;
    for (i = 2; i <= m; i++)
    {
        if (n % i == 0)
        {
            printf("Number is not prime\n");
            flag = 1;
            break;
        }
    }
    if (flag == 0)
        printf("Number is prime\n");

    pthread_exit((void *)NULL);
}

int main(void)
{
    int scanner;
    printf("Enter a number!\n");
    scanf("%d", &scanner);
    //initializes the thread
    pthread_t thread;
    //Creates the thread with the function and send a parameter
    pthread_create(&thread, NULL, thread_func, (void *)&scanner);
    //Waits for a specific thread to terminate, blocking the calling thread 
    pthread_join(thread, NULL);

    return 0;
}
