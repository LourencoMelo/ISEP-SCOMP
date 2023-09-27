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
#include <pthread.h>        //threads
#define THREADS_TOFILL 2     //THREADS TO FILL THE FIRST 2 MATRIX
#define INDEX 3  //INDEX OF THE MATRIXS

int MATRIX1[INDEX][INDEX]; //MATRIX1
int MATRIX2[INDEX][INDEX];  //MATRIX2
int FINAL_MATRIX[INDEX][INDEX]; //FINAL_MATRIX

//FILL MATRIX1
void *fill_matrix1(void *arg){
    int i, j;
    for(i=0 ; i < INDEX; i++){
        for(j=0; j < INDEX; j++){
            MATRIX1[i][j] = i+2;
        }
    }
    pthread_exit((void*)NULL);
}

//FILL MATRIX2
void *fill_matrix2(void *arg){
    int i, j;
    for(i=0 ; i < INDEX; i++){
        for(j=0; j < INDEX; j++){
            MATRIX2[i][j] = i+3;
        }
    }
    pthread_exit((void*)NULL);
}

//PRINT MATRIX1
void print_MATRIX1(){
    int i, j;
    for(i=0 ; i < INDEX; i++){
        for(j=0; j < INDEX; j++){
            printf("%d\n",MATRIX1[i][j]);
        }
        printf("NextLine\n");
    }
    printf("End of the MATRIX1...\n");
}

//PRINT MATRIX2
void print_MATRIX2(){
    int i, j;
    for(i=0 ; i < INDEX; i++){
        for(j=0; j < INDEX; j++){
            printf("%d\n",MATRIX2[i][j]);
        }
        printf("NextLine\n");
    }
    printf("End of the MATRIX2...\n");
}

//MULTIPLY THE LINE IN QUESTION
void *multiply_line_by_row(void *arg){
    int *line = (int*)arg;
    int i = *line;

    int j, k;
    for(j=0; j < INDEX;j++){
        FINAL_MATRIX[i][j]=0;
        for(k=0; k< INDEX; k++){
            FINAL_MATRIX[i][j] += MATRIX1[i][k] * MATRIX2[k][j];
        }
    }
    pthread_exit((void*)NULL);
}

//PRINT FINAL MATRIX
void print_FINAL_MATRIX(){
    int i, j;
    printf("FINAL MATRIX:\n");
    for(i=0 ; i < INDEX; i++){
        for(j=0; j < INDEX; j++){
            printf("%d\n",FINAL_MATRIX[i][j]);
        }
        printf("NextLine\n");
    }
    printf("End of the FINALMATRIX\n");
}



int main(void){
    //THREADS TO FILL
    pthread_t threads_fill[THREADS_TOFILL];
    int i;
    //CREATE THREADS
    pthread_create(&threads_fill[0], NULL, fill_matrix1,(void *)NULL);
    pthread_create(&threads_fill[1], NULL, fill_matrix2,(void *)NULL);
    //JOIN OF THE THREADS
    for(i=0; i <THREADS_TOFILL; i++){
        pthread_join(threads_fill[i], NULL);
    }
    //PRINTS OF MATRIX 1 AND 2
    printf("Matrix 1:\n");
    print_MATRIX1();
    printf("Matrix 2:\n");
    print_MATRIX2();
    //EACH THREADS MULTIPLIES TO GET A LINE OF THE FINAL MATRIX
    pthread_t thread_mult_line[INDEX];
    int parameter[INDEX];
    for(i=0; i < INDEX; i++){
        parameter[i]=i; //SENDS THE LINE
        pthread_create(&thread_mult_line[i], NULL, multiply_line_by_row, &parameter[i]);
    }
    //JOINS OF THE LAST THREADS
    for(i=0; i <INDEX; i++){
        pthread_join(thread_mult_line[i], NULL);
    }
    //PRINT THE FINAL MATRIX
    print_FINAL_MATRIX();
         
    return 0;
}