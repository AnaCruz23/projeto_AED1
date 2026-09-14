#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define RAIO_JOGADOR   20.0f
#define TOTAL_INIMIGOS 8
#define DANO_TIRO      20

typedef enum {
    INIMIGO_VIVO,
    INIMIGO_MORTO
} EstadoInimigo;

typedef struct {
    Vector2       pos;
    float         raio;
    int           vida;
    EstadoInimigo estado;
} Inimigo;


void inicializarInimigos(Inimigo *vetor, int n) {
    for (int i = 0; i < n; i++) {
        Inimigo *ini = (vetor + i); // ponteiro para o i-ésimo elemento
        ini->pos    = (Vector2){ GetRandomValue(30, LARGURA_JANELA - 30),
                                  GetRandomValue(30, ALTURA_JANELA - 30) };
        ini->raio   = 15.0f;
        ini->vida   = 60;
        ini->estado = INIMIGO_VIVO;
    }
}

void atingirInimigo(Inimigo *inimigo, int dano) {
    if (inimigo == NULL || inimigo->estado == INIMIGO_MORTO) return;

    inimigo->vida -= dano;
    if (inimigo->vida <= 0) {
        inimigo->vida = 0;
        inimigo->estado = INIMIGO_MORTO;
    }
}
void curarTodos(Inimigo *vetor, int n, int cura){ 
    if ( vetor == NULL) return;          // Criação de uma nova função - exercício 1

     for (int i=0; i<n; i++){
         Inimigo *ini= (vetor + i);
         if (ini->estado == INIMIGO_MORTO)continue; // se o inimigo estiver morto ignorar o próximo passo dentro do for - exercício 1
         ini -> vida += cura; // adiciona o valor do parâmetro
         if ( ini -> vida > 60){ // Tentativa de fixar o valor em 60 como valor máximo de vida - exercício 1
            ini -> vida=60;
         }
    }
}


Inimigo *encontrarInimigoMaisProximo(Inimigo *vetor, int n, Vector2 posJogador) {

    Inimigo *maisProximo = NULL;
    float menorDistancia = 0.0f;

    for (int i = 0; i < n; i++) {
        Inimigo *ini = (vetor + i);
        if (ini->estado == INIMIGO_MORTO) continue;

        float dx = ini->pos.x - posJogador.x;
        float dy = ini->pos.y - posJogador.y;
        float distancia = sqrtf(dx * dx + dy * dy);

        if (maisProximo == NULL || distancia < menorDistancia) {
            maisProximo = ini;
            menorDistancia = distancia;
        }
    }
    return maisProximo;
}

Inimigo *encontrarInimigoMaisFraco(Inimigo* vetor, int n){ //Exercício 02
      if (vetor == NULL) return NULL;

      Inimigo *maisFraco = NULL;
    
      for (int i=0; i<n; i++){
        Inimigo *ini= (vetor+i);

        if ( ini->estado== INIMIGO_MORTO) continue;

        if (maisFraco == NULL || ini->vida < maisFraco -> vida){
            maisFraco = ini;
        }
    }
    return maisFraco; // retorna o ponteiro para o inimigo vivo com a menor vida atual
}

void desenharInimigo(Inimigo *ini) {
    if (ini->estado == INIMIGO_MORTO) return;
    Color cor = (ini->vida > 30) ? MAROON : ORANGE;
    DrawCircleV(ini->pos, ini->raio, cor);
    DrawText(TextFormat("%d", ini->vida), ini->pos.x - 8, ini->pos.y - 26, 14, BLACK);
}

int main(void) {
    int cura=15;
     srand((unsigned int)time(NULL));

    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 4 - Ponteiros para Struct + Vetor de Struct");
    SetTargetFPS(60);

    Vector2 jogador = { LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f };

    // vetor de struct: um único bloco contíguo de memória com TOTAL_INIMIGOS structs
    Inimigo *inimigos = (Inimigo *)malloc(TOTAL_INIMIGOS * sizeof(Inimigo));
    inicializarInimigos(inimigos, TOTAL_INIMIGOS);

    while (!WindowShouldClose()) {

        float vel = 250.0f * GetFrameTime();
        if (IsKeyDown(KEY_RIGHT)) jogador.x += vel;
        if (IsKeyDown(KEY_LEFT))  jogador.x -= vel;
        if (IsKeyDown(KEY_UP))    jogador.y -= vel;
        if (IsKeyDown(KEY_DOWN))  jogador.y += vel;

        if (IsKeyPressed(KEY_SPACE)) {
            
            Inimigo *alvo = encontrarInimigoMaisProximo(inimigos, TOTAL_INIMIGOS, jogador);
            atingirInimigo(alvo, DANO_TIRO);
        }
        if (IsKeyPressed (KEY_F)){ // nova tecla de ataque- exercício 2

            Inimigo *alvo = encontrarInimigoMaisFraco(inimigos, TOTAL_INIMIGOS);
            atingirInimigo (alvo,DANO_TIRO);
        }

        if (IsKeyPressed(KEY_C)){ // Nova tecla: exercício 1

            curarTodos(inimigos, TOTAL_INIMIGOS, cura); // introdução de uma nova função associada a uma tecla (exercício 1)
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < TOTAL_INIMIGOS; i++) {
                desenharInimigo(inimigos + i);
            }

            DrawCircleV(jogador, RAIO_JOGADOR, BLUE);

            DrawText("ESPACO atira no inimigo vivo mais proximo | F: atira no mais fraco", 10, 10, 18, DARKGRAY);
            DrawText ("C: cura para todos os inimigos vivos", 10, 32, 18, DARKGRAY); // adiciona nova tecla
            DrawText("Setas movem o jogador | ESC sai", 10, ALTURA_JANELA - 25, 16, GRAY);

        EndDrawing();
    }

    free(inimigos); // libera o vetor de struct

    CloseWindow();
    return 0;
}
