#include <stdio.h>

int main(void){
    int miles = 26 , yards = 385;
    int x;
    float kilometers;

    printf("Type input \n");
    while(scanf("%d", &x) == 1)
        printf("%d\t\t", x);



    kilometers = 1.609 * ( miles + yards / 1760.0);

    printf("\nA marathon is %.2f kilometers.\n\n", kilometers);
    
    return 0;
}