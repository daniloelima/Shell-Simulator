#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "TratadoresDeSinal.h"

void imprimeJacare(int signo){
    printf("              _  _\n");
    printf("    _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--.\n");
    printf("   (.(.)----'`        ^^'                /^   ^^-._\n");
    printf("   (    `                 \\             |    _    ^^-._\n");
    printf("    VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:\\  (_..,______^^-.\n");
    printf("     `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  )\n");
    printf("              (((`   (((`          (((`  (((`        `'--'^\n");
    printf("    I feel weird...\n");
}

// Tratador de sinal que imprime o jacare (torna o processo imune a SIGUSR1 e SIGUSR2
void IgnoraSinaldoUsuarioCoronavac(void){
    sigset_t SinaisIgnorados;

    sigemptyset(&SinaisIgnorados);
    sigaddset(&SinaisIgnorados,SIGINT);
    sigaddset(&SinaisIgnorados,SIGQUIT);
    sigaddset(&SinaisIgnorados,SIGTSTP);

    Tratador coronavac;
    coronavac.sa_handler = imprimeJacare;
    coronavac.sa_mask = SinaisIgnorados;
    coronavac.sa_flags = 0;

    sigaction(SIGUSR1,&coronavac,NULL);
    sigaction(SIGUSR2,&coronavac,NULL);
}

// Tratador de sinal que faz com que os processos em foreground sejam imunes a SIGUSR1 e SIGUSR2
void IgnoraSinaldoUsuarioPfizer(void){
    sigset_t SinaisIgnorados;

    sigemptyset(&SinaisIgnorados);

    Tratador coronavac;
    coronavac.sa_handler = SIG_IGN; // Ignora os sinais (nao ficam bloqueados)
    coronavac.sa_mask = SinaisIgnorados;
    coronavac.sa_flags = 0;

    sigaction(SIGUSR1,&coronavac,NULL);
    sigaction(SIGUSR2,&coronavac,NULL);
}

// Faz com que, quando um processo foreground estiver em execucao, o processo da vsh seja imune aos comandos teclado
void TECLADO_FOREGROUND(){
    sigset_t SinaisIgnorados;

    sigemptyset(&SinaisIgnorados);

    Tratador teclado;
    teclado.sa_handler = SIG_IGN; // Ignora os sinais (nao ficam bloqueados)
    teclado.sa_mask = SinaisIgnorados;
    teclado.sa_flags = 0;

    sigaction(SIGINT,&teclado,NULL);
    sigaction(SIGQUIT,&teclado,NULL);
    sigaction(SIGTSTP,&teclado,NULL);
}

// Reseta os comandos de teclado para o processo da vsh, tornando-o novamente vulneravel a eles
void LIBERA_TECLADO(){
    sigset_t SinaisIgnorados;

    sigemptyset(&SinaisIgnorados);

    Tratador teclado;
    teclado.sa_handler = SIG_DFL; // Faz com que os sinais tenham comportamento padrao
    teclado.sa_mask = SinaisIgnorados;
    teclado.sa_flags = 0;

    sigaction(SIGINT,&teclado,NULL);
    sigaction(SIGQUIT,&teclado,NULL);
    sigaction(SIGTSTP,&teclado,NULL);
    
    IgnoraSinaldoUsuarioCoronavac();
}