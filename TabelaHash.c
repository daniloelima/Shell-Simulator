//
// Created by bonella on 25/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include "TabelaHash.h"

struct tabelaHash{
    Lista** vet;
    int tam;
};

static int fhash(TabelaHash* hash,int num){
    return num%hash->tam;
}

TabelaHash* inicializaHash(int tamHash){
    TabelaHash* novaTabela = malloc(sizeof(TabelaHash));

    novaTabela->vet = malloc(sizeof(Lista*) * tamHash);

    for(int i = 0; i < tamHash; i++){
        novaTabela->vet[i] = inicializaLista();
    }

    novaTabela->tam = tamHash;

    return novaTabela;
}

Lista* retornaListaHash(TabelaHash* hash, int pos){
    return hash->vet[pos];
}

void percorreHashMatandoGrupos(TabelaHash* hash){
    for(int i = 0; i < hash->tam;i++){
        if(!listaVazia(hash->vet[i])){
            printf("Entrou\n");
            percorreListaMatandoGrupos(hash->vet[i]);
        }
    }
}

//funcao que insere retorna a posicao na hash em que um pgid está
void insereHash(TabelaHash* hash, int pid){
    int pos = fhash(hash, pid);

    insereLista(hash->vet[pos], pid);
}

void retiraHash(TabelaHash* hash, int pid){
    int pos = fhash(hash, pid);
    retiraLista(hash->vet[pos],pid);
}
void liberaHash(TabelaHash* hash){
    for(int i = 0; i < hash->tam; i++){
        liberaLista(hash->vet[i]);
    }

    free(hash->vet);
    free(hash);
}

void imprimeHash(TabelaHash *hash){
    int i;
    for(i=0;i<hash->tam;i++){
        printf("Posiçao vet[%d]: ",i);
        imprimeLista(hash->vet[i]);
    }
    printf("\n");
}
