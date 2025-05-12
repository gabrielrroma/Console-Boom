// src/jogo.c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>      // _kbhit() e _getch() no Windows
#include <windows.h>    // Sleep()
#include "jogo.h"
#include "fila_bombas.h"
#include "arvore_pontuacoes.h"
#include "gpt_api.h"

#define WIDTH  20
#define HEIGHT 15

static int playerX, playerY;

// Desenha bordas, jogador e todas as bombas
void desenharTela(void) {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x==0 || x==WIDTH-1 || y==0 || y==HEIGHT-1) {
                putchar('#');
            } else if (x==playerX && y==playerY) {
                putchar('P');
            } else {
                int desenhou = 0;
                for (Bomba *b = getFilaHead(); b; b = b->prox) {
                    if (b->x==x && b->y==y) {
                        putchar('B');
                        desenhou = 1;
                        break;
                    }
                }
                if (!desenhou) putchar(' ');
            }
        }
        putchar('\n');
    }
}

// Menu principal
void menuPrincipal(void) {
    int opcao;
    do {
        printf("=== Console Boom ===\n");
        printf("1. Iniciar Jogo\n");
        printf("2. Ver Ranking\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: iniciarJogo(); break;
            case 2: exibirRanking(); break;
            case 3: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 3);
}

// Loop principal do jogo
void iniciarJogo(void) {
    playerX = WIDTH/2;
    playerY = HEIGHT-2;
    int pontos = 0;
    limparFila();

    while (1) {
        // Gera bomba aleatória
        if (rand()%5 == 0)
            inserirBomba(rand()%(WIDTH-2)+1, 1);
        // Move bombas
        atualizarBombas();
        // Remove as que saíram do campo
        while (getFilaHead() && getFilaHead()->y >= HEIGHT-1)
            removerBomba();
        // Colisão?
        if (verificarColisao(playerX, playerY))
            break;
        // Leitura não-bloqueante
        if (_kbhit()) {
            char c = _getch();
            if (c=='w' && playerY>1) playerY--;
            if (c=='s' && playerY<HEIGHT-2) playerY++;
            if (c=='a' && playerX>1) playerX--;
            if (c=='d' && playerX<WIDTH-2) playerX++;
        }
        // Desenha e mostra pontos
        desenharTela();
        printf("Pontos: %d\n", pontos);
        Sleep(100);
        pontos++;
    }

    // Fim de jogo
    printf("Game Over! Pontos: %d\n", pontos);
    char nome[50];
    printf("Digite seu nome: ");
    scanf("%49s", nome);
    inserirPontuacao(nome, pontos);
    salvarRankingEmArquivo("ranking.txt");
    encerrarJogo();
}

// Limpa recursos e volta ao menu
void encerrarJogo(void) {
    limparFila();
    // Se precisar liberar a árvore de pontuações, faça aqui
    // Depois disso, retornamos ao menu (quando iniciarJogo retorna)
}
