/*
to turn off debug either
#define NDEBUG or
gcc -DNDEBUG test.c

*/

#define NDEBUG  // goes before <assert.h>
#include <assert.h>
#include <stdio.h>



int main(void){
    double x, y;
    while (1){
        printf("Read in 2 floats:\n");
        scanf("%lf  %lf", &x, &y);  // try 2 5,  0 4,  4 0, 0 0
        assert(y != 0);
        printf("when divided x/y = %lf\n", x/y);
    }

    return 0;
}