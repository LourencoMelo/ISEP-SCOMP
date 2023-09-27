#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main() {

    const int upperlimit = 5;
    const int range = 4;

    const int CONTINUE = 1;
    const int TERMINATE = 0;

    const int InicialWallet = 20;

    const int WIN_CREDIT = 10;
    const int LOSE_CREDIT = -5;

    //One for the father
    int fdDirectedToFather[2];

    //0ne for the child
    int fdDirecetedToChild[2];

    // Pipe from father created
    if (pipe(fdDirectedToFather) == -1 || (pipe(fdDirecetedToChild) == -1 )) {
        perror("Pipe UP failed!");
        exit(-1);
    }


    // Fork a new process
    pid_t pid = fork();
    // Check for errors
    if (pid < 0) {
        perror("Fork with errors!");
        exit(-1);
    }

    // Father
    if (pid > 0) {
        srand(time(NULL));

        close(fdDirectedToFather[1]);
        close(fdDirecetedToChild[0]);
        int wallet = InicialWallet;
        while (wallet > 0) { //While child has money
            int randomNumber = (rand() % upperlimit) + (upperlimit - range);  // random 1 to 5
            write(fdDirecetedToChild[1], &CONTINUE, 4);
            int bet;
            read(fdDirectedToFather[0], &bet, 4);
            if (bet == randomNumber) {
                printf("The child Won. The bet: %d\n", bet);
                wallet = wallet + WIN_CREDIT;
            } else {
                printf("The child Lost. The bet: %d\n", bet);
                wallet = wallet + LOSE_CREDIT;
            }
            printf("Wallet: %d\n", wallet);
            if(wallet > 0){
                printf("-----NEXT BET------\n");
            }
        }
        write(fdDirecetedToChild[1], &TERMINATE, 4); //Say to the child to stop creating numbers
        close(fdDirectedToFather[0]);
        close(fdDirecetedToChild[1]);
    }

    // Child
    if (pid == 0) {
        srand(time(NULL) * getpid());

        close(fdDirectedToFather[0]);
        close(fdDirecetedToChild[1]);
        int shouldContinue = 1;

        while (shouldContinue) { //will end if the father writes TERMINATE
            read(fdDirecetedToChild[0], &shouldContinue, 4); 
            int randomNumber = (rand() % upperlimit) + (upperlimit - range);  // random 1 to 5
            write(fdDirectedToFather[1], &randomNumber, 4);
        }

        close(fdDirectedToFather[1]);
        close(fdDirecetedToChild[0]);
        exit(0);
    }

printf("------STATUS----------\n");
    int status;
    pid_t pidExit = wait(&status);
    int exitStatus = WEXITSTATUS(status);
    int piid = getppid();
    printf("O processo com o PID = %d terminou com valor = %d   %d\n", pidExit, exitStatus, piid);
    exit(0);
}