//
// Created by bonella on 25/04/2021.
//

#ifndef TRABALHOVSH_LISTA_H
#define TRABALHOVSH_LISTA_H

typedef struct celula Celula;
typedef struct lista Lista;

Lista* inicializaLista();
int listaVazia(Lista*);
void insereLista(Lista*, int);
void retiraLista(Lista*, int);
void percorreListaMatandoGrupos(Lista*);
void imprimeLista(Lista*);
void liberaLista(Lista*);

#endif //TRABALHOVSH_LISTA_H
