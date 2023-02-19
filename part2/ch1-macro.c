#include <stdio.h>

#define SIZE 10
#define FILL  for(i=0; i < SIZE; i++) data[i] = i
#define PRINT for(i = 0; i< SIZE; i++) printf("%d -- data[i]\n", data[i])

int main(void){
    int i;
    int data[SIZE] ={0};
    FILL;
    PRINT;
    
    return 0;
}