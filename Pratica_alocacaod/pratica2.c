#include<stdio.h>
#include<stdlib.h>
int main(){
    int **matriz= malloc(linha*sizeof(int*));
    for(int i=0; i<linha; i++){
        matriz[i]= malloc(coluna*sizeof(int));
        if(matriz[i]==NULL){
            printf ("Sem memória");
            exit (1);
        }
    
    }
}