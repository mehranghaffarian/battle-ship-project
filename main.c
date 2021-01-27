# include <stdio.h>
# include <stdlib.h>
# include <windows.h>
# include <stdbool.h>

int point1 = 0, point2 = 0;

typedef struct ships_data1{
    int arr1[4];//xs, xe, ys, ye->->xs:x_start, ye:y_end
    struct ships_data1* next;
}ships1;
typedef struct ships_data2{
    int arr2[4];//xs, xe, ys, ye->->xs:x_start, ye:y_end
    struct ships_data2* next;
}ships2;

char board1[10][10] = {"\0"}, board2[10][10] = {"\0"};//board1 : the board where player1 puts his/her ships in//board2 : the board where player2 puts his/her ships in

ships1* head1;
ships2* head2;

ships1* new_ship1(int arr[4]){
    ships1* new = (ships1*)malloc(sizeof(ships1));
    new->next = NULL;

    for(int i = 0;i < 4;i++)
        new->arr1[i] = arr[i];

    return new;
}
ships2* new_ship2(int arr[4]){
    ships2* new = (ships2*)malloc(sizeof(ships2));
    new->next = NULL;

    for(int i = 0;i < 4;i++)
        new->arr2[i] = arr[i];

    return new;
}
void add_end1(ships1* new){
    ships1* curr = head1;

    for(;curr->next != NULL;curr = curr->next);

    curr->next = new;
}
void add_end2(ships2* new){
    ships2* curr = head2;

    for(;curr->next != NULL;curr = curr->next);

    curr->next = new;
}
int check_value1(ships1* curr, int arr[]){
    int counter = 0;

    for(int i = 0;i < 4;i++) {
        if (arr[i] == curr->arr1[i])
            counter++;
    }

    if(counter == 4)
        return 1;

    else
        return 0;
}
int check_value2(ships2* curr, int arr[]){
    int counter = 0;

    for(int i = 0;i < 4;i++) {
        if (arr[i] == curr->arr2[i])
            counter++;
    }

    if(counter == 4)
        return 1;

    else
        return 0;
}
void remove_ship1(int arr[]){
    ships1* curr = head1;

    if(check_value1(head1, arr) && head1->next != NULL) {
        ships1 *hold = head1;
        head1 = head1->next;
        free(hold);
    }
    else if(check_value1(head1, arr) && head1->next == NULL){
        head1->arr1[0] = -1;
    }
    else{
        while(! check_value1(curr->next, arr))
            curr = curr->next;

        ships1* hold = curr->next;
        curr->next = curr->next->next;
        free(hold);
    }
}
void remove_ship2(int arr[]){
    ships2* curr = head1;

    if(check_value2(head2, arr)) {
        ships2 *hold = head2;
        head2 = head2->next;
        free(hold);
    }
    else{
        while(! check_value2(curr->next, arr))
            curr = curr->next;

        ships2* hold = curr->next;
        curr->next = curr->next->next;
        free(hold);
    }
}
int find_length(int arr[]){
    int len;

    if(arr[0] == arr[2]) {
        len = arr[3] - arr[1];
        len += len > 0 ? 1 : -1;
        len = len * (len > 0 ? 1 : -1);
//for simples ships
        if (len == 0)
            len++;
    }
    else if(arr[1] == arr[3]) {
        len = arr[0] - arr[2] + 1;
        len += len > 0 ? 1 : -1;
        len = len * (len > 0 ? 1 : -1);
//for simples ships
        if (len == 0)
            len++;
    }
    return len;
}

//S:a ship is in this place but it has not been a target, W:this place has no ship and has been a target, C:complete explosion has happened

void fill_board1(int arr[]){
    if(find_length(arr) == 1)
        board1[arr[0]][arr[1]] = 'S';

//is the ship in a row?
    else if(arr[1] == arr[3]){
        for(int i = arr[0];i <= arr[2];i++)
            board1[i][arr[1]] = 'S';
    }

//is the ship in a column?
    else if(arr[0] == arr[2])
        for(int i = arr[1];i <= arr[3];i++)
        board1[arr[0]][i] = 'S';

}
void fill_board2(int arr[]){
    if(find_length(arr) == 1)
        board2[arr[0]][arr[1]] = 'S';

//is the ship in a row?
    else if(arr[1] == arr[3]){
        for(int i = arr[0];i <= arr[2];i++)
            board2[i][arr[1]] = 'S';
    }

//is the ship in a column?
    else if(arr[0] == arr[2])
        for(int i = arr[1];i <= arr[3];i++)
            board2[arr[0]][i] = 'S';

}
bool is_located(int arr[]){
    bool located;

    if(find_length(arr) == 1 && board1[arr[0]][arr[1]] == 'S')
       located = 0;


}
void get_inputs(int player){
    if(player == 1) {
        int counter = 0, ships_sum = 21, arr[4];

        printf("\nplayer%d please enter the x and y of the ships(ships end and first) in order. if it is a ship with length of 1 please enter its start x and y twice\n",
               player);

        while (counter < ships_sum) {
            scanf("%d %d", &arr[0], &arr[1]);
            scanf("%d %d", &arr[2], &arr[3]);

            for(int f = 0;f < 4;f++)
                arr[f] -= 1;//beacuse of the fact that arrays start from 0

                if(! is_located(arr)) {
                    if (arr[0] <= 10 && arr[1] <= 10 && arr[2] <= 10 && arr[3] <= 10) {
                        if (counter + find_length(arr) <= ships_sum) {
                            if (counter == 0)
                                head1 = new_ship1(arr);

                            else
                                add_end1(new_ship1(arr));

                            fill_board1(arr);

                            counter += find_length(arr);
                        }
                    }
                }
                else
                    printf("\nOops!!: your input is not acceptable\n");
        }
    }
    else {
        int counter = 0, ships_sum = 21, arr[4];

        printf("\nplayer%d please enter the x and y of the ships(ships end and first) in order. if it is a ship with length of 1 please enter its start x and y twice\n",
               player);

        while (counter < ships_sum) {
            scanf("%d %d", &arr[0], &arr[1]);
            scanf("%d %d", &arr[2], &arr[3]);

            for(int f = 1;f <= 4;f++)
                arr[f] -=1;//beacuse of the fact that arrays start from 0

            if (counter == 0)
                head2 = new_ship2(arr);

            else
                add_end2(new_ship2(arr));

            fill_board2(arr);

            counter += find_length(arr);
        }
    }
}
void print(int player){
    if(player == 1) {
        printf("  ");
        for(int x = 1;x <= 10;x++){
            printf("  %.2d",x);
        }
        printf("\n   _________________________________________");
        printf("\n");
        for (int i = 0; i < 10; i++) {
            printf("%.2d |", 1 + i);
            for (int j = 0; j < 10; j++)
                printf(" %c |", board1[i][j]);

            printf("\n   |");

            for (int k = 0; k < 10; k++)
                printf("___|");

            printf("\n");
        }
    }
    else {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++)
                printf(" %c |", board2[i][j]);

            printf("\n");

            for (int k = 0; k < 10; k++)
                printf("___|");

            printf("\n");
        }
    }
}

int main(void) {
    get_inputs(1);
//    get_inputs(2);

//    while(head1->arr1[0] != -1 && head2->arr2[0] != -1){
//
//    }
print(1);
    return 0;
}
//1 1
//1 10
//2 1
//2 10
//3 1
//3 1