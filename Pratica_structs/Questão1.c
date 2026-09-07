#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define MAX_TURMAS 3
#define MAX_VAGAS 10

struct aluno{
    int mat;
    char nome [81];
    float notas [3];
    float media;
};
typedef struct aluno Aluno;

struct turma{
    char id;
    int vagas;
    Aluno* alunos[MAX_VAGAS]; //Vetor
};
typedef struct turma Turma;
Turma* turmas[MAX_TURMAS]; // Vetor de ponteiros

Turma* cria_turma(char id){
    /*Malloc reserva o espaço de memória para que a nova turma exista*/
    /*o sizeof calcula exatamente quantos bytes a Turma irá precisar(espaço para id, vagas e
    vetor de ponteiros alunos)*/
    Turma* nova_turma= (Turma*) malloc(sizeof(Turma));
    if (nova_turma == NULL){
        printf ("Não há vagas para essa nova turma\n");
        return NULL; // Caso haja erro na alocação da memória
    }
    /*Guarde o valor do parâmetro id dentro do campo id da estrutura 
    apontada por "nova_turma"*/
       
    nova_turma->id=id;
    /* guarde a quantidade máxima de vagas dentro das vagas, que está dentro 
    dessa estrutura nova_turma.*/
    nova_turma->vagas= MAX_VAGAS;
   /* O malloc aloca a memória mantendo o "lixo" antigo. 
   O loop for percorre o vetor e limpa esse lixo, 
   garantindo que cada posição comece explicitamente como NULL (vazia). */
    for ( int i=0; i<MAX_VAGAS; i++){
        nova_turma->alunos[i]=NULL;
       }
       return nova_turma; // ao terminar de executar, a função devolve o ponteiro da  nova turma e impede o vazamente da memória (memória leak).
    void matricula_aluno(Turma* turma, int mat, char* nome){
        if(turma->vagas==0){
            printf (" Não há vagas para essa turma\n");
            return;
        }
        Turma* turma=
    }
    }

