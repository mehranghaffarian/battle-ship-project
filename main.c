# include <stdio.h>
# include <stdlib.h>
# include <windows.h>
# include <time.h>

int point1, point2, max_size, ships_sum = 21, arr_size[15], map_size = 10;

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
    if((arr[0] == curr->arr1[0] && arr[2] ==curr->arr1[2] && arr[1] == curr->arr1[1] && arr[3] == curr->arr1[3]) || (arr[0] == curr->arr1[2] && arr[2] ==curr->arr1[0] && arr[1] == curr->arr1[3] && arr[3] == curr->arr1[1]))
        return 1;
}
int check_value2(ships2* curr, int arr[]){
    if((arr[0] == curr->arr2[0] && arr[2] ==curr->arr2[2] && arr[1] == curr->arr2[1] && arr[3] == curr->arr2[3]) || (arr[0] == curr->arr2[2] && arr[2] ==curr->arr2[0] && arr[1] == curr->arr2[3] && arr[3] == curr->arr2[1]))
        return 1;
}
void remove_ship1(int arr[]){
    ships1* curr = head1;

    if(check_value1(head1, arr) && head1->next != NULL) {
        ships1 *hold = head1;
        head1 = head1->next;
        free(hold);
    }
    if(check_value1(head1, arr) && head1->next == NULL){
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
    ships2* curr = head2;

    if(check_value2(head2, arr) && head2->next != NULL) {
        ships2 *hold = head2;
        head2 = head2->next;
        free(hold);
    }
    if(check_value2(head2, arr) && head2->next == NULL){
        head2->arr2[0] = -1;//end of game
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
        len = (len > 0 ? (len + 1) * 1 : -1 * (len - 1));
//for simples ships
        if (len == 0)
            len++;
    }
    else if(arr[1] == arr[3]) {
        len = arr[0] - arr[2] ;
        len = (len > 0 ? (len + 1) * 1 : -1 * (len - 1));
//for simples ships
        if (len == 0)
            len++;
    }
    return len;
}
//S:a ship is in this place but it has not been a target, W:this place has no ship and has been a target, C:complete explosion has happened
void fill_board1(int arr[]){
    int x1 = arr[0] < arr[2] ? arr[0] : arr[2], x2 = arr[2] > arr[0] ? arr[2] : arr[0];
    int y1 = arr[1] < arr[3] ? arr[1] : arr[3], y2 = arr[1] < arr[3] ? arr[3] : arr[1];

    if(find_length(arr) == 1)
        board1[arr[0]][arr[1]] = 'S';

//is the ship in a row?
    else if(arr[1] == arr[3]){
        for(int i = x1;i <= x2;i++)
            board1[i][arr[1]] = 'S';
    }

//is the ship in a column?
    else if(arr[0] == arr[2])
        for(int i = y1;i <= y2;i++)
        board1[arr[0]][i] = 'S';

}
void fill_board2(int arr[]){
    int x1 = arr[0] < arr[2] ? arr[0] : arr[2], x2 = arr[2] > arr[0] ? arr[2] : arr[0];
    int y1 = arr[1] < arr[3] ? arr[1] : arr[3], y2 = arr[1] < arr[3] ? arr[3] : arr[1];

    if(find_length(arr) == 1)
        board2[arr[0]][arr[1]] = 'S';

//is the ship in a row?
    else if(arr[1] == arr[3]){
        for(int i = x1;i <= x2;i++)
            board2[i][arr[1]] = 'S';
    }

//is the ship in a column?
    else if(arr[0] == arr[2])
        for(int i = y1;i <= y2;i++)
            board2[arr[0]][i] = 'S';


}
int is_really_locatable(int array[], int player){
    int is_locatable = 1;

    if(player == 1) {
        if (array[0] >= 10 || array[1] >= 10 || array[0] <= -1 || array[1] <= -1)
            is_locatable = 0;

       if(array[0] - 1 >= 0 && array[1] - 1 >= 0 ) {
            if (board1[array[0] - 1][array[1] - 1] == 'S')
                is_locatable = 0;
        }

       if(array[0] - 1 >= 0) {
            if( board1[array[0] - 1][array[1]] == 'S')
            is_locatable = 0;
        }

      if(array[0] - 1 >= 0 && array[1] + 1 <= 9) {
            if (board1[array[0] - 1][array[1] + 1] == 'S')
                is_locatable = 0;
        }

      if(array[1] + 1 <= 9) {
            if (board1[array[0]][array[1] + 1] == 'S')
                is_locatable = 0;
        }

       if(board1[array[0]][array[1]] == 'S')
            is_locatable = 0;

       if(array[1] - 1 >= 0 ) {
            if (  board1[array[0]][array[1] - 1] == 'S')
            is_locatable = 0;
        }

       if(array[0] + 1 <= 9 && array[1] - 1 >= 0) {
            if ( board1[array[0] + 1][array[1] - 1] == 'S')
            is_locatable = 0;
        }

      if(array[0] + 1 <= 9) {
            if (board1[array[0] + 1][array[1]] == 'S')
                is_locatable = 0;
        }

      if(array[0] + 1 <= 9 && array[1] + 1 <= 9 ) {
            if (board1[array[0] + 1][array[1] + 1] == 'S')
                is_locatable = 0;
        }

    }
    else{
        if (array[0] >= 10 || array[1] >= 10 || array[0] <= -1 || array[1] <= -1)
            is_locatable = 0;

        if(array[0] - 1 >= 0 && array[1] - 1 >= 0 ) {
            if (board2[array[0] - 1][array[1] - 1] == 'S')
                is_locatable = 0;
        }

        if(array[0] - 1 >= 0) {
            if( board2[array[0] - 1][array[1]] == 'S')
                is_locatable = 0;
        }

        if(array[0] - 1 >= 0 && array[1] + 1 <= 9) {
            if (board2[array[0] - 1][array[1] + 1] == 'S')
                is_locatable = 0;
        }

        if(array[1] + 1 <= 9) {
            if (board2[array[0]][array[1] + 1] == 'S')
                is_locatable = 0;
        }

        if(board2[array[0]][array[1]] == 'S')
            is_locatable = 0;

        if(array[1] - 1 >= 0 ) {
            if (  board1[array[0]][array[1] - 1] == 'S')
                is_locatable = 0;
        }

        if(array[0] + 1 <= 9 && array[1] - 1 >= 0) {
            if ( board2[array[0] + 1][array[1] - 1] == 'S')
                is_locatable = 0;
        }

        if(array[0] + 1 <= 9) {
            if (board2[array[0] + 1][array[1]] == 'S')
                is_locatable = 0;
        }

        if(array[0] + 1 <= 9 && array[1] + 1 <= 9 ) {
            if (board2[array[0] + 1][array[1] + 1] == 'S')
                is_locatable = 0;
        }

    }
        return is_locatable;
}
void print(int player){
    if(player == 1) {
        printf("  ");
        for(int x = 1;x <= map_size;x++)
            printf("  %.2d",x);

        printf("\n   ");

        for(int i = 1;i <= 4 * map_size;i++)
            printf("_");

        printf("\n");
        for (int i = 0; i < map_size; i++) {
            printf("%.2d |", 1 + i);
            for (int j = 0; j < map_size; j++)
                printf(" %c |", board1[i][j]);

            printf("\n   |");

            for (int k = 0; k < map_size; k++)
                printf("___|");

            printf("\n");
        }
//        system("CLS");
//        printf("\n enter any key to continue\n");
//        char wait;
//        scanf("%c",&wait);
    }
    else {
        printf("  ");
        for(int x = 1;x <= map_size;x++)
            printf("  %.2d",x);

        printf("\n   ");

        for(int i = 1;i <= 4 * map_size;i++)
            printf("_");

        printf("\n");
        for (int i = 0; i < map_size; i++) {
            printf("%.2d |", 1 + i);
            for (int j = 0; j < map_size; j++)
                printf(" %c |", board2[i][j]);

            printf("\n   |");

            for (int k = 0; k < map_size; k++)
                printf("___|");

            printf("\n");
        }
//        system("CLS");
//        printf("\n enter any key to continue\n");
//        char wait;
//        scanf("%c",&wait);
    }
}
void get_inputs(int player){
    if(player == 1) {
        int counter = 0,  arr[4], is_locatable, temp_array[2], arr_size1[15] = {0};//arr_size[size of ship][number of this ship]

        arr_size1[1] = arr_size[1];
        arr_size1[2] = arr_size[2];
        arr_size1[3] = arr_size[3];
        arr_size1[5] = arr_size[5];

        printf("\nplayer%d please enter the row and column of the ships(ships end and first). if it is a ship with length of 1 please enter its start row and column twice\n",
               player);

        while (counter < ships_sum) {
            is_locatable = 1;

            if(counter == 0)//better view for deciding in the first time
                print(player);

            scanf("%d %d", &arr[0], &arr[1]);
            scanf("%d %d", &arr[2], &arr[3]);

            for(int f = 0;f < 4;f++)
                arr[f] -= 1;//because of the fact that arrays start from 0

                int x1 = arr[0] < arr[2] ? arr[0] : arr[2], x2 = arr[2] > arr[0] ? arr[2] : arr[0];
                int y1 = arr[1] < arr[3] ? arr[1] : arr[3], y2 = arr[1] < arr[3] ? arr[3] : arr[1];

                if(arr[0] != arr[2] && arr[1] != arr[3])
                    is_locatable = 0;

                if(arr[0] == arr[2]){
                    for(int i = y1;i <= y2;i++) {
                        temp_array[0] = arr[0];
                        temp_array[1] = i;
                        if (is_really_locatable(temp_array, player) == 0)
                            is_locatable = 0;
                    }
                }
                if(arr[1] == arr[3]){
                    for(int i = x1;i <= x2;i++) {
                        temp_array[1] = arr[1];
                        temp_array[0] = i;
                        if (is_really_locatable(temp_array, player) == 0)
                            is_locatable = 0;
                    }
                }

                if(is_locatable == 1 && arr_size1[find_length(arr)] != 0 && find_length(arr) <= max_size) {
                    if (counter + find_length(arr) <= ships_sum) {
                        if (counter == 0)
                            head1 = new_ship1(arr);

                        else
                            add_end1(new_ship1(arr));

                        fill_board1(arr);

                        counter += find_length(arr);
                        print(player);
                        arr_size1[find_length(arr)]--;
                    }
                    else
                        is_locatable = 0;
                }
                else
                    is_locatable = 0;

                if(is_locatable == 0)
                    printf("\nOops!!: your input is not acceptable. chose other locations\n");
        }
    }
    else {
        int counter = 0,  arr[4], is_locatable, temp_array[2], arr_size2[15] = {0};//arr_size[size of ship][number of this ship]

        arr_size2[1] = arr_size[1];
        arr_size2[2] = arr_size[2];
        arr_size2[3] = arr_size[3];
        arr_size2[5] = arr_size[5];

        printf("\nplayer%d please enter the row and column of the ships(ships end and first). if it is a ship with length of 1 please enter its start row and column twice\n",
               player);

        while (counter < ships_sum) {
            is_locatable = 1;

            if(counter == 0)//better view for deciding in the first time
                print(player);

            scanf("%d %d", &arr[0], &arr[1]);
            scanf("%d %d", &arr[2], &arr[3]);

            for(int f = 0;f < 4;f++)
                arr[f] -= 1;//because of the fact that arrays start from 0

            int x1 = arr[0] < arr[2] ? arr[0] : arr[2], x2 = arr[2] > arr[0] ? arr[2] : arr[0];
            int y1 = arr[1] < arr[3] ? arr[1] : arr[3], y2 = arr[1] < arr[3] ? arr[3] : arr[1];

            if(arr[0] != arr[2] && arr[1] != arr[3])
                is_locatable = 0;

            if(arr[0] == arr[2]){
                for(int i = y1;i <= y2;i++) {
                    temp_array[0] = arr[0];
                    temp_array[1] = i;
                    if (is_really_locatable(temp_array, player) == 0)
                        is_locatable = 0;
                }
            }
            if(arr[1] == arr[3]){
                for(int i = x1;i <= x2;i++) {
                    temp_array[1] = arr[1];
                    temp_array[0] = i;
                    if (is_really_locatable(temp_array, player) == 0)
                        is_locatable = 0;
                }
            }

            if(is_locatable == 1 && arr_size2[find_length(arr)] != 0 && find_length(arr) <= max_size) {
                if (counter + find_length(arr) <= ships_sum) {
                    if (counter == 0)
                        head2 = new_ship2(arr);

                    else
                        add_end2(new_ship2(arr));

                    fill_board2(arr);

                    counter += find_length(arr);
                    print(player);
                    arr_size2[find_length(arr)]--;
                }
                else
                    is_locatable = 0;
            }
            else
                is_locatable = 0;

            if(is_locatable == 0)
                printf("\nOops!!: your input is not acceptable. chose other locations\n");
        }
    }
}
int is_shotable(int temp_row, int temp_column, int player){

}
void  print_shotable(int player){
    if(player == 1) {
        printf("  ");

        for (int x = 1; x <= map_size; x++)
            printf("  %.2d", x);

        printf("\n   ");

        for (int i = 1; i <= 4 * map_size; i++)
            printf("_");

        printf("\n");
        for (int i = 0; i < map_size; i++) {
            printf("%.2d |", 1 + i);
            for (int j = 0; j < map_size; j++) {
                printf(" ");
                printf("%c", board2[i][j] != 'S' ? board2[i][j] : ' ');
                printf(" |");
            }

            printf("\n   |");

            for (int k = 0; k < map_size; k++)
                printf("___|");

            printf("\n");
        }
    }
    else{
        printf("  ");

        for (int x = 1; x <= map_size; x++)
            printf("  %.2d", x);

        printf("\n   ");

        for (int i = 1; i <= 4 * map_size; i++)
            printf("_");

        printf("\n");
        for (int i = 0; i < map_size; i++) {
            printf("%.2d |", 1 + i);
            for (int j = 0; j < map_size; j++) {
                printf(" ");
                printf("%c", board1[i][j] != 'S' ? board1[i][j] : ' ');
                printf(" |");
            }
            printf("\n   |");

            for (int k = 0; k < map_size; k++)
                printf("___|");

            printf("\n");
        }
    }
}

int main(void){
    //settings
    max_size = 5;
    arr_size[1] = 4;
    arr_size[2] = 3;
    arr_size[3] = 2;
    arr_size[5] = 1;
    int player1 = 1, player2 = 2;
    get_inputs(2);
//settings

    while(head1->arr1[0] != -1 || head2->arr2[0] != -1){
        int temp_row, temp_column;
        //player1 shots player2
        do {
            print_shotable(1);
            scanf("%d %d", &temp_row, &temp_column);

        }while(is_shotable(temp_row, temp_column, player1));
//player2 shots player1
        do {
            print_shotable(2);
            scanf("%d %d", &temp_row, &temp_column);



        }while(is_shotable(temp_row, temp_column, player2));
    }


    return 0;
}