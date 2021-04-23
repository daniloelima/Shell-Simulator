//
// Created by vitor,enzo,danilo on 02/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Comando.h"
#include "VSH.h"

extern void imprimeJacare();

int main(){
    while(1){
        printf("\033[0;32mvsh>\033[0m");
        VSH* vsh = initVSH();
        leComandos(vsh);
        executaComandos(vsh);
    }

    return 0;
}