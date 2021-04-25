//
// Created by vitor,enzo,danilo on 02/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Comando.h"
#include "TratadoresDeSinal.h"
#include "VSH.h"
#include "TabelaHash.h"

int main(){
    printf("My PID is %d\n",getpid());
    VSH* vsh = initVSH();
    IgnoraSinaldoUsuarioCoronavac();
    while(1){
        printf("\033[0;32mvsh>\033[0m");
        leComandos(vsh);
        executaComandos(vsh);
        liberaComandos(vsh);
        reInitComandos(vsh);
    }

    return 0;
}
