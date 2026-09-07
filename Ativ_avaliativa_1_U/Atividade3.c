

#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define RAIO_JOGADOR   20.0f
#define TOTAL_ITENS    10

typedef enum {
    ITEM_ARMA,
    ITEM_POCAO,
    ITEM_ESCUDO // Adicionando um tipo de item extra (exercício 1)
} TipoItem;

typedef struct {
    float dano;
    int   alcance;
} DadosArma;

typedef struct {
    int cura;
} DadosPocao;

typedef struct { // nova struct criada (exercício 1)
    int absorcao; 
} DadosEscudo;

typedef union {
    DadosArma  arma;
    DadosPocao pocao;
    DadosEscudo escudo; //Adicionando um tipo de item extra (exercício 1)
} DadosItem;

typedef struct {
    Vector2   pos;
    float     raio;
    TipoItem  tipo;
    DadosItem dados;
    bool      coletado;
} Item;

typedef struct {
    Vector2 pos;
    float   raio;
    int     vida;
    float   dano;
    int     armadura; // Adicionando um campo de armadura para o jogador (exercício 2)
} Jogador;

Item *criarItens(int quantidade) {
    Item *itens = (Item *)malloc(quantidade * sizeof(Item));
    if (itens == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        Item *it = (itens + i);
        it->pos      = (Vector2){ GetRandomValue(30, LARGURA_JANELA - 30),
                                   GetRandomValue(30, ALTURA_JANELA - 30) };
        it->raio     = 12.0f;
        it->coletado = false;
        it->tipo     = (TipoItem)GetRandomValue(ITEM_ARMA, ITEM_ESCUDO);

        if (it->tipo == ITEM_ARMA) {
            it->dados.arma.dano    = (float)GetRandomValue(2, 8);
            it->dados.arma.alcance = GetRandomValue(1, 3);
        } if(it->tipo == ITEM_ESCUDO) {         // Item escudo adicionado (exercício 1)
            it->dados.escudo.absorcao = GetRandomValue(5, 15);
        } if (it->tipo == ITEM_POCAO){
          int ValorCura= GetRandomValue(10, 30); // Sorteia um valor de cura entre esses valores para não alterar o tipo enum (exercíco 2)
          int ChanceArmadilha= GetRandomValue(0,9); // sorteia a probabilidade (exercício 2)
          if (ChanceArmadilha < 3){
            it -> dados.pocao.cura= -ValorCura;
           } else {
            it -> dados.pocao.cura= ValorCura;
           }
        }
    }
    return itens;
}

void aplicarItem(Jogador *j, Item *item) {
    switch (item->tipo) {
        case ITEM_ARMA:
            j->dano += item->dados.arma.dano;
            break;
        case ITEM_POCAO:
            j->vida += item->dados.pocao.cura;
            if (j-> vida < 0){ // travando no zero a vida do jogador (exercício 2)
                j-> vida=0;
            } 
            break;
        case ITEM_ESCUDO: //adicionando o item escudo
            j-> armadura += item->dados.escudo.absorcao;
            break;
    }
    item->coletado = true;
}

bool colidiu(Vector2 a, float raioA, Vector2 b, float raioB) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float distancia2 = dx * dx + dy * dy;
    float somaRaios2 = (raioA + raioB) * (raioA + raioB);
    return distancia2 <= somaRaios2;
}

void desenharItem(Item *item) {
    if (item->coletado) return;

    Color cor = BLACK;
    if (item->tipo == ITEM_ARMA){
        cor = RED;
    } else if (item->tipo == ITEM_ESCUDO){ // acrescentando ITEM_ESCUDO (exercício 1)
        cor = BLUE;
    } else if (item->tipo == ITEM_POCAO){
        if (item->dados.pocao.cura > 0){
        cor = GREEN;
        } else{
        cor= PURPLE; // poção envenenada com cor diferente (exercício 2)
        }
    }

    DrawCircleV(item->pos, item->raio, cor);
}

int main(void) {
    srand((unsigned int)time(NULL));

    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 3 - Union + Enum + Struct");
    SetTargetFPS(60);

    Jogador jogador = { { LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f }, RAIO_JOGADOR, 100, 5.0f, 0 };
    Item *itens = criarItens(TOTAL_ITENS); // vetor dinâmico de struct com union

    while (!WindowShouldClose()) {

        float vel = 250.0f * GetFrameTime();
        if (IsKeyDown(KEY_RIGHT)) jogador.pos.x += vel;
        if (IsKeyDown(KEY_LEFT))  jogador.pos.x -= vel;
        if (IsKeyDown(KEY_UP))    jogador.pos.y -= vel;
        if (IsKeyDown(KEY_DOWN))  jogador.pos.y += vel;

        for (int i = 0; i < TOTAL_ITENS; i++) {
            Item *it = (itens + i);
            if (!it->coletado && colidiu(jogador.pos, jogador.raio, it->pos, it->raio)) {
                aplicarItem(&jogador, it); // &jogador: ponteiro para struct
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < TOTAL_ITENS; i++) {
                desenharItem(itens + i);
            }

            DrawCircleV(jogador.pos, jogador.raio, BLUE);

            DrawText(TextFormat("Vida: %d   Dano: %.1f", jogador.vida, jogador.dano), 10, 10, 22, DARKGRAY);
            DrawText("Vermelho = arma (aumenta dano) | Verde = pocao (cura)", 10, 34, 18, GRAY);
            DrawText("Setas movem o jogador | ESC sai", 10, ALTURA_JANELA - 25, 16, GRAY);

        EndDrawing();
    }

    free(itens); // libera o vetor dinâmico

    CloseWindow();
    return 0;
}