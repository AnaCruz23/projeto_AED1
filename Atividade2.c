
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

/* ---------------------------------------------------------------
 * cria uma MATRIZ dinâmica de inteiros (linhas x colunas)
 * Retorna um ponteiro para ponteiro (int **): cada posição do
 * vetor externo aponta para um vetor de inteiros (uma linha).
 * --------------------------------------------------------------- */
int **criarMatriz(int linhas, int colunas) {

    // aloca o vetor de ponteiros (um ponteiro por linha)
    int **matriz = (int **)malloc(linhas * sizeof(int *));
    if (matriz == NULL) return NULL;

    for (int i = 0; i < linhas; i++) {
        // aloca cada linha como um vetor de inteiros
        matriz[i] = (int *)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            // preenche com 0 ou 1 aleatoriamente (dois "tipos" de célula)
            matriz[i][j] = GetRandomValue(0, 1);
        }
    }
    return matriz;
}

/* libera a memória da matriz: primeiro cada linha, depois o vetor de linhas */
void liberarMatriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);   // libera cada linha
    }
    free(matriz);           // libera o vetor de ponteiros
}

/* desenha a matriz na tela, célula por célula */
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

/* ---------------------------------------------------------------
 * cria o vetor dinâmico de bolas
 * --------------------------------------------------------------- */
Bola *criarBolas(int quantidade) {
    Bola *bolas = (Bola *)malloc(quantidade * sizeof(Bola));
    if (bolas == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        // usar (bolas + i) é o mesmo que &bolas[i]: aqui acessamos
        // o campo via ponteiro para deixar explícito o conceito.
        Bola *b = (bolas + i);
        b->pos = (Vector2){ GetRandomValue(50, LARGURA_JANELA - 50),
                             GetRandomValue(50, ALTURA_JANELA - 50) };
        b->vel = (Vector2){ (float)GetRandomValue(-4, 4),
                             (float)GetRandomValue(-4, 4) };
        b->raio = (float)GetRandomValue(10, 25);
        b->cor  = (Color){ GetRandomValue(100,255), GetRandomValue(100,255),
                            GetRandomValue(100,255), 255 };
    }
    return bolas;
}

/* atualiza a posição de UMA bola: recebe um PONTEIRO para a struct,
 * então as alterações afetam diretamente o vetor original (sem cópia) */
void atualizarBola(Bola *b) {
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;

    // rebate nas bordas
    if (b->pos.x - b->raio < 0 || b->pos.x + b->raio > LARGURA_JANELA)
        b->vel.x *= -1;
    if (b->pos.y - b->raio < 0 || b->pos.y + b->raio > ALTURA_JANELA)
        b->vel.y *= -1;
}

int main(void) {
    srand((unsigned int)time(NULL));

    InitWindow(LARGURA_JANELA, ALTURA_JANELA,
               "Ponteiros e Alocacao Dinamica - raylib");
    SetTargetFPS(60);

    int linhas   = ALTURA_JANELA / TAM_CELULA;
    int colunas  = LARGURA_JANELA / TAM_CELULA;
    int **grade  = criarMatriz(linhas, colunas);   // matriz dinâmica

    int quantidadeBolas = 12;
    Bola *bolas = criarBolas(quantidadeBolas);      // vetor dinâmico

    while (!WindowShouldClose()) {

        //TECLA DE ESPAÇO: ADICIONANDO A BOLA
        if (IsKeyPressed(KEY_SPACE)){
            quantidadeBolas++;
            // Realoca a memória para o novo tamanho
            Bola *temp=(Bola*)realloc(bolas, quantidadeBolas * sizeof(Bola));
            if(temp != NULL){
                bolas=temp;
                // inicializaapenas a nova bola do último índice
                inicializarBola(&bolas[quantidadeBolas - 1]);

            }else{
                quantidadeBolas--;
            }
        }
        // TECLA BACKSPACE: REMOVER ÚLTIMA BOLA
        if (IsKeyPressed (KEY_BACKSPACE) && quantidadeBolas>0){
            quantidadeBolas--;

            if (quantidadeBolas==0){
                free(bolas);
                bolas=NULL;
            }else{
                // Reduz o bloco de memórias
                Bola *temp= (Bola*)realloc (bolas, quantidadeBolas * sizeof(Bola));
                if(temp != NULL){
                    bolas = temp;
                }
            }

        }
        // Atualização de posição das bolas
        for (int i=0; i< quantidadeBolas; i++){
            atualizarBola(bolas + i);
        }
        // Marcar células visitadas na grade
        for (int i=0; i< quantidadeBolas; i++){
            int coluna= (int)(bolas[i].pos.x / TAM_CELULA);
            int linha= (int) (bolas[i].pos.y/ TAM_CELULA);
        // verificação de limites antes do acesso à matriz
            if (linha>=0 && linha<linhas && coluna >= 0 && coluna < colunas){
                 grade [linha][coluna]=1;
            }
        }
        // Contagem das células visitada no quadro atual
            int celulasVisitadas=0;
            for (int i=0; i<linhas; i++){
                for (int j=0; j< colunas; j++){
                    if (grade [i][j]==1){
                        celulasVisitadas++;
                    }
                }
            }
              // Desenhar na tela
        BeginDrawing();
            ClearBackground(RAYWHITE);

            desenharMatriz(grade, linhas, colunas);

            for (int i = 0; i < quantidadeBolas; i++) {
                DrawCircleV(bolas[i].pos, bolas[i].raio, bolas[i].cor);
            }
        // Exibição dos dados
            DrawText(TextFormat("Bolas ativas: %d", quantidadeBolas), 10, 10, 20, GREEN);
            DrawText (TextFormat("Células visitadas: %d / %d", celulasVisitadas, linhas*colunas), 10,35,20, YELLOW);
            DrawText ("[ESPAÇO]Adicionar bola | [BACKSPACE] Remover bola", 10, 35, 16, RAYWHITE);
            DrawText("Pressione ESC para sair", 10, ALTURA_JANELA - 25, 16, RAYWHITE);

        EndDrawing();
    }

    // libera TODA a memória alocada dinamicamente antes de encerrar
   if ( bolas != NULL){
        free(bolas);
   }
    
    liberarMatriz(grade, linhas);

    CloseWindow();
    return 0;
}