//
// Created by vitor,enzo,danilo on 02/04/2021.
//

#include <stdio.h>
#include "TratadoresDeSinal.h"
#include "VSH.h"

int main(){
    IgnoraSinaldoUsuarioCoronavac(); //Cria o tratador que lida com os sinais SIGUSR1 e SIGUSR2
    VSH* vsh = initVSH(); //Inicializa a estrutura que contem os principais dados durante a execucao do programa

    //Executa o programa até a chamada do comando "armageddon"
    while (1) {
        printf("\033[0;32mvsh>\033[0m"); // Imprime o prompt "vsh>"
        leComandos(vsh); // Le os comandos inseridos pelo usuario
        executaComandos(vsh); // Executa os comandos lidos pelo usuario
        liberaComandos(vsh); // Libera os comandos depois de terem sido executados
        reInitComandos(vsh); // Reinicializa a estrutura para uma nova leitura de comandos
    }
}
