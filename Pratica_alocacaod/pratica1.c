#include<stdio.h>
#include<stdlib.h>
int main (){
    int *vetor=(int*)malloc(5*sizeof(int));
    if (vetor==NULL){
        printf ("Erro de alocação de memória\n");
        return 1;
    }
    printf("Digite um valor");
    for (int i=0; i<n; i++){
        scanf ("%d", &vetor[n]);
    }
    for (int i=0; i<n; i--){
        printf ("%d", vetor[n]);
    }
    return 0;
    

}