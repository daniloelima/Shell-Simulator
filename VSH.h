//
// Created by bonella on 22/04/2021.
//

#ifndef TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_VSH_H
    #define TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_VSH_H

    #include "TratadoresDeSinal.h"
    #include "Comando.h"
    #include "TabelaHash.h"

    typedef struct vsh VSH;

    VSH* initVSH(void);
    void adicionaComando(VSH* vsh , Comando* comando);
    void leComandos(VSH* vsh);
    int executaComandos(VSH* vsh);
    void liberaComandos(VSH* vsh);
    void reInitComandos(VSH* vsh);

#endif //TRABALHO_1_DE_SISTEMAS_OPERACIONAIS_VSH_H
