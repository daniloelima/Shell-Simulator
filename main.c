#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Comando.h"


extern void imprimeJacare();

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
        char* str = strtok(comandos, "|");

        while(str != NULL){
            Comando* comando = inicializaComando();

            preencheArgumentos(comando, str);

            imprimeComando(comando);

            char** args = retornaArgumentos(comando);

            int pid = fork();
            if(pid == 0){
                execvp(args[0], args);
            }else{
                liberaComando(comando);
                str = strtok(NULL, "|");
                printf("str: %s", str);
            }
            wait(NULL);
        }

        return 0;
    }

    return 0;
}