// src/gpt_api.c

#include "gpt_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define BUF_SIZE 8192

/* buffer onde guardamos apenas o texto extraído da resposta */
static char resposta[BUF_SIZE];

/* estrutura para coletar toda a resposta bruta da API */
typedef struct {
    char *buffer;
    size_t size;
} MemoryStruct;

/* callback do libcurl que vai preenchendo nosso MemoryStruct */
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct*)userp;
    char *ptr = realloc(mem->buffer, mem->size + realsize + 1);
    if (!ptr) return 0;
    mem->buffer = ptr;
    memcpy(&mem->buffer[mem->size], contents, realsize);
    mem->size += realsize;
    mem->buffer[mem->size] = '\0';
    return realsize;
}

void chamarIA(const char *prompt) {
    CURL *curl;
    CURLcode res;
    MemoryStruct chunk = { .buffer = malloc(1), .size = 0 };

    /* 1) leia sua API key do Gemini (ambiente ou key.txt) */
    const char *api_key = getenv("GEMINI_API_KEY");
    if (!api_key) {
        FILE *f = fopen("key.txt","r");
        if (!f) { fprintf(stderr,"Erro: não achei key.txt\n"); return; }
        static char buf[256];
        fgets(buf, sizeof(buf), f);
        fclose(f);
        buf[strcspn(buf, "\r\n")] = '\0';
        api_key = buf;
    }

    /* 2) monte a URL do endpoint */
    const char *model = "gemini-1.5-flash-latest";
    char api_url[512];
    snprintf(api_url, sizeof(api_url),
        "https://generativelanguage.googleapis.com/v1beta/models/%s:generateContent?key=%s",
        model, api_key);

    /* 3) payload JSON com o prompt */
    char json_payload[1024];
    snprintf(json_payload, sizeof(json_payload),
        "{\"contents\":[{\"parts\":[{\"text\":\"%s\"}]}]}",
        prompt);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr,"Erro: curl_easy_init falhou\n");
        curl_global_cleanup();
        return;
    }

    /* configurações básicas */
    curl_easy_setopt(curl, CURLOPT_URL,            api_url);
    curl_easy_setopt(curl, CURLOPT_POST,           1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,     json_payload);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,      &chunk);
    curl_easy_setopt(curl, CURLOPT_USERAGENT,      "ConsoleBoom-Gemini/1.0");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    /* cabeçalho JSON */
    struct curl_slist *hdr = NULL;
    hdr = curl_slist_append(hdr, "Content-Type: application/json; charset=UTF-8");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdr);

    /* faz a chamada */
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr,"curl falhou: %s\n", curl_easy_strerror(res));
    } else {
        /* extrai só o texto de "text": "..." do JSON bruto */
        char *p = strstr(chunk.buffer, "\"text\":");
        if (p) {
            p = strchr(p, '"');        // primeira "
            p = strchr(p+1, '"');      // segunda "
            p++;                       // início do texto
            char *q = strchr(p, '"');  // final
            if (q && q > p) {
                size_t len = q - p;
                if (len >= BUF_SIZE) len = BUF_SIZE-1;
                memcpy(resposta, p, len);
                resposta[len] = '\0';
            }
        }
    }

    /* limpeza */
    curl_slist_free_all(hdr);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    free(chunk.buffer);
}

void exibirMensagemIA(void) {
    printf("IA: %s\n", resposta);
}
