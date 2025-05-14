#include <stdio.h>
#include <stdlib.h>
#include <conio.h>      // _kbhit(), _getch()
#include <windows.h>
#include <stdbool.h> // Sleep(), SetConsoleCursorPosition(), GetStdHandle()
#include <time.h>
#include "jogo.h"
#include "fila_bombas.h"
#include "arvore_pontuacoes.h"
#include "gpt_api.h"

#define WIDTH  20
#define HEIGHT 15

static int playerX;
static int playerY;
static int prevLevel;
static bool closedLines[HEIGHT];

static void desenharBordas(void) {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1)
                putchar('#');
            else
                putchar(' ');
        }
        putchar('\n');
    }
}

static void desenharInterior(void) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            coord.X = x;
            coord.Y = y;
            SetConsoleCursorPosition(h, coord);
            if (closedLines[y]) {
                putchar('#');
                continue;
            }
            if (x == playerX && y == playerY) {
                putchar('P');
                continue;
            }
            int found = 0;
            for (Bomba *b = getFilaHead(); b; b = b->prox) {
                if (b->x == x && b->y == y) {
                    putchar('B');
                    found = 1;
                    break;
                }
            }
            if (!found)
                putchar(' ');
        }
    }
}

void menuPrincipal(void) {
    int opcao = 0;
    while (opcao != 3) {
        printf("=== Console Boom ===\n");
        printf("1. Iniciar Jogo\n");
        printf("2. Ver Ranking\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            opcao = 3;
            fflush(stdin);
        }
        if (opcao == 1)
            iniciarJogo();
        else if (opcao == 2)
            exibirRanking();
        else if (opcao == 3)
            printf("Saindo...\n");
        else
            printf("Opcao invalida!\n");
    }
}

void iniciarJogo(void) {
    playerX = WIDTH / 2;
    playerY = HEIGHT - 2;
    int pontos = 0;
    srand((unsigned)time(NULL));
    limparFila();
    prevLevel = 0;
    for (int i = 0; i < HEIGHT; i++) closedLines[i] = false;
    desenharBordas();
    int maxShift = (HEIGHT - 2) / 2;

    while (1) {
        int level = pontos / 300;
        if (level > prevLevel && prevLevel < maxShift) {
            int closeLine = 1 + prevLevel;
            closedLines[closeLine] = true;
            prevLevel++;
        }
        int shift = prevLevel;
        if (rand() % 5 == 0)
            inserirBomba(rand() % (WIDTH - 2) + 1, 1 + shift);

        atualizarBombas();
        while (getFilaHead() && getFilaHead()->y >= HEIGHT - 1)
            removerBomba();

        if (verificarColisao(playerX, playerY))
            break;

        if (_kbhit()) {
            char c = _getch();
            if (c == 'a' && playerX > 1)
                playerX--;
            else if (c == 'd' && playerX < WIDTH - 2)
                playerX++;
        }

        desenharInterior();
        COORD scorePos = {0, HEIGHT};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), scorePos);
        printf("Pontos: %d   ", pontos);

        int delay = 100 - (pontos * 70 / 300);
        if (delay < 30)
            delay = 30;
        Sleep(delay);
        pontos++;
    }

    COORD endPos = {0, HEIGHT + 1};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), endPos);
    printf("Game Over! Pontos: %d\n", pontos);
    char nome[50];
    printf("Digite seu nome: ");
    scanf("%49s", nome);
    inserirPontuacao(nome, pontos);
    salvarRankingEmArquivo("ranking.txt");
    encerrarJogo();
}

void encerrarJogo(void) {
    limparFila();
}
