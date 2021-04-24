//
// Created by bonella on 22/04/2021.
//

#include "VSH.h"
#include <stdio.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

struct vsh{
    Comando** comandos;
    int numComandos;
};

VSH* initVSH(void){
    VSH* novoVSH = (VSH*) malloc(sizeof(VSH));

    novoVSH->numComandos = 0;
    novoVSH->comandos = (Comando**) malloc(sizeof(Comando*)*5);

    return novoVSH;
}

static char* novosComandos(){
    char strAux[100];
    char* novosComandos;

    fgets(strAux, 100, stdin);

    novosComandos = strdup(strAux);
    novosComandos[strlen(novosComandos) - 1] = '\0';

    return novosComandos;
}

void adicionaComando(VSH* vsh , Comando* comando){
    vsh->comandos[vsh->numComandos] = comando;
    vsh->numComandos++;
}

void leComandos(VSH* listacomandos){
    char* comandos = novosComandos();
    char* str = strtok(comandos, "|");

    while(str != NULL){
        Comando* comando = inicializaComando();

        preencheArgumentos(comando, str);

        adicionaComando(listacomandos,comando);

        str = strtok(NULL, "|");
    }
}

void executaComandos(VSH* vsh){
    if(vsh->numComandos == 1){ // FOREGROUND
        int pid = fork();
        if(pid == 0) {
            char **args = retornaArgumentos(vsh->comandos[0]);
            execvp(args[0], args);
        }
        waitpid(pid,NULL,0);
    }else { // BACKGROUND
        int liderSessao = fork(); // FILHO DA VSH
        if (liderSessao == 0) {
            int tampipe = vsh->numComandos - 1;
            int fd[tampipe][2];

            for (int i = 0; i < tampipe; i++) {
                if (pipe(fd[i]) != 0) {
                    perror("Erro ao abrir pipe!\n");
                    return;
                }
            }

            setsid();

            for (int i = 0; i < vsh->numComandos; i++) {
                char **args = retornaArgumentos(vsh->comandos[i]);

                int pid = fork(); // NETOS DA VSH
                if (pid == 0) {
                    //fecha os pipes que nao vao ser usados
                    for (int j = 0; j < tampipe; j++) {
                        if (j != i && j != i - 1) {
                            close(fd[j][READ]);
                            close(fd[j][WRITE]);
                        }
                    }

                    //redirecionando o STDIN para o pipe , menos o ultimo
                    if (i > 0) {
                        close(fd[i - 1][WRITE]);
                        dup2(fd[i - 1][READ], STDIN_FILENO);
                    }

                    //redirecionando STOUD para o pipe , menos o ultimo processo
                    if (i < tampipe) {
                        close(fd[i][READ]);
                        dup2(fd[i][WRITE], STDOUT_FILENO);
                    }

                    execvp(args[0], args);
                }
            }
            for (int j = 0; j < tampipe; j++) {
                close(fd[j][READ]);
                close(fd[j][WRITE]);
            }

            for(int i = 0; i < vsh->numComandos; i++) wait(NULL);
            exit(1);
        }
    }
}

void imprimeComandos(VSH* vsh){
    for(int i = 0; i < vsh->numComandos; i++){
        imprimeComando(vsh->comandos[i]);
    }
}

void liberaComandos(VSH* vsh){
    for(int i = 0; i < vsh->numComandos; i++) {
        free(vsh->comandos[i]);
    }
    free(vsh->comandos);
    free(vsh);
}
