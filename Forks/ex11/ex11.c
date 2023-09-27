#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 1000
int main(){
    int numbers[ARRAY_SIZE];
    int n=5,i,j,counter = 0;
    time_t t;
    pid_t pid;
    int size_Divided
    size_Divided = ARRAY_SIZE / n;

    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < ARRAY_SIZE; i++)
        numbers[i] = rand() % 10;
    
    //Function
    printf("Number of processes %d \n", n);
    for(i=0;i < n;i++){
        size_Divided = size_Divided*(i+1);
        pid = fork();
        if(p==0){
            for(j=size_Divided*i ; j < sizeDivided ;j++){
                numbers[j]>counter;
                counter = numbers[j];
            }
            exit(count);
            if(WIFEXITED(status)){
                counter = counter + counter;
            }    
        }
    }
}