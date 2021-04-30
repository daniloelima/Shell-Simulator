//
// Created by bonella on 22/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "VSH.h"

#define MAX_COM 7
#define READ 0
#define WRITE 1

struct vsh{
    Comando** comandos;
    int numComandos;
    TabelaHash* descendentes;
    int numZombies;
};

//inicializa as estruturas necessarias para VSH
VSH* initVSH(void){
    VSH* novoVSH = (VSH*) malloc(sizeof(VSH));

    novoVSH->numComandos = 0;
    novoVSH->comandos = (Comando**) malloc(sizeof(Comando*)*MAX_COM);
    if(novoVSH->comandos == NULL){
        perror("Erro na alocação dos comandos\n");
        exit(1);
    }
    novoVSH->descendentes = inicializaHash(47);
    novoVSH->numZombies = 0;


    return novoVSH;
}

//Reinicia os comandos depois de executar um comando
void reInitComandos(VSH* vsh){
    vsh->numComandos = 0;
    vsh->comandos = (Comando**) malloc(sizeof(Comando*)*MAX_COM);
    if(vsh->comandos == NULL){
        perror("Erro na alocação dos comandos\n");
        exit(1);
    }
}

//le a string digitada no buffer do vsh
static char* novosComandos(){
    char strAux[100];
    char* novosComandos;

    fgets(strAux, 100, stdin);

    novosComandos = strdup(strAux);
    novosComandos[strlen(novosComandos) - 1] = '\0';

    return novosComandos;
}

//Adiciona novo comando a VSH
void adicionaComando(VSH* vsh , Comando* comando){
    vsh->comandos[vsh->numComandos] = comando;
    vsh->numComandos++;
}

//le a linha da vsh e separa ela por comando(s)
void leComandos(VSH* listacomandos){
    
    char* comandos = novosComandos();
    char* str = strtok(comandos, "|");

    while(str != NULL){
        Comando* comando = inicializaComando();

        preencheArgumentos(comando, str);

        adicionaComando(listacomandos,comando);

        str = strtok(NULL, "|");
    }

    free(comandos);
}

//Faz com que o VSH receba o retorno dos filhos zombies
static void liberaMoita(VSH* vsh){
    for(int i = 0;i < vsh->numZombies;i++)
        waitpid(-1,NULL,WNOHANG);

    vsh->numZombies = 0;
}
//Acaba com todos processo relacionados ao vsh
static void armageddon(VSH* vsh){
    percorreHashMatandoGrupos(vsh->descendentes); //Mata todos grupos mapeados ativos de descendentes
    liberaComandos(vsh);
    liberaHash(vsh->descendentes);
    free(vsh);
    exit(0);
}

//Roda os comandos lançados no foreground lançando um filho ou fazendo operação interna
static void FOREGROUND(VSH* vsh){
    char* nomeComando = retornaNomeComando(vsh->comandos[0]);
    /*==============PROCESSO PAI==============*/


    if(strcmp("liberamoita", nomeComando) == 0){ //caso seja o comando da VSH liberamoita
        liberaMoita(vsh);
    }else if(strcmp("armageddon", nomeComando) == 0){ //caso seja o comando da VSH armageddon
        armageddon(vsh);
    }else{ //caso seja um comando a ser executado
        int pid;
        if((pid = fork()) < 0){
            perror("erro ao criar filho foreground");
            exit(1);
        }
        if(pid == 0) {
            /*==============PROCESSO FOREGROUND==============*/
            IgnoraSinaldoUsuarioPfizer();
            char **args = retornaArgumentos(vsh->comandos[0]);
            if(execvp(args[0], args) == -1){
                perror("Comando não encontrado");
                exit(1);
            }
            /*==============FIM PROCESSO FOREGROUND==============*/
        }
        TECLADO_FOREGROUND();
        int status;
        waitpid(pid,&status,WUNTRACED); //espera pelo filho terminar ou ser suspenso
        if(WIFSTOPPED(status)){ //caso tenha sido suspenso da um aviso ao usuario
            printf("PROCESSO[%d] SUSPENSO\n",pid);
            vsh->numZombies++;
        }
        
    }
    LIBERA_TECLADO();
    /*==============FIM PROCESSO PAI==============*/
}

//roda os comandos em background em uma nova sessao e grupo
static void BACKGROUND(VSH* vsh){

    int pipeGRUPO[2]; //pipe para passar o pgid para o processo da vsh
    if(pipe(pipeGRUPO) != 0){
        perror("ERRO AO ABRIR PIPE que passa o grupo\n");
        exit(1);
    }
    int liderSessao;
    if((liderSessao = fork()) < 0){
        perror("Erro ao abrir processo mestre\n");
        exit(1);
    } // FILHO DA VSH

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

        int sid = setsid(); //cria uma nova sessao consquentemente um novo grupo

        close(pipeGRUPO[READ]);
        if(write(pipeGRUPO[WRITE],&sid,sizeof(int)) < 0){ //escreve para o pai no novo pgid
            perror("Erro escrever no pipe que envia o pgid\n");
            exit(1);
        }
        //para cada comando executado com operador | lança uma exec
        for (int i = 0; i < vsh->numComandos; i++) {
            char **args = retornaArgumentos(vsh->comandos[i]); //retorna o comando em ordens

            int pid;
            if((pid = fork()) < 0){
                perror("erro ao criar neto , processo em bg\n");
                exit(1);
            } // NETOS DA VSH

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

                if(execvp(args[0], args) == -1){
                    perror("Comando não encontrado\n");
                    exit(1);
                }
                /*==============FIM i PROCESSO BACKGROUND==============*/
            }
        }
        for (int j = 0; j < tampipe; j++) {
            close(fd[j][READ]);
            close(fd[j][WRITE]);
        }
        //o lider da sessao(NAO A VSH) espera cada um dos seus filhos
        for(int i = 0; i < vsh->numComandos; i++){
            int status = 0;
            wait(&status);
            //caso algum filho termine por um sinal
            if(WIFSIGNALED(status)){
                //caso um dos filhos termine por SIGUSR 1 ou 2 mata todos irmaos ; (
                if(WTERMSIG(status) == SIGUSR1 || WTERMSIG(status) == SIGUSR2){
                    killpg(getpgid(0),SIGKILL);
                }
            }
        }

        retiraHash(vsh->descendentes,getpgid(0)); //retira ele do mapeamento de programas pois finalizou

        exit(1);
        /*==============FIM PROCESSO MESTRE==============*/
    }

    /*==============PROCESSO PAI==============*/
    int sid;
    close(pipeGRUPO[WRITE]);
    if(read(pipeGRUPO[READ],&sid,sizeof(int)) < 0){ //recebe o numero do novo pgid
        perror("Erro ao ler no pipe que recebe o grupo\n");
        exit(1);
    }
    insereHash(vsh->descendentes,sid); //adiciona um pgid ao mapeamento dos decendentes do vsh
    vsh->numZombies++;
    /*==============FIM PROCESSO PAI==============*/
}

//executa os comandos FOREGROUND ou em BACKGROUND
void executaComandos(VSH* vsh){

    if(vsh->numComandos == 1){ // FOREGROUND
        FOREGROUND(vsh);
    }else if(vsh->numComandos > 1) { // BACKGROUND
        BACKGROUND(vsh);

    }

}
//libera cada comando da VSH
void liberaComandos(VSH* vsh){
    for(int i = 0; i < vsh->numComandos; i++) {
        liberaComando(vsh->comandos[i]);
    }
    free(vsh->comandos);
}
