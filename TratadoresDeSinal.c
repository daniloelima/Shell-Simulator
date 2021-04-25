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

void mataGrupo(int signalgroup){
    printf("grupo morto: %d", getpgid(0));
    killpg(getpgid(0),SIGKILL);
}

void Aglomeracao(void){
    struct sigaction corona;
    corona.sa_handler = mataGrupo;
    corona.sa_flags = 0;
    sigemptyset(&corona.sa_mask);
    sigaction(SIGUSR1,&corona,NULL);
    sigaction(SIGUSR2,&corona,NULL);

}

void IgnoraSinaldoUsuarioCoronavac(void){
    sigset_t SinaisIgnorados;

    sigemptyset(&SinaisIgnorados);
    sigaddset(&SinaisIgnorados,SIGINT);
    sigaddset(&SinaisIgnorados,SIGQUIT);
    sigaddset(&SinaisIgnorados,SIGTSTP);

    sigprocmask(SIG_SETMASK, &SinaisIgnorados, NULL);

    Tratador coronavac;
    coronavac.sa_handler = imprimeJacare;
    coronavac.sa_mask = SinaisIgnorados;
    coronavac.sa_flags = 0;

    sigaction(SIGUSR1,&coronavac,NULL);
    sigaction(SIGUSR2,&coronavac,NULL);
}

void IgnoraSinaldoUsuarioPfizer(void){
    sigset_t SinaisIgnorados;

    sigemptyset(&SinaisIgnorados);
    sigprocmask(SIG_SETMASK, &SinaisIgnorados, NULL);

    Tratador coronavac;
    coronavac.sa_handler = SIG_IGN;
    coronavac.sa_mask = SinaisIgnorados;
    coronavac.sa_flags = 0;

    sigaction(SIGUSR1,&coronavac,NULL);
    sigaction(SIGUSR2,&coronavac,NULL);
}