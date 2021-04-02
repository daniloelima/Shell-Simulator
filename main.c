#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char* novosComandos(){
    char strAux[100];
    char* novosComandos;

    fgets(strAux, 100, stdin);

    novosComandos = strdup(strAux);
    novosComandos[strlen(novosComandos) - 1] = '\0';

    return novosComandos;
}

int main(){
    while(1){
        printf("vsh> ");

        char* comandos = novosComandos();

        printf("%s\n", comandos);
    }

    return 0;
}