//
// Created by bonella on 22/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "VSH.h"

#define READ 0
#define WRITE 1

struct vsh{
    Comando** comandos;
    int numComandos;
    TabelaHash* descendentes;
    int numZombies;
};

VSH* initVSH(void){
    VSH* novoVSH = (VSH*) malloc(sizeof(VSH));

    novoVSH->numComandos = 0;
    novoVSH->comandos = (Comando**) malloc(sizeof(Comando*)*5);
    novoVSH->descendentes = inicializaHash(47);
    novoVSH->numZombies = 0;


    return novoVSH;
}

void reInitComandos(VSH* vsh){
    vsh->numComandos = 0;
    vsh->comandos = (Comando**) malloc(sizeof(Comando*)*5);
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

static void liberaMoita(VSH* vsh){
    for(int i = 0;i < vsh->numZombies;i++)
        waitpid(-1,NULL,WNOHANG);

    vsh->numZombies = 0;
}

static void armageddon(VSH* vsh){
    percorreHashMatandoGrupos(vsh->descendentes);
    liberaComandos(vsh);
    liberaHash(vsh->descendentes);
    free(vsh);
    killpg(getpgid(0),SIGKILL);
}

static void FOREGROUND(VSH* vsh){
    char* nomeComando = retornaNomeComando(vsh->comandos[0]);
    /*==============PROCESSO PAI==============*/
    if(strcmp("liberamoita", nomeComando) == 0){
        liberaMoita(vsh);
    }else if(strcmp("armageddon", nomeComando) == 0){
        armageddon(vsh);
    }else{
        int pid = fork();
        if(pid == 0) {
            /*==============PROCESSO FOREGROUND==============*/
            IgnoraSinaldoUsuarioPfizer();
            char **args = retornaArgumentos(vsh->comandos[0]);
            execvp(args[0], args);
            /*==============FIM PROCESSO FOREGROUND==============*/
        }
        waitpid(pid,NULL,0);

    }
    /*==============FIM PROCESSO PAI==============*/
}

static void BACKGROUND(VSH* vsh){
    int pipeGRUPO[2];
    pipe(pipeGRUPO);

    int liderSessao = fork(); // FILHO DA VSH

    if (liderSessao == 0) {
        /*==============PROCESSO MESTRE==============*/

        int tampipe = vsh->numComandos - 1;
        int fd[tampipe][2];

        for (int i = 0; i < tampipe; i++) {
            if (pipe(fd[i]) != 0) {
                perror("Erro ao abrir pipe!\n");
                return;
            }
        }

        int sid = setsid();

        close(pipeGRUPO[READ]);
        write(pipeGRUPO[WRITE],&sid,sizeof(int));

        for (int i = 0; i < vsh->numComandos; i++) {
            char **args = retornaArgumentos(vsh->comandos[i]);

            int pid = fork(); // NETOS DA VSH

            if (pid == 0) {
                /*==============i PROCESSO BACKGROUND==============*/
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
                /*==============FIM i PROCESSO BACKGROUND==============*/
            }
        }
        for (int j = 0; j < tampipe; j++) {
            close(fd[j][READ]);
            close(fd[j][WRITE]);
        }

        for(int i = 0; i < vsh->numComandos; i++){
            int status = 0;
            wait(&status);
            if(WIFSIGNALED(status)){
                if(WTERMSIG(status) == SIGUSR1 || WTERMSIG(status) == SIGUSR2){
                    killpg(getpgid(0),SIGKILL);
                }
            }
        }


        retiraHash(vsh->descendentes,getpgid(0));

        exit(1);
        /*==============FIM PROCESSO MESTRE==============*/
    }
    /*==============PROCESSO PAI==============*/
    int sid;
    close(pipeGRUPO[WRITE]);
    read(pipeGRUPO[READ],&sid,sizeof(int));
    insereHash(vsh->descendentes,sid);
    vsh->numZombies++;
    /*==============FIM PROCESSO PAI==============*/
}

void executaComandos(VSH* vsh){

    if(vsh->numComandos == 1){ // FOREGROUND
        FOREGROUND(vsh);
    }else if(vsh->numComandos > 1) { // BACKGROUND
        BACKGROUND(vsh);

    }

}

void liberaComandos(VSH* vsh){
    for(int i = 0; i < vsh->numComandos; i++) {
        liberaComando(vsh->comandos[i]);
    }
    free(vsh->comandos);
}
