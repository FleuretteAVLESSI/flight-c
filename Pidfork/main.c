#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    printf("Je suis un processus qui va en creer un autre\n");
    pid_t pid = fork();
    //vérification d'erreur
    if(pid < 0){
        fprintf(stderr,"une erreur s'est produite '(%d)\n",errno);
        exit(1);
    }
    //traitement fait par le fils
    else if (pid ==0) {
        printf ("Je suis le fils, le pid de mon pere est %d, et le mien est %d\n",getppid(),getpid());
    }
    //traitement fait par le pere
    else {
        printf("Je suis le pere, mon pid est %d, celui de mon fils est %d\n",getpid(), pid);
    }
    //du code executer par les deux
    printf("Qui suis-je?%d\n",getpid());
    return 0;
}
