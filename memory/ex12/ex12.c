#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h> /* For constants O_* */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 50
#define NR_DISC 10

typedef struct{
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
    int available;
}shared_data_type;

int main(int argc, char *argv[]){
    int checkError, i;
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    shm_unlink ("/shmtest");
    pid_t p;

    fd = shm_open("/shmtest", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    /*Creates (O_CREAT) shared memory area (with error if
    existis because of O_EXCL) with name “/shmtest, and
    read/write permissons (O_RDWR), and open for user to
    read (S_IRUSR) and write (S_IWUSR).
    */
    if(fd == -1){
        perror("Error in the shm_open!");
        return -1;
    }
    checkError = ftruncate(fd,data_size);
    /*
    Defines a n bytes size
    */
    if(checkError == -1){
        perror("Error in the ftruncate!");
        return -1;
    }
    shared_data = (shared_data_type*)mmap(NULL,data_size, PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);
    /*
        Maps the area in na address decided by the OS
        (NULL), with the specific data_size, with
        read and write permissions (PROT_READ |
        PROT_WRITE) , consistente with open (continues)
    */
   if(shared_data == MAP_FAILED){
       perror("Error in the shared_data!\n");
       return -1;
   }
    shared_data->available = 0;
    p = fork();
    if(p == -1){
        perror("Error in fork!");
        exit(EXIT_FAILURE);
    }
    if(p > 0){ //father process
        shared_data->numero = 10;
        strcpy(shared_data->nome, "Lourenço Melo");
        for(i = 0; i < NR_DISC; i++){
            shared_data->disciplinas[i] = (i*2);//fills the array
        }
        shared_data->available = 1;
    }else{ //child process
        while (!shared_data->available);
        int arrayCal[NR_DISC];
        int j;
        for(j = 0; j <NR_DISC; j++){
            arrayCal[j] = shared_data->disciplinas[j]; //coppies the array of the structure
        }
        int smaller = arrayCal[0];
        int bigger = arrayCal[0];
        int total = arrayCal[0];
        int k, value = 0;
        for(k = 1; k<NR_DISC ; k++){
            value = arrayCal[k];
            total += value;
            if(value > bigger){
                bigger = value;
            }
            if(value < smaller){
                smaller = value;
            }
        }
        int average;
        average = (total / NR_DISC); //calculates the average
        //prints
        printf("Student ID: %d\n", shared_data->numero);
        printf("Name of the student: %s\n", shared_data->nome);
        printf("Average: %d \n", average);
        printf("Bigger: %d\n", bigger);
        printf("Smaller %d\n", smaller);
        //closes of the child
        if (munmap(shared_data, data_size)){
            perror("Error in munmap!");
            return 1;
        }
        if (close(fd) == -1){
            perror("Error in close!");
            return 1;
        }
        exit(0);

    }
    wait(NULL);
    //closes of the father
    if (munmap(shared_data, data_size)){
            perror("Error in munmap!");
            return 1;
    }
    if (close(fd) == -1){
        perror("Error in close!");
        return 1;
    }
    if(shm_unlink("/shmtest")){
        perror("Error in shm_unlink");
        return 1;
    }
    return 0;
}