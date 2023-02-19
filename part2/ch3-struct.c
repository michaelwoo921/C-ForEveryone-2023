#include <stdio.h>

int main(void){

    struct card {
        int pips; char suit;   // pips 1--13,  suit clubs, diamonds, hearts, spades
    } c1, c2, c3, *p;

    typedef struct {double re; double im;} complex;

    complex c;
    c.re =0.2, c.im = 1.5;

    printf("value c is %.2f + %.2f i\n", c.re, c.im);




    c1.pips = 3, c1.suit = 'h';
    p = &c1;
    printf("%d --- %c\n", p->pips, p -> suit);



    return 0;
}