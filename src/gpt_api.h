#ifndef GPT_API_H
#define GPT_API_H

// Envia prompt para a API do ChatGPT e armazena resposta interna
void chamarIA(const char *prompt);

// Exibe a última resposta recebida da IA
void exibirMensagemIA(void);

#endif // GPT_API_H
