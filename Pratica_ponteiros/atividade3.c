#include <stdio.h>
int main(void) {
 int a, b, c, d;
 int *p1;
 int *p2 = &a; //
 int *p3 = &c;
 p1 = p2;
 *p2 = 10;
 b = 20;
 int **pp;
 pp = &p1;
 *p3 = **pp;
 int *p4 = &d;
 *p4 = b + (*p1)++;// pós-incremento: primeiro soma e depois incrementa
 printf("%d\t%d\t%d\t%d\n", a, b, c, d);
 return 0;
}

//*p1=&a; *p2=&a; *p3=&c; *p4=&d; **pp=&a;
//a=11; b=20; c=11; d=30;
//Por que d é igual a 30 e não a 31? porque o operador de pós-incremento (*p1)++ incrementa o valor de 'a' após a expressão ser avaliada. 
//Portanto, na expressão *p4 = b + (*p1)++, o valor de *p1 (que é 10) é usado na soma com b (20), 
//resultando em 30. Depois disso, *p1 é incrementado para 11, mas isso não afeta o valor de d, que já foi calculado como 30.