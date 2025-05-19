#include <stdio.h>
#include "gpt_api.h"

// Armazena a última resposta da IA
static char respostaIA[1024];

void chamarIA(const char *prompt) {
    // TODO: Implementar libcurl para chamar a API da OpenAI
    // Preencher respostaIA com o texto retornado
}

void exibirMensagemIA(void) {
    printf("IA: %s\n", respostaIA);
}