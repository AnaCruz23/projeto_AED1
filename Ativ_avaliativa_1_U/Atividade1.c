#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define TAM_CELULA     40   // tamanho de cada célula da grade (matriz)

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float   raio;
    Color   cor;
} Bola;

void inicializarBola(Bola *b) {
    b->pos = (Vector2){ (float)GetRandomValue(50, LARGURA_JANELA - 50),
                        (float)GetRandomValue(50, ALTURA_JANELA - 50) };
    b->vel = (Vector2){ (float)GetRandomValue(-4, 4),
                        (float)GetRandomValue(-4, 4) };
    b->raio = (float)GetRandomValue(10, 25);
    b->cor  = (Color){ GetRandomValue(100, 255), GetRandomValue(100, 255),
                       GetRandomValue(100, 255), 255 };
}


int **criarMatriz(int linhas, int colunas) {
    int **matriz = (int **)malloc(linhas * sizeof(int *));
    if (matriz == NULL) return NULL;

    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int *)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = GetRandomValue(0, 1);
        }
    }
    return matriz;
}

/* Libera a memória da matriz em duas etapas */
void liberarMatriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

/* Desenha a matriz na tela */
void desenharMatriz(int **matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            Color cor = (matriz[i][j] == 1) ? (Color){20, 40, 70, 255}
                                            : (Color){15, 30, 55, 255};
            DrawRectangle(j * TAM_CELULA, i * TAM_CELULA,
                          TAM_CELULA - 2, TAM_CELULA - 2, cor);
        }
    }
}


Bola *criarBolas(int quantidade) {
    Bola *bolas = (Bola *)malloc(quantidade * sizeof(Bola));
    if (bolas == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        inicializarBola(&bolas[i]);
    }
    return bolas;
}

/* Atualiza a posição de UMA bola via ponteiro */
void atualizarBola(Bola *b) {
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;

    // Rebate nas bordas da janela
    if (b->pos.x - b->raio < 0 || b->pos.x + b->raio > LARGURA_JANELA)
        b->vel.x *= -1;
    if (b->pos.y - b->raio < 0 || b->pos.y + b->raio > ALTURA_JANELA)
        b->vel.y *= -1;
}

int main(void) {
    srand((unsigned int)time(NULL));

    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Ponteiros e Alocacao Dinamica - raylib");
    SetTargetFPS(60);

    int linhas   = ALTURA_JANELA / TAM_CELULA;
    int colunas  = LARGURA_JANELA / TAM_CELULA;
    int **grade  = criarMatriz(linhas, colunas);

    int quantidadeBolas = 12;
    Bola *bolas = criarBolas(quantidadeBolas);

    while (!WindowShouldClose()) {

        // TECLA ESPAÇO: Adicionar nova bola usando realloc 
        if (IsKeyPressed(KEY_SPACE)) {
            quantidadeBolas++;
            Bola *temp = (Bola *)realloc(bolas, quantidadeBolas * sizeof(Bola));
            if (temp != NULL) {
                bolas = temp;
                inicializarBola(&bolas[quantidadeBolas - 1]);
            } else {
                quantidadeBolas--; // Reverte a contagem se falhar a alocação
            }
        }

        // TECLA BACKSPACE: Remover última bola ajustando a memória 
        if (IsKeyPressed(KEY_BACKSPACE) && quantidadeBolas > 0) {
            quantidadeBolas--;

            if (quantidadeBolas == 0) {
                free(bolas);
                bolas = NULL;
            } else {
                Bola *temp = (Bola *)realloc(bolas, quantidadeBolas * sizeof(Bola));
                if (temp != NULL) {
                    bolas = temp;
                }
            }
        }

        // Atualização da posição de todas as bolas
        for (int i = 0; i < quantidadeBolas; i++) {
            atualizarBola(bolas + i);
        }

        // Marcar células da grade visitadas pelas bolas (Exercício 2)
        for (int i = 0; i < quantidadeBolas; i++) {
            int coluna = (int)(bolas[i].pos.x / TAM_CELULA);
            int linha  = (int)(bolas[i].pos.y / TAM_CELULA);

            if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
                grade[linha][coluna] = 1;
            }
        }

        // Contagem de células visitadas no quadro atual (Exercício 2)
        int celulasVisitadas = 0;
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (grade[i][j] == 1) {
                    celulasVisitadas++;
                }
            }
        }

        // Renderização gráfica
        BeginDrawing();
            ClearBackground(RAYWHITE);

            desenharMatriz(grade, linhas, colunas);

            for (int i = 0; i < quantidadeBolas; i++) {
                DrawCircleV(bolas[i].pos, bolas[i].raio, bolas[i].cor);
            }

            // HUD do jogo (posicionamentos corrigidos sem sobreposição)
            DrawText(TextFormat("Bolas ativas: %d", quantidadeBolas), 10, 10, 20, GREEN);
            DrawText(TextFormat("Células visitadas: %d / %d", celulasVisitadas, linhas * colunas), 10, 35, 20, YELLOW);
            DrawText("[ESPAÇO] Adicionar bola | [BACKSPACE] Remover bola", 10, 60, 16, RAYWHITE);
            DrawText("Pressione ESC para sair", 10, ALTURA_JANELA - 25, 16, RAYWHITE);

        EndDrawing();
    }

    // Liberação de memória antes de encerrar
    if (bolas != NULL) {
        free(bolas);
    }
    liberarMatriz(grade, linhas);

    CloseWindow();
    return 0;
}