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

#define ARRAY_SIZE 1000 //ARRAY SIZE
#define NUMBER_OF_THREADS 5 //NUMBER OF THREADS

int array_initial[ARRAY_SIZE]; //INITIAL ARRAY
int array_results[ARRAY_SIZE]; //RESULTS ARRAY
pthread_mutex_t mutex; //MUTEX
pthread_cond_t turn; //TURN
int current_index; //CURRENT INDEX

void *thread_func(void *arg){
    //Receives parameters
    int *numberaux = (int *)arg;//Converts the parameter
    int n = *numberaux;
    int i;
    //Filling the array of the 
    int inicial, final;
    inicial = n*(ARRAY_SIZE/NUMBER_OF_THREADS); //WHERE SHOULD START
    final = (n+1)*(ARRAY_SIZE/NUMBER_OF_THREADS); //WHERE SHOULD END
    for(i= inicial; i< final; i++){
        array_results[i]=(array_initial[i]*10)+2;
    }
    //LOCK MUTEX
    if (pthread_mutex_lock(&mutex) != 0){ //Dá lock as threads q vem para tras 
        perror("Error in mutex lock\n");
    }
    
    while (*numberaux != current_index) //enquanto o thread id n for a que deve ser printed ele vai dar release as outras threads ate que venha a q deve ser printed
    {
        pthread_cond_wait(&turn, &mutex); //unlock ao mutex so da a 1
        pthread_cond_signal(&turn); //liberta outra thread presa no wiat da variavel condicional impedindo que o programa fique preso
    }
    //PRINT RESULTS
    for(i= inicial; i< final; i++){
        printf("Result %d: %d\n", i, array_results[i]);
    }

    current_index++;
    //UNLOCK MUTEX
    pthread_cond_signal(&turn); //tira todos os cond_wait 
    if (pthread_mutex_unlock(&mutex) != 0){
        perror("Error in unlock\n");
    }

    pthread_exit((void *)NULL);
}

int main(void){
    //Randoms to fill the initial array
    time_t t;
    srand ((unsigned) time (&t));
    int i;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        array_initial[i]=rand () % 10 + 1;
    }
    pthread_cond_init(&turn, NULL);
    pthread_mutex_init(&mutex, NULL);
    current_index=0;
    //initializes the thread
    pthread_t thread[NUMBER_OF_THREADS];
    int parameter[NUMBER_OF_THREADS];
    //Creates the thread with the function and send a parameter
    for(i=0; i < NUMBER_OF_THREADS; i++){
        parameter[i] = i;
        pthread_create(&thread[i], NULL, thread_func, (void *)&parameter[i]);
    }
    //Waits for a specific thread to terminate, blocking the calling thread 
    for(i=0; i < NUMBER_OF_THREADS; i++){
        pthread_join(thread[i], NULL);
    }
    //DESTROY COND AND MUTEX
    pthread_cond_destroy(&turn);
    pthread_mutex_destroy(&mutex);

    return 0;
}