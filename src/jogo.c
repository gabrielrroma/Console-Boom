// src/jogo.c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>      // _kbhit() e _getch()
#include <windows.h>    // Sleep(), SetConsoleCursorPosition()
#include <time.h>       // time()
#include "jogo.h"
#include "fila_bombas.h"
#include "arvore_pontuacoes.h"
#include "gpt_api.h"

#define WIDTH   20
#define HEIGHT  15

static int playerX, playerY;

// Desenha a barreira fixa (borda) e limpa o interior
static void desenharBordas(void) {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x==0 || x==WIDTH-1 || y==0 || y==HEIGHT-1)
                putchar('#');
            else
                putchar(' ');
        }
        putchar('\n');
    }
}

// Atualiza apenas o interior (player e bombas) sem tocar na borda
static void desenharInterior(void) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    for (int y = 1; y < HEIGHT-1; y++) {
        for (int x = 1; x < WIDTH-1; x++) {
            coord.X = x;
            coord.Y = y;
            SetConsoleCursorPosition(h, coord);

            // player
            if (x == playerX && y == playerY) {
                putchar('P');
                continue;
            }
            // bomba?
            int mostrou = 0;
            for (Bomba *b = getFilaHead(); b; b = b->prox) {
                if (b->x == x && b->y == y) {
                    putchar('B');
                    mostrou = 1;
                    break;
                }
            }
            if (mostrou) continue;
            // espaço vazio
            putchar(' ');
        }
    }
}

// Exibe o menu principal
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
            case 1: iniciarJogo();  break;
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

    srand((unsigned)time(NULL));
    limparFila();

    // desenha borda fixa
    desenharBordas();

    while (1) {
        // gera bombas mais frequentemente
        if (rand() % 5 == 0) {
            inserirBomba(rand() % (WIDTH-2) + 1, 1);
        }

        atualizarBombas();
        while (getFilaHead() && getFilaHead()->y >= HEIGHT-1) {
            removerBomba();
        }

        if (verificarColisao(playerX, playerY)) {
            break;
        }

        // movimento WASD
        if (_kbhit()) {
            char c = _getch();
            if (c=='w' && playerY>1)           playerY--;
            else if (c=='s' && playerY<HEIGHT-2) playerY++;
            else if (c=='a' && playerX>1)        playerX--;
            else if (c=='d' && playerX<WIDTH-2)  playerX++;
        }

        // redesenha interior e atualiza pontuação
        desenharInterior();
        // posição do texto de pontos
        COORD scorePos = {0, HEIGHT};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), scorePos);
        printf("Pontos: %d   ", pontos);

        // delay dinâmico
        int delay = 100 - (pontos / 10);
        if (delay < 30) delay = 30;
        Sleep(delay);

        pontos++;
    }

    // fim de jogo
    COORD endPos = {0, HEIGHT+1};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), endPos);
    printf("Game Over! Pontos: %d\n", pontos);

    char nome[50];
    printf("Digite seu nome: ");
    scanf("%49s", nome);
    inserirPontuacao(nome, pontos);
    salvarRankingEmArquivo("ranking.txt");

    encerrarJogo();
}

// Limpa recursos e retorna ao menu
void encerrarJogo(void) {
    limparFila();
    // (a árvore de pontuações permanece para exibir no menu)
}
