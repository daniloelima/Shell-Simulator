//
// Created by bonella on 25/04/2021.
//

#ifndef TRABALHOVSH_TRATADORESDESINAL_H
    #define TRABALHOVSH_TRATADORESDESINAL_H

    typedef struct sigaction Tratador;

    void IgnoraSinaldoUsuarioCoronavac(void);
    void IgnoraSinaldoUsuarioPfizer(void);
    void TECLADO_FOREGROUND();
    void LIBERA_TECLADO();

#endif //TRABALHOVSH_TRATADORESDESINAL_H
