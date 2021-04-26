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

    //cria um filho para ser a VSH
    signal(SIGINT,SIG_DFL);
    while(1){
        printf("MAIN PID is %d , GID %d ,SID %d \n",getpid(), getpgid(0), getsid(0));
        int vshProcess = fork();
        if(vshProcess == 0){
            /*==============PROCESSO DA VSH==============*/
            printf("VSH PID is %d , GID %d ,SID %d \n",getpid(), getpgid(0), getsid(0));
            VSH* vsh = initVSH();
            IgnoraSinaldoUsuarioCoronavac();
            //RODA A VSH
            while (1) {
                int flag = 0;
                printf("\033[0;32mvsh>\033[0m");
                leComandos(vsh);
                flag = executaComandos(vsh);
                liberaComandos(vsh);
                reInitComandos(vsh);
                if(flag == 1) return 0;

            }
            /*==============FIM PROCESSO DA VSH==============*/
            //em caso de liberamoita o VSH se mata para criar um novo

        }
        
        /*==============PROCESSO PAI==============*/

        waitpid(vshProcess,NULL,0);

        /*==============FIM PROCESSO PAI==============*/
    }
    return 0;
}
