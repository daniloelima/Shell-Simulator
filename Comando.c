//
// Created by vitor,enzo,danilo on 02/04/2021.
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

    novoComando->argumentos = (char**) malloc(sizeof(char*)*30);

    for(int i = 0; i < 30; i++){
        novoComando->argumentos[i] = (char*) malloc(sizeof(char)*50);
    }

    novoComando->numArgumentos = 0;

    return novoComando;
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

char** retornaArgumentos(Comando* c){
    return c->argumentos;
}

int retornaNumArgumentos(Comando* c){
    return c->numArgumentos;
}

void imprimeComando(Comando * c){
    for(int i = 0; i < c->numArgumentos; i++) {
        printf("%s ", c->argumentos[i]);
    }

    printf("Numero de argumentos : %d\n",c->numArgumentos);
}

void liberaComando(Comando* c){
    for(int i = 0; i < c->numArgumentos; i++){
        free(c->argumentos[i]);
    }

    free(c->argumentos);
    free(c);
}

/*
int contaCaracter(char caracter){
    int i, count;
    for (i=0, count=0; str[i]; i++){
        count += (str[i] == caracter);
    }

    return count;
};
*/