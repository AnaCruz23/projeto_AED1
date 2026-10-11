#include "raylib.h"
#include "entidade.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA_JANELA  800
#define ALTURA_JANELA   600


int main (){
    srand((unsigned int)time(NULL));

    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 2 - teste de modularização");
    SetTargetFPS(60);

    Entidade* fada =entidadeCriar(ENTIDADE_JOGADOR, (Vector2){ LARGURA_JANELA / 4.0f, ALTURA_JANELA / 2.0f });
    Entidade* inimigo =entidadeCriar(ENTIDADE_INIMIGO, (Vector2){ LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f });
    Entidade* item =entidadeCriar(ENTIDADE_ITEM, (Vector2){LARGURA_JANELA / 5.0f, ALTURA_JANELA / 3.0f });

    while (!WindowShouldClose()) {
        BeginDrawing();
            
            ClearBackground(RAYWHITE);

            entidadeDesenhar(fada);
            entidadeDesenhar(inimigo);
            entidadeDesenhar(item);

            DrawText("Atividade 2: Fada (Azul), Inimigo (Vermelho) e Item (Dourado)", 20, 20, 20, DARKGRAY);
        EndDrawing();
    }

    free(fada);
    free(inimigo);
    free(item);

    CloseWindow();
    return 0;

}


// para compilar: C:/msys64.1/ucrt64/bin/gcc.exe atividade8b.c entidade.c -o programa.exe -I 
//C:/msys64.1/ucrt64/include -L C:/msys64.1/ucrt64/lib -lraylib -lopengl32 -lgdi32 -lwinmm
// lembrar de usar o "cd" para entrar na pasta do projeto antes de compilar.