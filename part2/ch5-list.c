#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct list{int data; struct list *next;} list;

int is_empty(const list *l){ return (l == NULL); }

void print_list(list *h, char *title)
{
    printf("%s\n", title);
    while ( h != NULL){
        printf("%d :", h -> data);
        h = h -> next;
    }
}

list* create_list(int d){
    list* head = malloc(sizeof(list));
    head -> data = d;
    head -> next = NULL;
    return head;
}

list* add_to_front(int d, list* h){
    list* head = create_list(d);
    head -> next = h;
    return head;
}

list* array_to_list(int d[], int size){
    list* head  = create_list(d[0]);
    int i;
    for(i=1;i<size;i++)
    {
        head = add_to_front(d[i], head);
    }
    return head;
}

int main(void){
    list list_of_int;
    list* head = NULL;
    int data[6] ={2,4,6,8,10,12};

    head = malloc(sizeof(list));
    printf("sizeof(list)  = %lu\n", sizeof(list));
    head -> data = 5;
    head -> next = NULL;
    print_list(head, "single element list");
    printf("\n\n");
    head = array_to_list(data, 6);
    print_list(head, "data[6] to 6 element list");
    
    return 0;
    
}


