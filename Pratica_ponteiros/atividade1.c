int x, y, *p;
y=0;
p=&y; // P recebe o endereço de y;
x=*p; // x recebe o valor de y através de ponteiros;
x=4; // x recebe o valor de 4;
(*p)++; // o valor de y é incrementado em 1;
--x; // x é decrementado em 1;
(*p)+=x; // o valor de y é somado ao x;

//* No final, x=3, y=4 e p=&y