//
// Created by vitor on 02/04/2021.
//

#ifndef TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_COMANDO_H
#define TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_COMANDO_H

typedef struct comando Comando;

Comando* inicializaComando();
void preencheArgumentos();
void imprimeComando();
void liberaComando();

#endif //TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_COMANDO_H
