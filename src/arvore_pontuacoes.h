#ifndef ARVORE_PONTUACOES_H
#define ARVORE_PONTUACOES_H

typedef struct No {
    char nome[50];
    int pontos;
    struct No *esq, *dir;
} No;

void inserirPontuacao(const char *nome, int pontos);
void exibirRanking(void);
No* buscarPontuacao(const char *nome);
void salvarRankingEmArquivo(const char *filename);

#endif // ARVORE_PONTUACOES_H