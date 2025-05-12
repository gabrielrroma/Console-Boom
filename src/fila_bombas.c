#include <stdlib.h>
#include "fila_bombas.h"

static Bomba *filaHead = NULL;
static Bomba *filaTail = NULL;

void inserirBomba(int x, int y) {
    Bomba *nova = (Bomba*)malloc(sizeof(Bomba));
    if (!nova) return;
    nova->x = x;
    nova->y = y;
    nova->prox = NULL;
    if (filaTail) filaTail->prox = nova;
    else filaHead = nova;
    filaTail = nova;
}

void atualizarBombas(void) {
    for (Bomba *ptr = filaHead; ptr; ptr = ptr->prox) ptr->y++;
}

void removerBomba(void) {
    if (!filaHead) return;
    Bomba *rem = filaHead;
    filaHead = filaHead->prox;
    if (!filaHead) filaTail = NULL;
    free(rem);
}

int verificarColisao(int playerX, int playerY) {
    for (Bomba *ptr = filaHead; ptr; ptr = ptr->prox)
        if (ptr->x == playerX && ptr->y == playerY) return 1;
    return 0;
}

void limparFila(void) {
    while (filaHead) removerBomba();
}

Bomba* getFilaHead(void) {
    return filaHead;
}