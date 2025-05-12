#include <stdio.h>
#include "jogo.h"

int main() {
    menuPrincipal();
    return 0;
}


/* src/jogo.h */
#ifndef JOGO_H
#define JOGO_H

// Exibe o menu principal e trata a escolha do jogador
void menuPrincipal(void);

// Inicia e executa o loop principal do jogo
void iniciarJogo(void);

// Fecha o jogo, salva pontuação e limpa recursos
void encerrarJogo(void);

#endif // JOGO_H