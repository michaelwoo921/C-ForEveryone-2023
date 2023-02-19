#include <stdio.h>

enum day {sun, mon, tue, wed, thu, fri, sat};  // declare type 
typedef enum day day;  // you can use day d instead of enum day d

void print_day(day d){
    switch(d){
        case sun: printf(" sunday "); break;
        case mon: printf(" monday "); break;
        case tue: printf(" tuesday "); break;
        case wed: printf(" wednesday "); break;
        case thu: printf(" thursday "); break;
        case fri: printf(" friday "); break;
        case sat: printf(" satday "); break;
        default: printf("%d  is an error", d);
    }
}

day next_day(day d)
{
    return (d+1) % 7;
}

int main(void){
    day d = sat;
    print_day(next_day(d));

    return 0;
}