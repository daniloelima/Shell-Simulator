//
// Created by bonella on 25/04/2021.
//

#ifndef TRABALHOVSH_TABELAHASH_H
    #define TRABALHOVSH_TABELAHASH_H
    #include "Lista.h"
    typedef struct tabelaHash TabelaHash;

    TabelaHash* inicializaHash(int);
    Lista* retornaListaHash(TabelaHash*, int);
    void percorreHashMatandoGrupos(TabelaHash*);
    void insereHash(TabelaHash* hash, int);
    void retiraHash(TabelaHash* hash, int);
    void liberaHash(TabelaHash* hash);
    void imprimeHash(TabelaHash *hash);

#endif //TRABALHOVSH_TABELAHASH_H
