#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h>    /* For constants O_* */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 10
#define OPERATIONS 30

int fd;

typedef struct
{
    int buffer[BUFFER_SIZE];
    int size;
    int head;
    int tail;
    int full;
} shared_data_type;

shared_data_type *menuStarter()
{
    int checkError;
    int data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    fd = (shm_open("/shmtest", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR));
    /*Creates (O_CREAT) shared memory area (with error if
    existis because of O_EXCL) with name “/shmtest, and
    read/write permissons (O_RDWR), and open for user to
    read (S_IRUSR) and write (S_IWUSR).
    */
    if (fd == -1)
    {
        perror("Error in the shm_open!");
        exit(-1);
    }
    checkError = ftruncate(fd, data_size);
    /*
    Defines a n bytes size
    */
    if (checkError == -1)
    {
        perror("Error in the ftruncate!");
        exit(-1);
    }
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /*
        Maps the area in na address decided by the OS
        (NULL), with the specific data_size, with
        read and write permissions (PROT_READ |
        PROT_WRITE) , consistente with open (continues)
    */
    if (shared_data == MAP_FAILED)
    {
        perror("Error in the shared_data!\n");
        exit(-1);
    }
    //Define and initializes the variables of the structure
    shared_data->size = BUFFER_SIZE;
    shared_data->head = 0;
    shared_data->tail = 0;
    shared_data->full = 0;

    return shared_data;
}

int BufferIsFull(shared_data_type *shared_data)
{
    return shared_data->full;
}

int BufferIsEmpty(shared_data_type *shared_data)
{
    int temp = 0;
    if ((shared_data->head == shared_data->tail) && (shared_data->full != 1))
    {
        temp = 1;
    }
    return temp;
}

void CB_WriteToBuffer(shared_data_type *shared_data, int inserted)
{ //shared_data_type *shared_data
    if (BufferIsFull(shared_data))
    { //fazer esta funcao
        printf("Buffer is Full\n");
        sleep(1);
    }
    else
    {
        shared_data->buffer[shared_data->head] = inserted;
        printf("Write Buffer: %d\n", shared_data->buffer[shared_data->head]);
        shared_data->head = (shared_data->head + 1) % BUFFER_SIZE;
        if (shared_data->head == shared_data->tail)
        {
            shared_data->full = 1;
        }
    }
}

void CB_ReadToBuffer(shared_data_type *shared_data, int *inserted)
{
    if (BufferIsEmpty(shared_data))
    { //fazer esta funcao
        printf("Buffer is Empty\n");
        sleep(1);
    }
    else
    {
        *inserted = shared_data->buffer[shared_data->tail];
        printf("Read Buffer: %d\n", shared_data->buffer[shared_data->tail]);
        shared_data->tail = ((shared_data->tail) + 1) % BUFFER_SIZE;
        shared_data->full = 0;
    }
}

int main(int argc, char *argv[])
{
    int i;
    pid_t p;
    shared_data_type *shared_data = menuStarter();
    int data_size = sizeof(shared_data_type);
    p = fork();
    if (p == -1)
    {
        perror("Error in fork!");
        exit(EXIT_FAILURE);
    }
    if (p > 0)
    { //father process
        for (i = 0; i < OPERATIONS; i++)
        {
            CB_WriteToBuffer(shared_data, i);
        }
        wait(NULL);
    }

    if (p == 0)
    {

        for (i = 0; i < OPERATIONS; i++)
        {
            int inserted;
            CB_ReadToBuffer(shared_data, &inserted);
        }

        //closes of the child
        if (munmap(shared_data, data_size))
        {
            perror("Error in munmap!");
            return 1;
        }
        if (close(fd) == -1)
        {
            perror("Error in close!");
            return 1;
        }
        exit(0);
    }

    //closes of the father
            if (munmap(shared_data, data_size))
            {
                perror("Error in munmap!");
                return 1;
            }
            if (close(fd) == -1)
            {
                perror("Error in close!");
                return 1;
            }
    return 0;
}