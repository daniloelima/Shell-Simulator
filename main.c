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

    printf("Comandos: %s\n", novosComandos);

    return novosComandos;
}

int main(){

    while(1){
        printf("vsh> ");

        char* comandos = novosComandos();
        char* comando = strtok(comandos, "|");

        while(comando != NULL){
            char** argumentosComandos = obtemArgumentos(comando);

            for(int i = 0; i < )

            comando = strtok(NULL, "|");
        }

        return 0;
    }

    return 0;
}