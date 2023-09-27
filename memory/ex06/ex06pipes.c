#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h> /* For constants O_* */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define sizeOfTheArray 10000000

int main(void){
    //initializing clock_t variables
    clock_t start_time, end_time;
    //i of the for and initializing the array
    int i, *array = calloc(sizeOfTheArray, sizeof(int));
    //Creating vector for de pipe(read and write)
    int fd[2], checkErrorPipe;
    //initializing pid
    pid_t pid;
    //Starts timer
    start_time = clock();

    //initializing the array with random numbers up to 5
    srand((unsigned) time(NULL));
	for(i = 0; i < sizeOfTheArray; i++){
		array[i] = rand() % 6;
	}

    checkErrorPipe = pipe(fd);
    if(checkErrorPipe == -1){
        perror("Pipe done with errors");
        return 1; //Return with errors
    }


    if((pid = fork()) == -1){
        perror("Fork done with error");
        exit(EXIT_FAILURE);
    }

    //If its the child...
    if(pid == 0){
        int j; //j for the next for
        //close the reader to save resources
        close(fd[0]);
        for(j = 0; j < sizeOfTheArray; j++) {
            //Writes on the pipe the array
            write(fd[1], &array[j], sizeof(array[j])); 
            //printf("%d\n", array[j]);
        }
        close(fd[1]); //Closes the writer
        exit(0); //Exit with no erros
    }
    //Parent waits for the child to end
    //Closes the writer to save resources
    close(fd[1]);

    //for to the parent to read
    for(i = 0; i < sizeOfTheArray; i++){
        read(fd[0], &array[i], sizeof(array[i]));
    }
    //Closes the reader
    close(fd[0]);
    //Registers the end of the clock
    end_time = clock();
    //Total time
    float total_time = (float)(end_time - start_time) / CLOCKS_PER_SEC;
    //Prints the final result
    printf("Time of the pipes process: %f\n", total_time);
    printf("Exiting the program...\n");
    wait(NULL);
    return 0;
}