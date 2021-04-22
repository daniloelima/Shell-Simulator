//
// Created by bonella on 22/04/2021.
//

#include "VSH.h"
#include <stdio.h>
#include <stdlib.h>

struct vsh{
    Comando** comandos;
    int numComandos;
};

VSH* initVSH(void){
    VSH* novoVSH = (VSH*) malloc(sizeof(VSH));

    novoVSH->numComandos = 0;
    novoVSH->comandos = (Comando**) malloc(sizeof(Comando*)*5);

    return novoVSH;
}

static char* novosComandos(){
    char strAux[100];
    char* novosComandos;

    fgets(strAux, 100, stdin);

    novosComandos = strdup(strAux);
    novosComandos[strlen(novosComandos) - 1] = '\0';

    printf("Comandos: %s\n", novosComandos);

    return novosComandos;
}

void adicionaComando(VSH* vsh , Comando* comando){
    vsh->comandos[vsh->numComandos] = comando;
    vsh->numComandos++;
}

void leComandos(VSH* listacomandos){
    char* comandos = novosComandos();
    char* str = strtok(comandos, "|");

    while(str != NULL){
        Comando* comando = inicializaComando();

        preencheArgumentos(comando, str);

        adicionaComando(listacomandos,comando);

        str = strtok(NULL, "|");
    }
}

void executaComandos(VSH* vsh){
    if(vsh->numComandos == 1){ // FOREGROUND
        int pid = fork();
        if(pid == 0) {
            char **args = retornaArgumentos(vsh->comandos[0]);
            execvp(args[0], args);
        }
        wait(NULL);
    }else{ // BACKGROUND
        for(int i = 0 ; i < vsh->numComandos; i++){
            int pid = fork();
            if(pid == 0){
                char** args = retornaArgumentos(vsh->comandos[i]);
                execvp(args[0], args);
                //botar os pipes
            }
        }
    }
}

void imprimeComandos(VSH* vsh){
    for(int i = 0; i < vsh->numComandos; i++){
        imprimeComando(vsh->comandos[i]);
    }
}

void liberaComandos(VSH* vsh){
    for(int i = 0; i < vsh->numComandos; i++){
        free(vsh->comandos[i]);
    }

    free(vsh->comandos);
    free(vsh);
}
