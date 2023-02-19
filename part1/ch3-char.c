/* Char in C 

*/

#include <stdio.h>

int main(void){

    char c = 'a';
    printf("c in ASCII is %d\n", c);
    printf("c in ASCII is %c\n", c);
    printf("Three consecutive characters are %c%c%d \n", c, c+1, c+2);
    printf("Three bell ring characters are : %c%c%c \n", '\a', '\a', '\a');


    return 0;
}

