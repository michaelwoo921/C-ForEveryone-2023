#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int cnt;

int get_n_from_user(void);
void move(int n, char a, char b, char c);



int get_n_from_user(void){
    int n;

    printf("%s---", "Towers of Hanoi: \n" "\n" "there are three towers: A, B, and C: \n"
        "The problem starts with n disks on Tower A.\n"
        "Input n: ");

    if(scanf("%d", &n) !=1 || n <1){
        printf("\nERROR: Positive integer not found - bye\n\n");
        exit(1);
    }
    printf("\nn is %d\n", n);
    return n;

}

void move(int n, char a, char b, char c){

   
    if(n==1){
         ++cnt;
        printf("%5d: %s%d%s%c%s%c.\n", cnt, "move Disk ", 1, " from tower ", a, " to tower ", c );
    }
    else {
        move(n-1, a, c, b);
        ++cnt;
        printf("%5d: %s%d%s%c%s%c.\n", cnt, "move Disk ", n, " from tower ", a, " to tower ", c );
        move(n-1, b, a, c);

    }
}

