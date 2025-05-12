#ifndef FILA_BOMBAS_H
#define FILA_BOMBAS_H

typedef struct Bomba {
    int x, y;
    struct Bomba *prox;
} Bomba;

void inserirBomba(int x, int y);
void atualizarBombas(void);
void removerBomba(void);
int verificarColisao(int playerX, int playerY);
void limparFila(void);

#endif // FILA_BOMBAS_H

// Retorna ponteiro para o primeiro nó da lista de bombas
Bomba* getFilaHead(void);
