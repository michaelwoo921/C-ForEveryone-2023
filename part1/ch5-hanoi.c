#include "hanoi.h"


int main(void){
    int n;

    n = get_n_from_user();
    assert(n>0);
    /*
        move n disks from tower A to tower C 
        usng tower B as an itermediate tower
    */

    move(n, 'A', 'B', 'C');
    return 0;
}