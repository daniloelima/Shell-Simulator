//
// Created by bonella on 25/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include "TabelaHash.h"

// Implementacao de uma tabela hash utilizando listas

struct tabelaHash{
    Lista** vet;
    int tam;
};

// Funcao de hash tradicional para mapear um inteiro como chave
static int fhash(TabelaHash* hash,int num){
    return num%hash->tam;
}

TabelaHash* inicializaHash(int tamHash){
    TabelaHash* novaTabela = malloc(sizeof(TabelaHash));
    if(novaTabela == NULL){
        perror("Erro na alocacao de uma nova hash\n");
        exit(1);
    }

    novaTabela->vet = malloc(sizeof(Lista*) * tamHash);
    if(novaTabela->vet == NULL){
        perror("Erro na alocacao de uma nova hash\n");
        exit(1);
    }

    for(int i = 0; i < tamHash; i++){
        novaTabela->vet[i] = inicializaLista();
    }

    novaTabela->tam = tamHash;

    return novaTabela;
}

// Percorre cada celula da hash e, para cada celula, percorre a lista de colisoes com o objetivo de matar todos os grupos
void percorreHashMatandoGrupos(TabelaHash* hash){
    for(int i = 0; i < hash->tam;i++){
        if(!listaVazia(hash->vet[i])){
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
