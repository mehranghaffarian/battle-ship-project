# include <stdio.h>
# include <stdlib.h>
# include <windows.h>

char board[10][10];

typedef struct ships_data{
    int arr[4];//xs, xe, ys, ye->->xs:x_start, ye:y_end
    struct ships_data* next;
}ships;

ships* new_ship(ships* head, int arr[4]){
    ships* new = (ships*)malloc(sizeof(ships));
    new->next = NULL;

    for(int i = 0;i < 4;i++)
        new->arr[i] = arr[i];

    return new;
}
void add_end(ships* head, ships* new){
    ships* curr = head;

    for(;curr->next != NULL;curr = curr->next);

    curr->next = new;
}
int check_value(ships* curr, int arr[]){
   int counter = 0;

    for(int i = 0;i < 4;i++) {
        if (arr[i] == curr->arr[i])
            counter++;
    }

        if(counter == 4)
            return 1;

        else
            return 0;
}
void remove_ship(ships** head, int arr[]){
    ships* curr = *(head);

    if(check_value((*head), arr)){
        ships* hold = (*head);
        (*head) = (*head)->next;
        free(hold);
    }
    else{
        while(! check_value(curr->next, arr))
            curr = curr->next;

        ships* hold = curr->next;
        curr->next = curr->next->next;
        free(hold);
    }
}
void print(void){
    for(int i = 0;i < 10;i++){
        for(int j = 0;j < 10;j++)
            printf(" E |");

        printf("\n");

        for(int k = 0;k < 10;k++)
            printf("___|");

        printf("\n");
    }
}

int main(void) {


    return 0;
}
