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
#include "info.h"      //Struct with the info
#define MAX_THREADS 5  //Max number of threads

void *thread_func(void *arg)
{
    personal_data *data = (personal_data *)arg;
    printf("Content:\n");
    printf("Number: %d\n", data->number);
    printf("Name: %s\n", data->name);
    printf("Grade: %d\n", data->grade);
    pthread_exit((void *)NULL);
}

int main(void)
{
    int numberOfThreads;
    int i;
    printf("Starting program...\n");
    sleep(1);
    printf("Enter the number of threads\n");
    scanf("%d", &numberOfThreads);
    pthread_t threads[numberOfThreads];
    personal_data array[numberOfThreads];
    if (numberOfThreads >= 1 && numberOfThreads <= MAX_THREADS)
    {
        int i;
        printf("Filling the structure...\n");
        for (i = 0; i < numberOfThreads; i++)
        {
            printf("Strucutre %d\n", i);
            printf("Number:\n");
            scanf("%d", &array[i].number);
            printf("Name:\n");
            scanf("%s", array[i].name);
            printf("Grade:\n");
            scanf("%d", &array[i].grade);
        }

        for (i = 0; i < numberOfThreads; i++)
        {
            if (pthread_create(&threads[i], NULL, thread_func, (void *)&array[i]))
            {
                perror("Error in pthread_create\n");
            }
        }
    }
    else
    {
        perror("Number of threads are invalid!\n");
    }

    for (i = 0; i < numberOfThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("All threads terminated!\n");
    return 0;
}
