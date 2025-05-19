// src/jogo.c

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include "jogo.h"
#include "fila_bombas.h"
#include "arvore_pontuacoes.h"
#include "gpt_api.h"

#define WIDTH       20
#define HEIGHT      15
#define THRESHOLD   500
#define INIT_DELAY 100
#define MIN_DELAY   30

static int playerX, playerY;
static int prevLevel, speedCounter, gamePoints;
static bool closedLines[HEIGHT];

static void desenharBordas(void) {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++)
            putchar((x == 0 || x == WIDTH-1 || y == 0 || y == HEIGHT-1) ? '#' : ' ');
        putchar('\n');
    }
}

static void desenharInterior(void) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    for (int y = 1; y < HEIGHT-1; y++) {
        for (int x = 1; x < WIDTH-1; x++) {
            coord.X = x; coord.Y = y;
            SetConsoleCursorPosition(h, coord);
            if (closedLines[y]) {
                putchar('#');
                continue;
            }
            if (x == playerX && y == playerY) {
                putchar('P');
                continue;
            }
            bool bombHere = false;
            for (Bomba *b = getFilaHead(); b; b = b->prox) {
                if (b->x == x && b->y == y) {
                    putchar('B');
                    bombHere = true;
                    break;
                }
            }
            if (!bombHere) putchar(' ');
        }
    }
}

void menuPrincipal(void) {
    int opcao = 0;
    while (opcao != 3) {
        printf("=== Console Boom ===\n"
               "1. Iniciar Jogo\n"
               "2. Ver Ranking\n"
               "3. Sair\n"
               "Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            opcao = 3;
            fflush(stdin);
        }
        if (opcao == 1) iniciarJogo();
        else if (opcao == 2) exibirRanking();
        else if (opcao == 3) printf("Saindo...\n");
        else printf("Opcao invalida!\n");
    }
}

void iniciarJogo(void) {
    playerX = WIDTH/2;
    playerY = HEIGHT-2;
    prevLevel = speedCounter = gamePoints = 0;
    srand((unsigned)time(NULL));
    limparFila();
    for (int i = 0; i < HEIGHT; i++)
        closedLines[i] = false;

    desenharBordas();

    int maxShift = (HEIGHT-2)/2;
    while (1) {
        // 1) subir de nível
        if (speedCounter >= THRESHOLD && prevLevel < maxShift) {
            closedLines[1 + prevLevel] = true;
            prevLevel++;
            speedCounter = 0;

            chamarIA("Está ficando apertado aí, hein?");
            // exibe IA sem limpar o mapa
            {
                HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
                COORD iaPos = { 0, HEIGHT };
                SetConsoleCursorPosition(h, iaPos);
                exibirMensagemIA();
            }
            Sleep(2000);
            desenharBordas();
        }

        // 2) inserir e mover bombas
        int shift = prevLevel;
        if (rand() % 5 == 0)
            inserirBomba(rand() % (WIDTH - 2) + 1, 1 + shift);
        atualizarBombas();
        while (getFilaHead() && getFilaHead()->y >= HEIGHT - 1)
            removerBomba();

        // 3) marco de 300 pontos
        if (gamePoints > 0 && gamePoints % 300 == 0) {
            char prompt[100];
            snprintf(prompt, sizeof(prompt),
                     "Parabéns! Você atingiu %d pontos!", gamePoints);
            chamarIA(prompt);
            {
                HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
                COORD iaPos = { 0, HEIGHT };
                SetConsoleCursorPosition(h, iaPos);
                exibirMensagemIA();
            }
            Sleep(2000);
            desenharBordas();
        }

        // 4) colisão?
        if (verificarColisao(playerX, playerY))
            break;

        // 5) controles
        if (_kbhit()) {
            char c = _getch();
            if (c == 'a' && playerX > 1) playerX--;
            else if (c == 'd' && playerX < WIDTH - 2) playerX++;
        }

        // 6) desenha interior + placar
        desenharInterior();
        {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD scorePos = { 0, HEIGHT };
            SetConsoleCursorPosition(h, scorePos);
            printf("Pontos: %d   ", gamePoints);
        }

        // 7) incrementos e delay
        gamePoints++;
        speedCounter++;
        int delay = INIT_DELAY - (speedCounter * (INIT_DELAY - MIN_DELAY) / THRESHOLD);
        if (delay < MIN_DELAY) delay = MIN_DELAY;
        Sleep(delay);
    }

    // Game Over
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD endPos = { 0, HEIGHT+1 };
        SetConsoleCursorPosition(h, endPos);
        printf("Game Over! Pontos: %d\n", gamePoints);

        chamarIA("Você jogou bem! Tente bater os 500 na próxima.");
        COORD iaPos2 = { 0, HEIGHT+2 };
        SetConsoleCursorPosition(h, iaPos2);
        exibirMensagemIA();
        Sleep(2000);
    }

    char nome[50];
    printf("Digite seu nome: ");
    scanf("%49s", nome);
    inserirPontuacao(nome, gamePoints);
    salvarRankingEmArquivo("ranking.txt");
    encerrarJogo();
}

void encerrarJogo(void) {
    limparFila();
}
