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
#include "info.h"      //Product structure
#include "database.h"   //Database structure
#define SIZE_OF_ARRAY 30
#define THREADS 3
pthread_mutex_t mutex; //MUTEX
pthread_cond_t turn; //TURN
int current_index; //CURRENT INDEX

void *thread_func(void *arg)
{
    //LOCK MUTEX
    if (pthread_mutex_lock(&mutex) != 0){
        perror("Error in mutex lock\n");
    }

    //Receives parameters
    data_base *data = (data_base *)arg;
    int n = data->parameter;
    int i;
    //Filling the array of the 
    int inicial, final;
    inicial = n*(SIZE_OF_ARRAY/THREADS); //WHERE SHOULD START
    final = (n+1)*(SIZE_OF_ARRAY/THREADS); //WHERE SHOULD END
    for(i= inicial; i< final; i++){
        if(data->globalData[i].id_h == 1){
            data->vec1[data->savedVec1] = data->globalData[i];
            data->savedVec1++;
        }else if(data->globalData[i].id_h ==2){
            data->vec2[data->savedVec2] = data->globalData[i];
            data->savedVec2++;
        }else{
            data->vec3[data->savedVec3] = data->globalData[i];
            data->savedVec3++; 
        }
    }
   
    if (pthread_mutex_unlock(&mutex) != 0){
        perror("Error in unlock\n");
    }

    pthread_exit((void *)NULL);
}

void *thread_computing(void *arg){
    //Receives parameters
    data_base *data = (data_base *)arg;
    int i, j, k;
    int extreme;
    if(data->parameter==0){
        extreme = data->savedVec1;
        printf("=========VEC1==========\n");
        for(i=0; i < extreme; i++){
            printf("Hypermarket Identifier: %d\n", data->vec1[i].id_h);
            printf("Product Identifier: %d\n", data->vec1[i].id_p);
            printf("Product Price: %d\n", data->vec1[i].p);
        }
        //Averages of cost of products
        if(extreme>5){
            int averageEachProduct = 0;
            int CalculatedProducts = 0;
            int counter = 0;
            for(i=0;i<data->savedVec1;i++){
                averageEachProduct = data->vec1[i].p;
                for(j=0;j<data->savedVec1;j++){
                    if(data->vec1[i].id_p==data->vec1[j].id_p){
                        averageEachProduct = data->vec1[j].p;
                        counter++;
                    }
                }
                for(k=0;k<10000;k++){ //already saved numbers
                    if(data->vec1[i].id_p==data->temporaryProducts[k]){
                        averageEachProduct = 0;
                        break;
                    }
                }
                if(averageEachProduct!=0){
                    printf("Average of product %d: %d", data->vec1[i].id_p, averageEachProduct);
                    CalculatedProducts++;
                    data->temporaryProducts[CalculatedProducts] = data->vec1[i].id_p;
                }
            }
            int average5Products = 0;
            for(i=0; i<extreme; i++){
                average5Products += data->vec1[i].p;
            }
            printf("Average of 5 products price Hypermarket 1", (average5Products/extreme));
        }else{
            printf("Not enough products to calculate the average\n");
        }
    }else if(data->parameter==1){
        extreme = data->savedVec2;
        printf("=========VEC2==========\n");
        for(i=0; i < extreme; i++){
            printf("Hypermarket Identifier: %d\n", data->vec2[i].id_h);
            printf("Product Identifier: %d\n", data->vec2[i].id_p);
            printf("Product Price: %d\n", data->vec2[i].p);
        }
         //Averages of cost of products
        if(extreme>5){
            int averageEachProduct = 0;
            int CalculatedProducts = 0;
            int counter = 0;
            for(i=0;i<data->savedVec2;i++){
                averageEachProduct = data->vec2[i].p;
                for(j=0;j<data->savedVec2;j++){
                    if(data->vec2[i].id_p==data->vec2[j].id_p){
                        averageEachProduct += data->vec2[j].p;
                        counter++;
                    }
                }
                for(k=0;k<10000;k++){ //already saved numbers
                    if(data->vec2[i].id_p==data->temporaryProducts[k]){
                        averageEachProduct = 0;
                        break;
                    }
                }
                if(averageEachProduct!=0){
                    printf("Average of product %d: %d", data->vec2[i].id_p, averageEachProduct);
                    CalculatedProducts++;
                    data->temporaryProducts[CalculatedProducts] = data->vec2[i].id_p;
                }
            }
            int average5Products = 0;
            for(i=0; i<extreme; i++){
                average5Products += data->vec2[i].p;
            }
            printf("Average of 5 products price Hypermarket 2", (average5Products/extreme));
        }else{
            printf("Not enough products to calculate the average\n");
        }
    }else{
        extreme = data->savedVec3;
        printf("=========VEC3==========\n");
        for(i=0; i < extreme; i++){
            printf("Hypermarket Identifier: %d\n", data->vec3[i].id_h);
            printf("Product Identifier: %d\n", data->vec3[i].id_p);
            printf("Product Price: %d\n", data->vec3[i].p);
        }
        //Averages of cost of products
        if(extreme>5){
            int averageEachProduct = 0;
            int CalculatedProducts = 0;
            int counter = 0;
            for(i=0;i<data->savedVec3;i++){
                averageEachProduct = data->vec3[i].p;
                for(j=0;j<data->savedVec3;j++){
                    if(data->vec3[i].id_p==data->vec3[j].id_p){
                        averageEachProduct += data->vec3[j].p;
                        counter++;
                    }
                }
                for(k=0;k<10000;k++){ //already saved numbers
                    if(data->vec3[i].id_p==data->temporaryProducts[k]){
                        averageEachProduct = 0;
                        break;
                    }
                }
                if(averageEachProduct!=0){
                    printf("Average of product %d: %d", data->vec3[i].id_p, (averageEachProduct/counter));
                    CalculatedProducts++;
                    data->temporaryProducts[CalculatedProducts] = data->vec3[i].id_p;
                }
            }
            int average5Products = 0;
            for(i=0; i<extreme; i++){
                average5Products += data->vec3[i].p;
            }
            printf("Average of 5 products price Hypermarket 3", (average5Products/extreme));
        }else{
            printf("Not enough products to calculate the average\n");
        }
    }

    pthread_exit((void *)NULL);
}

