//
// Created by vitor on 02/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Comando.h"

struct comando{
    char** argumentos;
    int numArgumentos;
};

Comando* inicializaComando(){
    Comando* novoComando = (Comando*) malloc(sizeof(Comando));

    novoComando->argumentos = (char**) malloc(sizeof(char**)*30);

    for(int i = 0; i < 30; i++){
        novoComando->argumentos[i] = (char*) malloc(sizeof(char*)*50);
    }

    novoComando->numArgumentos = 0;

    return novoComando;
}

char* obtemArgumento(char* comando){
    char* novoArgumento = malloc(sizeof(char) * 20);

    int i = 0, j = 0;
    if(comando[0] == ' '){
        while(comando[i] == ' ') i++;
    }

    while(comando[i] != ' ') {
        novoArgumento[j] = comando[i];
        i++;
        j++;
    }
    novoArgumento[j] = '\0';

    return novoArgumento;
}

void preencheArgumentos(Comando* c, char* comando){
    char* buffer;
    char* aux = strtok_r(comando," ",&buffer);

    while(aux != NULL){
        c->argumentos[c->numArgumentos] = strdup(aux);

        aux = strtok_r(NULL," ",&buffer);
        c->numArgumentos++;
    }

    c->argumentos[c->numArgumentos] = NULL;
    c->numArgumentos++;
}