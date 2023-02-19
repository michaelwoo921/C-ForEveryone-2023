/*
FILE I/O
*/

#include <stdio.h>
#define MAX_HW 20

void read_data(FILE *ptr, int d[], int *size){

    *size = 0;
    while(fscanf(ptr, "%d", &d[*size]) ==1){
        (*size)++;
    }
}

void print_data(int d[], int size){
    int i;
    for(i=0; i< size; i++){
        printf("%d\t", d[i]);
        if((i+1) % 10 == 0)
            printf("\n");
    }
}

double average(int d[], int size){
    int i, sum =0;
    for(i=0; i<size; i++){
        sum += d[i];
    }

    return (double) sum / size;
}

int main(void){
    int sz = MAX_HW;
    int data[MAX_HW] = {0};

    FILE *ifp;
    ifp = fopen("ch7-hw.txt", "r");
    
    read_data(ifp, data, &sz);
    printf("my %d homework scores are\n", sz);
    print_data(data, sz);
    printf("average score was %10.2f", average(data, sz));
    printf("\n\n");
    fclose(ifp);


}