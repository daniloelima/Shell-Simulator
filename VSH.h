//
// Created by bonella on 22/04/2021.
//

#ifndef TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_VSH_H
#define TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_VSH_H
#include "Comando.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct vsh VSH;

VSH* initVSH(void);
void adicionaComando(VSH* vsh , Comando* comando);
void leComandos(VSH* vsh);
void executaComandos(VSH* vsh);
void imprimeComandos(VSH* vsh);
void liberaComandos(VSH* vsh);

#endif //TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_VSH_H
