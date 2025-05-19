# 🎮 Console Boom

Console Boom é um jogo retrô em C jogado no terminal, onde o jogador deve desviar de bombas caindo usando as teclas WASD. Cada bomba evitada aumenta a pontuação, e a dificuldade cresce com o tempo. Além disso, uma inteligência artificial integrada (via OpenAI API) interage com o jogador e influencia a dinâmica do jogo em tempo real.

---

## 🧠 Destaques

- Lógica principal baseada em **estruturas de dados** (Fila e Árvore Binária)
- **Ordenação com QuickSort** para ranking de pontuação
- **API de IA (GPT da OpenAI)** para gerar mensagens e eventos dinâmicos
- Totalmente em **C (C99)**, rodando via terminal

---

## 🔧 Compilação

Você precisa do **CMake** e de uma versão do GCC com suporte a C99.

### 📦 Dependência:
- `libcurl` (para integração com a API do Gemini)

### ✅ Compilação via terminal:

```bash
cmake -S . -B build
cmake --build build
