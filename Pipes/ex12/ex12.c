#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int baby_maker(int child_number){
    int pid;
    int i;
    for(i = 0; i < child_number; i++){
        pid = fork();
        if(pid == 0){
            return i + 1; //sends de id
        }else if (pid == -1){
            return -1;
        }
    }
    //father returns 0
    return 0;
}

//Ends Childs
void babyFuneral(int n){
    int i, status, exitcode, p;
    for(i=0; i<n; i++){
        p = waitpid(0,&status,0);
        if(WIFEXITED(status)){
            exitcode = WEXITSTATUS(status);
            printf("Baby %d has gone with exit code %d\n", p, exitcode);
        }
    }
}

typedef struct{
	int id;
	int cost;
	char name[10];
} products;

int checkDataBase(int idproduct, products* dataBase){
    int i;
    for(i=0; i < 5; i++){
        if(dataBase[i].id == idproduct){
            return idproduct;
        }
    }
    return -1;
}


int main(void){
    
    products dataBase[5];
	dataBase[0].id = 1;
	strcpy(dataBase[0].name, "Sopa");
	dataBase[0].cost = 2;
	dataBase[1].id = 2;
	strcpy(dataBase[1].name, "Melancia");
	dataBase[1].cost = 2;
	dataBase[2].id = 3;
	strcpy(dataBase[2].name, "Coco");
	dataBase[2].cost = 1;
	dataBase[3].id = 4;
	strcpy(dataBase[3].name, "Morango");
	dataBase[3].cost = 3;
	dataBase[4].id = 5;
	strcpy(dataBase[4].name, "Vinho");
	dataBase[4].cost = 14;



    const int NUMBER_OF_READERS = 5; 
    int fd[2]; //matrix of fds

    int idChild = baby_maker(NUMBER_OF_READERS);
    int idProduct;
    int upperlimit = 5;
    int range = 4;

    pipe(fd);
    idChild = baby_maker(NUMBER_OF_READERS);

        if(idChild > 0){ //Child
            wait(NULL);
            idProduct = (rand() % upperlimit) + (upperlimit - range);  // random 1 to 5
            products produto1;
            int sp = write(fd[1], &idProduct, 4); //send the id of the product
            if(sp == -1){
                printf("Errou a escrever!");
            }
            close(fd[1]);
            read(fd[0], &produto1, sizeof(produto1));
            close(fd[0]);
            printf("\n BAR CODE READER: %d", idChild);
            printf("\n Id Product: %d \n Name of the product: %s \n Cost: %d \n ", produto1.id, produto1.name, produto1.cost);
            exit(0);
        }

        if(idChild == 0){ //Father
            read(fd[0], &idProduct, 4);
            int reached;
            reached = checkDataBase(idProduct, dataBase); //check if the product read from the child is on the data base
            int wr = write(fd[1], &dataBase[reached], sizeof(dataBase));
            if(wr == -1){
                printf("Errou a escrever!");
            }
            babyFuneral(NUMBER_OF_READERS);
        }


        //Check if fork has errors
        if((idChild == -1)){
            perror("Fork with error");
            exit(1);
        }
    return 0;
}