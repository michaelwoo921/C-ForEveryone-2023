#include <stdio.h>

#define SIZE 5

void print_grades(int how_many, int grades[]){
    int i;
    printf("I have %d grades\n", how_many);
      for (i = 0; i< how_many;i++)
        printf("%d\t", grades[i]);  
}

double average_grades(int how_many, int grades[]){
    int i;
    double sum =0.0;

    for(i = 0 ; i <SIZE; i++)
        sum = sum + grades[i];

    return (sum/how_many);

}

int main(void){
    // const int SIZE =5;

    
    int grades[SIZE] = {78,67,92,83, 88};


    int *p = grades;


    print_grades(SIZE, grades);

    printf("\n\n");

    printf("My average is %.2f\n\n", average_grades(SIZE, grades));
    printf(" pointer: %p, --- %lu,  its values: %d,  %d --- addresses for p and p+5 are %lu,  %lu\n", p, p, *p, *(p+1), p, p+5);

    return 0;

}