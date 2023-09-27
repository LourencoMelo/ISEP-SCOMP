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

#define ARRAY_SIZE1 10
#define ARRAY_SIZE2 5
#define NUMBER_OF_CHILDS 3

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

//Terminates the n child process
void babyFuneral(int n)
{
    int i, status, exitcode, p;
    for (i = 0; i < n; i++)
    {
        p = waitpid(0, &status, 0);
        if (WIFEXITED(status))
        {
            exitcode = WEXITSTATUS(status);
            printf("Baby %d has gone with exit code %d\n", p, exitcode);
        }
    }
}

typedef struct
{
    int n[ARRAY_SIZE1];
    int array_final[ARRAY_SIZE2];
    int maior;
    int checkMaior;
} shared_data_type;

int main(void)
{

    //UNLINKS
    sem_unlink("semteste1");
    sem_unlink("semteste2");
    sem_unlink("semteste3");
    //SHARED MEMORY---------------------------------
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    //Creates (O_CREAT) shared memory area with name “/shmtest, and read/write permissons (O_RDWR), and open for user to read (S_IRUSR) and write (S_IWUSR).
    fd = shm_open("/shmtest", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    //Verification of error
    if (fd == -1)
    {
        return fd;
    }

    //Defines a data_size bytes size
    int error = ftruncate(fd, data_size);
    if (error == -1)
    {
        perror("Error in ftruncate \n");
        return error;
    }

    //Maps the area in an address decided by the OS (NULL), requiring to use all the size (data_size), with read and write permissions(PROT_READ|PROT_WRITE), consistent with open(continues)
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //Verification of error
    if (shared_data == MAP_FAILED)
    {
        return -1;
    }

    //SEMAFORES------------------------------------------
    sem_t *semteste1;
    sem_t *semteste2;
    sem_t *semteste3;

    if ((semteste1 = sem_open("semteste1", O_CREAT, 0777, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open() east\n");
        exit(EXIT_FAILURE);
    }

    if ((semteste2 = sem_open("semteste2", O_CREAT, 0777, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open() east\n");
        exit(EXIT_FAILURE);
    }

    if ((semteste3 = sem_open("semteste3", O_CREAT, 0777, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open() east\n");
        exit(EXIT_FAILURE);
    }

    //===========IMPLEMENTATION==========================
    //randoms
    time_t t;
    srand ((unsigned) time (&t));
    int i;
    for (i = 0; i < ARRAY_SIZE1; i++)
    {
        shared_data->n[i] = rand () % 50 + 1;
    }
    
    shared_data->maior = 0;
    shared_data->checkMaior = 0;

    int pid;
    pid = babymaker(NUMBER_OF_CHILDS);

    if (pid > 0)
    { //childs code
        if (pid == 1)
        { //P1 começa sempre primeiro
            for (i = 0; i < ARRAY_SIZE2; i ++)
            {
                sem_wait(semteste1);
                shared_data->array_final[i] = shared_data->n[i] * shared_data->n[i +1];
                if (shared_data->array_final[i] > shared_data->maior)
                {
                    shared_data->maior = shared_data->array_final[i];
                }
                sem_post(semteste2);
                sem_post(semteste3);
            }
            exit(0);
        }

        if (pid == 2)
        { //P2
            for (i = 0; i < ARRAY_SIZE2; i ++) //problema em fill dos arrays so descubri q podia fazer com += 2 no final n tive tempo para mudar
            {
                sem_wait(semteste2);
                shared_data->array_final[i+1] = shared_data->n[i+2] * shared_data->n[i+3];
                if (shared_data->array_final[i] > shared_data->maior)
                {
                    shared_data->maior = shared_data->array_final[i];
                }
                sem_post(semteste1);
                sem_post(semteste3);
            }
            exit(0);
        }
        if (pid == 3)
        { //P3
            int acabar;
            sem_wait(semteste1);
            sem_wait(semteste2);
            if (shared_data->maior > shared_data->checkMaior)
            {
                shared_data->checkMaior = shared_data->maior;
                printf("Foi encontrado um numero maior\n");
            }
            sem_getvalue(semteste3, &acabar);
            if (acabar == ARRAY_SIZE2)
            {
                exit(0);
            }
            sem_post(semteste2);
            sem_post(semteste1);
        }
    }

    if (pid == 0)
    {
        int j;
        int k;
        for (j = 0; j < NUMBER_OF_CHILDS; j++)
        {
            wait(NULL);
        }
        babyFuneral(NUMBER_OF_CHILDS);
        for (k = 0; ARRAY_SIZE2; k++)
        {
            printf("Array: %d\n", shared_data->array_final[k]);
        }
        printf("Numero maior: %d\n", shared_data->maior);
    }

    //Disconnects the shared memory area from the process address space (página 9 TP)
    if(munmap(shared_data, sizeof(shared_data_type))<0){
        perror("No munmap()!\n");
        exit(0);
    }
    //Closes the file descriptor (página 9 TP)
    if(close(fd)<0){
        perror("No close()!\n");
        exit(0);
    }

    return 0;
}