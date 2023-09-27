#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 1000
int main()
{
    int numbers[ARRAY_SIZE]; /* array to lookup */
    int n;                   /* the number to find */
    time_t t;                /* needed to initialize random number generator (RNG) */
    int i, count = 0, status;
    pid_t pid;
    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < ARRAY_SIZE; i++)
        numbers[i] = rand() % 10;

    /* initialize n */
    n = 5;
    printf("%d \n", n);
    pid = fork();
    if(pid == 0){
        for(i = 0; i < ARRAY_SIZE/2; i++){
            if(numbers[i] == n){
               count ++;
            }
        }
        exit(count); //da return do count pelo exit ao acabar o seu processo
    }else{
        for(i = n/2; i < ARRAY_SIZE; i++){
            if(numbers[i] == n){
                count ++;
            }
        }
        wait(&status); //espera que o processo filho acabe para acabar tambem o seu processo
    }
    if(WIFEXITED(status)){
        count = count + WEXITSTATUS(status); // o count vem do filho e o WEXITSTATUS(STATUS) = count do filho
        printf("Numbers found: %d \n", count);
    }
    return 0;
}