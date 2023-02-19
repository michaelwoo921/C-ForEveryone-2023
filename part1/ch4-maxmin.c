#include <stdio.h>

void print_info(void);
float maximum(float, float);
float minimum(float, float);

int main(void){
    int i, n;
    float max, min, x;

    print_info();
    printf("Input n: ");
    scanf("%d", &n);
    printf("\nInput %d real numbers: ", n);

    scanf("%f", &x);
    max = min =x;

    for(i=2;i<=n;i++){
        scanf("%f", &x);
        max = maximum(max, x);
        min = minimum(min, x);
    }

    printf("\n%s%11.3f\n%s%11.3f\n\n", "Maximum value: ", max, "Minimum Value:", min);

    return 0;
}

void print_info(void){

}

float maximum(float x, float y){
    if(x >=y)
        return x;
    else
        return y;
}

float minimum(float x, float y){
    if(x >=y)
        return y;
    else
        return x;
}