//
// Created by vitor,enzo,danilo on 02/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Comando.h"

#define MAX_ARG 10

struct comando{
    char** argumentos;
    int numArgumentos;
};

// Inicializa um comando (assim como seus argumentos)
Comando* inicializaComando(){
    Comando* novoComando = (Comando*) malloc(sizeof(Comando));
    if(novoComando == NULL){
        perror("Erro na alocacao de um novo comando\n");
        exit(1);
    }

    novoComando->argumentos = (char**) malloc(sizeof(char*)*MAX_ARG);
    if(novoComando->argumentos == NULL){
        printf("Erro na alocação dos argumentos\n");
        exit(1);
    }

    novoComando->numArgumentos = 0;

    return novoComando;
}

// Preenche os argumentos do comando, no maximo 10
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

char* retornaNomeComando(Comando* c){
    return c->argumentos[0];
}

char** retornaArgumentos(Comando* c){
    return c->argumentos;
}

// Libera a memoria utilizada pelo comando
void liberaComando(Comando* c){
    for(int i = 0; i < c->numArgumentos; i++){
        free(c->argumentos[i]);
    }

    free(c->argumentos);
    free(c);
}