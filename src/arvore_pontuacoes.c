#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_pontuacoes.h"
static No*raiz=NULL;
static No*criarNo(const char*n,int p){No*no=malloc(sizeof(No));if(!no)return NULL;strncpy(no->nome,n,49);no->nome[49]='\0';no->pontos=p;no->esq=no->dir=NULL;return no;}
void inserirPontuacao(const char*nome,int pontos){No*novo=criarNo(nome,pontos);if(!novo)return;if(!raiz){raiz=novo;return;}No*cur=raiz,*parent=NULL;while(cur){parent=cur;if(pontos>cur->pontos)cur=cur->dir;else cur=cur->esq;}if(pontos>parent->pontos)parent->dir=novo;else parent->esq=novo;}
static void exibirDec(No*node){if(!node)return;exibirDec(node->dir);printf("%s -> %d\n",node->nome,node->pontos);exibirDec(node->esq);}
void exibirRanking(void){printf("=== Ranking ===\n");exibirDec(raiz);}
static void salvarDec(No*node,FILE*f){if(!node)return;salvarDec(node->dir,f);fprintf(f,"%s %d\n",node->nome,node->pontos);salvarDec(node->esq,f);}
void salvarRankingEmArquivo(const char*filename){FILE*f=fopen(filename,"w");if(!f)return;salvarDec(raiz,f);fclose(f);}