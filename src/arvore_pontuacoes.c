#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_pontuacoes.h"

static No *raiz = NULL;

// Função auxiliar para criar um nó
static No* criarNo(const char *nome, int pontos) {
    No *novo = (No*)malloc(sizeof(No));
    if (!novo) return NULL;
    strncpy(novo->nome, nome, 49);
    novo->nome[49] = '\0';
    novo->pontos = pontos;
    novo->esq = novo->dir = NULL;
    return novo;
}

void inserirPontuacao(const char *nome, int pontos) {
    // TODO: Implementar inserção na BST
}

void exibirRanking(void) {
    // TODO: Implementar travessia in-order e exibição
}

No* buscarPontuacao(const char *nome) {
    // TODO: Implementar busca por nome
    return NULL;
}

void salvarRankingEmArquivo(const char *filename) {
    // TODO: Percorrer árvore e gravar em arquivo
}