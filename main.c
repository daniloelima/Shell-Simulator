//
// Created by vitor,enzo,danilo on 02/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "TratadoresDeSinal.h"
#include "VSH.h"


int main(){
    /*==============PROCESSO DA VSH==============*/
    IgnoraSinaldoUsuarioCoronavac();
    VSH* vsh = initVSH();
    //RODA A VSH
    while (1) {
        printf("\033[0;32mvsh>\033[0m");
        leComandos(vsh);
        executaComandos(vsh);
        liberaComandos(vsh);
        reInitComandos(vsh);
    }

}
