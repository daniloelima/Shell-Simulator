//
// Created by vitor,enzo,danilo on 02/04/2021.
//

#ifndef TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_COMANDO_H
#define TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_COMANDO_H

typedef struct comando Comando;

Comando* inicializaComando();
void preencheArgumentos(Comando*, char*);
char* retornaNomeComando(Comando*);
char** retornaArgumentos(Comando*);
void liberaComando(Comando*);

#endif //TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_COMANDO_H