int main(void)
{
    //Array of product Data
    data_base vec;
    vec.parameter= -1;
    vec.savedVec1= 0;
    vec.savedVec2= 0;
    vec.savedVec3= 0;
    //Inserts-----------------------------------
    int i;
    printf("Inserting data...\n");
    sleep(1);
    //Randoms to fill the initial array
    time_t t;
    srand((unsigned)time(&t));
    for (i = 0; i < SIZE_OF_ARRAY; i++)
    {
        vec.globalData[i].id_h = rand() % 3 + 1;
        vec.globalData[i].id_p = rand() % 1000 + 1;
        vec.globalData[i].p = rand() % 50 + 1;
    }

    //Creating T1 T2 T3---------------------------
    pthread_t threads[THREADS];
    for (i = 0; i < THREADS; i++)
    {
        vec.parameter += 1;
        pthread_create(&threads[i], NULL, thread_func, (void *)&vec);
    }
    //JOINS T1 T2 T3---------------------------------
    for (i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    //Creating T4 T5 T6---------------------------
    pthread_t threads_computing[THREADS];
    vec.parameter= -1;
    for(i=0;i <THREADS; i++){
        vec.parameter += 1;
        pthread_create(&threads_computing[i], NULL, thread_computing, (void *)&vec);
    }
    
    //JOINS T1 T2 T3---------------------------------
    for (i = 0; i < THREADS; i++)
    {
        pthread_join(threads_computing[i], NULL);
    }

    return 0;
}