#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h> /* For constants O_* */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int code;
    char description[30];
    int price;
} shared_data_type;

int main(int argc, char *argv[]){
    int checkError;
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    shm_unlink ("/shmtest");
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
    printf("Write the code of the product\n");
    scanf("%d",&shared_data->code);
    printf("Write the string of the description\n");
    scanf("%s",shared_data-> description);
    printf("Write the price of the product:\n");
    scanf("%d",&shared_data->price);

    return 0;
}

