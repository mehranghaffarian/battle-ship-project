# include <stdio.h>
# include <stdlib.h>
# include <windows.h>
# include <time.h>
# include <string.h>

int point1, point2, rocket1, rocket2, radar1, radar2, max_size = 5, ships_sum = 21, arr_size[30], map_size = 10, shot_point[30], count_loops2 = 0;
char name1[50], name2[50];

typedef struct ships_data1{
    int arr1[4];
    struct ships_data1* next;
}ships1;

//row and the column of the start and end of the ships

typedef struct ships_data2{
    int arr2[4];
    struct ships_data2* next;
}ships2;

typedef struct data{
    int score1, score2, turn, arr1[30][4], arr2[30][4], rocket1, rocket2, map_size, shot_point[30], radar1, radar2;
    char board1[15][15], board2[15][15], name1[50], name2[50];
}game;

char board1[20][20], board2[20][20];//board1 : the board where player1 puts his/her ships in//board2 : the board where player2 puts his/her ships in

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
void add_end1(ships1* head1, ships1* new){
    ships1* curr = head1;

    for(;curr->next != NULL;curr = curr->next);

    curr->next = new;
}
void add_end2(ships2* head2, ships2* new){
    ships2* curr = head2;

    for(;curr->next != NULL;curr = curr->next);

    curr->next = new;
}
int check_value1(ships1* curr, int arr[]){
    if((arr[0] == curr->arr1[0] && arr[2] ==curr->arr1[2] && arr[1] == curr->arr1[1] && arr[3] == curr->arr1[3]) || (arr[0] == curr->arr1[2] && arr[2] ==curr->arr1[0] && arr[1] == curr->arr1[3] && arr[3] == curr->arr1[1]))
        return 1;
    else
        return 0;
}
int check_value2(ships2* curr, int arr[]){
    if((arr[0] == curr->arr2[0] && arr[2] ==curr->arr2[2] && arr[1] == curr->arr2[1] && arr[3] == curr->arr2[3]) || (arr[0] == curr->arr2[2] && arr[2] ==curr->arr2[0] && arr[1] == curr->arr2[3] && arr[3] == curr->arr2[1]))
        return 1;
    else
        return 0;
}
void remove_ship1(ships1** head1, int arr[]){
    ships1* curr = *head1;

    if(check_value1(*head1, arr)){
        if((*head1)->next != NULL) {
            ships1 *hold = *head1;
            (*head1) = (*head1)->next;
            free(hold);
        }
        else
            (*head1) = NULL;
    }
    else{
        while(! check_value1(curr->next, arr))
            curr = curr->next;

        ships1* hold = curr->next;
        curr->next = curr->next->next;
        free(hold);
    }
}
void remove_ship2(ships2** head2, int arr[]){
    ships2* curr = *head2;

    if(check_value2(*head2, arr)){
        if((*head2)->next != NULL) {
            ships2* hold = *head2;
            (*head2) = (*head2)->next;
            free(hold);
        }
        else
            (*head2) = NULL;
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
//S:a ship is in this place but it has not been a exploded, C:complete explosion has happened, E:explosion has happened, T:this place has been a target but did not have any ship
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

    if(player == 1 || player == 3) {
        if (array[0] >= map_size || array[1] >= map_size || array[0] <= -1 || array[1] <= -1)
            is_locatable = 0;

        if(array[0] - 1 >= 0 && array[1] - 1 >= 0 ) {
            if (board1[array[0] - 1][array[1] - 1] == 'S')
                is_locatable = 0;
        }

        if(array[0] - 1 >= 0) {
            if( board1[array[0] - 1][array[1]] == 'S')
                is_locatable = 0;
        }

        if(array[0] - 1 >= 0 && array[1] + 1 < map_size) {
            if (board1[array[0] - 1][array[1] + 1] == 'S')
                is_locatable = 0;
        }

        if(array[1] + 1 < map_size) {
            if (board1[array[0]][array[1] + 1] == 'S')
                is_locatable = 0;
        }

        if(board1[array[0]][array[1]] == 'S')
            is_locatable = 0;

        if(array[1] - 1 >= 0 ) {
            if (board1[array[0]][array[1] - 1] == 'S')
                is_locatable = 0;
        }

        if(array[0] + 1 < map_size && array[1] - 1 >= 0) {
            if ( board1[array[0] + 1][array[1] - 1] == 'S')
                is_locatable = 0;
        }

        if(array[0] + 1 < map_size) {
            if (board1[array[0] + 1][array[1]] == 'S')
                is_locatable = 0;
        }

        if(array[0] + 1 < map_size && array[1] + 1 < map_size) {
            if (board1[array[0] + 1][array[1] + 1] == 'S')
                is_locatable = 0;
        }

    }
    else{
        if (array[0] >= map_size || array[1] >= map_size || array[0] <= -1 || array[1] <= -1)
            is_locatable = 0;

        if(array[0] - 1 >= 0 && array[1] - 1 >= 0 ) {
            if (board2[array[0] - 1][array[1] - 1] == 'S')
                is_locatable = 0;
        }

        if(array[0] - 1 >= 0) {
            if( board2[array[0] - 1][array[1]] == 'S')
                is_locatable = 0;
        }

        if(array[0] - 1 >= 0 && array[1] + 1 < map_size) {
            if (board2[array[0] - 1][array[1] + 1] == 'S')
                is_locatable = 0;
        }

        if(array[1] + 1 < map_size) {
            if (board2[array[0]][array[1] + 1] == 'S')
                is_locatable = 0;
        }

        if(board2[array[0]][array[1]] == 'S')
            is_locatable = 0;

        if(array[1] - 1 >= 0 ) {
            if (  board2[array[0]][array[1] - 1] == 'S')
                is_locatable = 0;
        }

        if(array[0] + 1 < map_size && array[1] - 1 >= 0) {
            if ( board2[array[0] + 1][array[1] - 1] == 'S')
                is_locatable = 0;
        }

        if(array[0] + 1 < map_size) {
            if (board2[array[0] + 1][array[1]] == 'S')
                is_locatable = 0;
        }

        if(array[0] + 1 < map_size && array[1] + 1 < map_size) {
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
    }
}
void get_inputs(ships1* head1, ships2* head2, int player){
    if(player == 1) {
        int counter = 0,  arr[4], is_locatable, temp_array[2], arr_size1[30] = {0};//arr_size[size of ship][number of this ship]

        for(int i = 0;i < 30;i++)
            arr_size1[i] = arr_size[i];

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
                    if (counter == 0) {
                        for(int i = 0;i < 4;i++)
                            head1->arr1[i] = arr[i];
                    }

                    else
                        add_end1(head1, new_ship1(arr));

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
    if(player == 2){
        int counter = 0,  arr[4], is_locatable, temp_array[2], arr_size2[30];//arr_size[size of ship][number of this ship]

        for(int i = 0;i < 30;i++)
            arr_size2[i] = arr_size[i];

        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nplayer%d please enter the row and column of the ships(ships end and first). if it is a ship with length of 1 please enter its start row and column twice\n",
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
                    if (counter == 0) {
                        for(int i = 0;i < 4;i++)
                            head2->arr2[i] = arr[i];
                    }

                    else
                        add_end2(head2, new_ship2(arr));

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
    srand(time(0));

    if(player == 3) {//auto for player1
        int counter = 0, arr[4], check, limit, count_loops = 0, is_locatable, temp_array[2], temp_array1[2], temp_array2[2], temp_array3[2], temp_array4[2], k, arr_size1[30] = {0}, locatable[map_size][map_size];//arr_size[size of ship][number of this ship]

        for(int i = 0;i < 30;i++)
            arr_size1[i] = arr_size[i];

        count_loops2 = 0;
        count_loops = 0;

        while (counter < ships_sum){
            count_loops2++;
            count_loops++;
            is_locatable = 1;

            if(count_loops2 > 15000){
                printf("\nthe automat map is not possible\n");

                counter += ships_sum;

                while(head1->next != NULL){//reseting the inputs to avoid endless loops
                    ships1* curr = head1;

                    for(;curr->next->next != NULL;curr = curr->next);

                    ships1* hold = curr->next;
                    curr->next = NULL;
                    free(hold);
                }

                for(int i = 0;i < map_size;i++){
                    for(int j = 0;j < map_size;j++)
                        board1[i][j] = '\0';
                }

                for(int i = 0;i < 30;i++)
                    arr_size1[i] = arr_size[i];

                counter = 0;
                count_loops = 0;
            }

            if(counter  > ships_sum / 2)
            { //to avoid impossible design
                k = 0;

                for(int i = 0;i < map_size;i++) {
                    for(int j = 0;j < map_size;j++)
                        locatable[i][j] = 0;
                }

                for(int i = 0;i < map_size;i++) {
                    for (int j = 0; j < map_size; j++) {
                        temp_array[0] = i;
                        temp_array[1] = j;
                        locatable[i][j] = is_really_locatable(temp_array, 1);

                        if(is_really_locatable(temp_array, 1))
                            k++;
                    }
                }
                check =  1 + rand() % k;

                for(int i = 0;i < map_size;i++){
                    for(int j = 0;j < map_size;j++){
                        if(locatable[i][j] == 1)
                            check--;

                        if(check == 0){
                            arr[0] = i;
                            arr[1] = j;
                        }
                    }
                }
                k = 0;

                for(int i = 0;i < 30;i++)
                    if(arr_size1[i] != 0 && i > k)
                        k = i;

                check = 1 + count_loops % 4;

                temp_array1[0] = arr[0] - k + 1;
                temp_array1[1] = arr[1];

                temp_array2[0] = arr[0];
                temp_array2[1] = arr[1] + k - 1;

                temp_array3[0] = arr[0] + k - 1;
                temp_array3[1] = arr[1];

                temp_array4[0] = arr[0];
                temp_array4[1] = arr[1] - k + 1;

                if(check == 1){
                    arr[2] = temp_array1[0];
                    arr[3] = temp_array1[1];
                }
                else if(check == 2){
                    arr[2] = temp_array2[0];
                    arr[3] = temp_array2[1];
                }
                else if(check == 3){
                    arr[2] = temp_array3[0];
                    arr[3] = temp_array3[1];
                }
                else if(check == 4){
                    arr[2] = temp_array4[0];
                    arr[3] = temp_array4[1];
                }
            }
            else {
                arr[2] = rand() % map_size;
                arr[3] = rand() % map_size;

                if (counter % 2) {
                    arr[0] = arr[2];
                    arr[1] = (arr[2] * arr[3]) % map_size;
                }
                else {
                    arr[1] = arr[3];
                    arr[0] = (arr[2] * arr[3]) % map_size;
                }
            }
            if(count_loops > 5000){
                while(head1->next != NULL){//reseting the inputs to avoid endless loops
                    ships1* curr = head1;

                    for(;curr->next->next != NULL;curr = curr->next);

                    ships1* hold = curr->next;
                    curr->next = NULL;
                    free(hold);
                }

                for(int i = 0;i < map_size;i++){
                    for(int j = 0;j < map_size;j++)
                        board1[i][j] = '\0';
                }

                for(int i = 0;i < 30;i++)
                    arr_size1[i] = arr_size[i];

                counter = 0;
                count_loops = 0;
            }

            int x1 = arr[0] < arr[2] ? arr[0] : arr[2], x2 = arr[2] > arr[0] ? arr[2] : arr[0];
            int y1 = arr[1] < arr[3] ? arr[1] : arr[3], y2 = arr[1] < arr[3] ? arr[3] : arr[1];

            if(arr[0] != arr[2] && arr[1] != arr[3])
                is_locatable = 0;

            if(arr[0] == arr[2]){
                for(int i = y1;i <= y2;i++) {
                    temp_array[0] = arr[0];
                    temp_array[1] = i;
                    if (is_really_locatable(temp_array, 1) == 0)
                        is_locatable = 0;
                }
            }
            if(arr[1] == arr[3]){
                for(int i = x1;i <= x2;i++) {
                    temp_array[1] = arr[1];
                    temp_array[0] = i;
                    if (is_really_locatable(temp_array, 1) == 0)
                        is_locatable = 0;
                }
            }

            if(is_locatable == 1 && arr_size1[find_length(arr)] != 0 && find_length(arr) <= max_size) {
                if (counter + find_length(arr) <= ships_sum) {
                    if (counter == 0) {
                        for(int i = 0;i < 4;i++)
                            head1->arr1[i] = arr[i];
                    }

                    else
                        add_end1(head1, new_ship1(arr));

                    fill_board1(arr);

                    counter += find_length(arr);
                    arr_size1[find_length(arr)]--;
                }
                else
                    is_locatable = 0;
            }
            else
                is_locatable = 0;
        }
    }
    if(player == 4) {//auto for player1
        int counter = 0, arr[4], check, limit, count_loops = 0, is_locatable, temp_array[2], temp_array1[2], temp_array2[2], temp_array3[2], temp_array4[2], k, arr_size2[30] = {0}, locatable[map_size][map_size];//arr_size[size of ship][number of this ship]

        for(int i = 0;i < 30;i++)
            arr_size2[i] = arr_size[i];

        count_loops2 = 0;
        count_loops = 0;

        while (counter < ships_sum){
            count_loops2++;
            count_loops++;
            is_locatable = 1;

            if(count_loops2 > 15000){
                printf("\nthe automat map is not possible\n");

                counter += ships_sum;

                while(head2->next != NULL){//reseting the inputs to avoid endless loops
                    ships2* curr = head2;

                    for(;curr->next->next != NULL;curr = curr->next);

                    ships2* hold = curr->next;
                    curr->next = NULL;
                    free(hold);
                }

                for(int i = 0;i < map_size;i++){
                    for(int j = 0;j < map_size;j++)
                        board2[i][j] = '\0';
                }

                for(int i = 0;i < 30;i++)
                    arr_size2[i] = arr_size[i];

                counter = 0;
                count_loops = 0;
            }

            if(counter  > ships_sum / 2)
            { //to avoid impossible design
                k = 0;

                for(int i = 0;i < map_size;i++) {
                    for(int j = 0;j < map_size;j++)
                        locatable[i][j] = 0;
                }

                for(int i = 0;i < map_size;i++) {
                    for (int j = 0; j < map_size; j++) {
                        temp_array[0] = i;
                        temp_array[1] = j;
                        locatable[i][j] = is_really_locatable(temp_array, 2);

                        if(is_really_locatable(temp_array, 2))
                            k++;
                    }
                }
                check =  1 + rand() % k;

                for(int i = 0;i < map_size;i++){
                    for(int j = 0;j < map_size;j++){
                        if(locatable[i][j] == 1)
                            check--;

                        if(check == 0){
                            arr[0] = i;
                            arr[1] = j;
                        }
                    }
                }
                k = 0;

                for(int i = 0;i < 30;i++)
                    if(arr_size2[i] != 0 && i > k)
                        k = i;

                check = 1 + count_loops % 4;

                temp_array1[0] = arr[0] - k + 1;
                temp_array1[1] = arr[1];

                temp_array2[0] = arr[0];
                temp_array2[1] = arr[1] + k - 1;

                temp_array3[0] = arr[0] + k - 1;
                temp_array3[1] = arr[1];

                temp_array4[0] = arr[0];
                temp_array4[1] = arr[1] - k + 1;

                if(check == 1){
                    arr[2] = temp_array1[0];
                    arr[3] = temp_array1[1];
                }
                else if(check == 2){
                    arr[2] = temp_array2[0];
                    arr[3] = temp_array2[1];
                }
                else if(check == 3){
                    arr[2] = temp_array3[0];
                    arr[3] = temp_array3[1];
                }
                else if(check == 4){
                    arr[2] = temp_array4[0];
                    arr[3] = temp_array4[1];
                }
            }
            else {
                arr[2] = rand() % map_size;
                arr[3] = rand() % map_size;

                if (counter % 2) {
                    arr[0] = arr[2];
                    arr[1] = (arr[2] * arr[3]) % map_size;
                } else {
                    arr[1] = arr[3];
                    arr[0] = (arr[2] * arr[3]) % map_size;
                }
            }
            if(count_loops > 5000){
                while(head2->next != NULL){//reseting the inputs to avoid endless loops
                    ships2* curr = head2;

                    for(;curr->next->next != NULL;curr = curr->next);

                    ships2* hold = curr->next;
                    curr->next = NULL;
                    free(hold);
                }

                for(int i = 0;i < map_size;i++){
                    for(int j = 0;j < map_size;j++)
                        board2[i][j] = '\0';
                }

                for(int i = 0;i < 30;i++)
                    arr_size2[i] = arr_size[i];

                counter = 0;
                count_loops = 0;
            }

            int x1 = arr[0] < arr[2] ? arr[0] : arr[2], x2 = arr[2] > arr[0] ? arr[2] : arr[0];
            int y1 = arr[1] < arr[3] ? arr[1] : arr[3], y2 = arr[1] < arr[3] ? arr[3] : arr[1];

            if(arr[0] != arr[2] && arr[1] != arr[3])
                is_locatable = 0;

            if(arr[0] == arr[2]){
                for(int i = y1;i <= y2;i++) {
                    temp_array[0] = arr[0];
                    temp_array[1] = i;
                    if (is_really_locatable(temp_array, 2) == 0)
                        is_locatable = 0;
                }
            }
            if(arr[1] == arr[3]){
                for(int i = x1;i <= x2;i++) {
                    temp_array[1] = arr[1];
                    temp_array[0] = i;
                    if (is_really_locatable(temp_array, 2) == 0)
                        is_locatable = 0;
                }
            }

            if(is_locatable == 1 && arr_size2[find_length(arr)] != 0 && find_length(arr) <= max_size) {
                if (counter + find_length(arr) <= ships_sum) {
                    if (counter == 0) {
                        for(int i = 0;i < 4;i++)
                            head2->arr2[i] = arr[i];
                    }

                    else
                        add_end2(head2, new_ship2(arr));

                    fill_board2(arr);

                    counter += find_length(arr);
                    arr_size2[find_length(arr)]--;
                }
                else
                    is_locatable = 0;
            }
            else
                is_locatable = 0;
        }
    }
    if(player == 5){
        FILE* bot = fopen("get input example for bot.txt", "r+");

        int counter = 0, arr[4];

        while(counter != 21){
            fscanf(bot, "%d %d %d %d", &arr[0], &arr[1], &arr[2], &arr[3]);

            for(int i = 0;i < 4;i++)
                arr[i]--;

            if (counter == 0) {
                for (int i = 0; i < 4; i++)
                    head2->arr2[i] = arr[i];
            }
            else
                add_end2(head2, new_ship2(arr));

            fill_board2(arr);

            counter += find_length(arr);
        }
        fclose(bot);
    }
}
int is_shotable(int temp_row, int temp_column, int player){
    int is_shotable = 1;

    if(player == 1){

        if(temp_row >= map_size || temp_row < 0 || temp_column >= map_size || temp_column < 0)
            is_shotable = 0;

        if(board2[temp_row][temp_column] == 'C' || board2[temp_row][temp_column] == 'E' || board2[temp_row][temp_column] == 'T')
            is_shotable = 0;

        return is_shotable;
    }
    else if(player == 2){

        if(temp_row >= map_size || temp_row < 0 || temp_column >= map_size || temp_column < 0)
            is_shotable = 0;

        if(board1[temp_row][temp_column] == 'C' || board1[temp_row][temp_column] == 'E' || board1[temp_row][temp_column] == 'T')
            is_shotable = 0;

        return is_shotable;
    }
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
void fill_around(int row, int column, int player){//board of player must be filled
    if(player == 1){

        for(int i = row - 1;i <= row + 1;i++) {
            if (i > -1 && i < map_size) {
                for (int j = column - 1; j <= column + 1; j++) {
                    if (j < map_size && j > -1)
                        if (board1[i][j] != 'E' && board1[i][j] != 'C')
                            board1[i][j] = 'T';
                }
            }
        }
    }
    if(player == 2){
        for(int i = row - 1;i <= row + 1;i++) {
            if (i > -1 && i < map_size) {
                for (int j = column - 1; j <= column + 1; j++) {
                    if (j < map_size && j > -1)
                        if (board2[i][j] != 'E' && board2[i][j] != 'C')
                            board2[i][j] = 'T';
                }
            }
        }
    }
}
int shot_it(ships1** head1, ships2** head2, int row, int column, int player){
    int status = 0;//0:the target did not have any ship -> next player, 1:the target had ship -> player prize

    //player 1 shots player 2
    if(player == 1){
        if(board2[row][column] == 'S'){
            status = 1;
            point1++;
            board2[row][column] = 'E';

            //if any surrounding place has been exploded we have to judge whether it was complete explosion or a simple explosion
            ships2* curr = *head2;

            while(curr != NULL){
                int is_colmplete_explosion = 1;

                if(((curr->arr2[0] <= row && row <= curr->arr2[2]) || (curr->arr2[2] <= row && row <= curr->arr2[0]))  &&  ((curr->arr2[1] <= column && curr->arr2[3] >= column) || (curr->arr2[3] <= column && curr->arr2[1] >= column))){
                    int row1 = curr->arr2[0] < curr->arr2[2] ? curr->arr2[0] : curr->arr2[2], row2 = curr->arr2[0] > curr->arr2[2] ? curr->arr2[0] : curr->arr2[2];
                    int column1 = curr->arr2[1] < curr->arr2[3] ? curr->arr2[1] : curr->arr2[3], column2 = curr->arr2[1] > curr->arr2[3] ? curr->arr2[1] : curr->arr2[3];

                    for(int i = row1;i <= row2;i++){
                        for(int j = column1;j <= column2;j++){
                            if(board2[i][j] != 'E')
                                is_colmplete_explosion = 0;
                        }
                    }
                    if(is_colmplete_explosion == 1)
                    {
                        point1 += shot_point[find_length(curr->arr2)];
                        remove_ship2(head2, curr->arr2);

                        for(int i = row1;i <= row2;i++){
                            for(int j = column1;j <= column2;j++){
                                board2[i][j] = 'C';
                                fill_around(i, j, 2);
                            }
                        }
                        printf("\nNICE SHOT!! COMPLETE EXPLOSION\n");
                    }
                }
                curr = curr->next;
            }
        }
        else{
            status = 0;
            board2[row][column] = 'T';
            printf("\nOops?! NO ship\n");
        }
    }
    if(player == 2 || player == 3){
        if(board1[row][column] == 'S'){
            status = 1;
            point2++;
            board1[row][column] = 'E';

            //if any surrounding place has been exploded we have to judge whether it was complete explosion or a simple explosion
            ships1* curr = *head1;

            while(curr != NULL){
                int is_colmplete_explosion = 1;

                if(((curr->arr1[0] <= row && row <= curr->arr1[2]) || (curr->arr1[2] <= row && row <= curr->arr1[0]))  &&  ((curr->arr1[1] <= column && curr->arr1[3] >= column) || (curr->arr1[3] <= column && curr->arr1[1] >= column))){
                    int row1 = curr->arr1[0] < curr->arr1[2] ? curr->arr1[0] : curr->arr1[2], row2 = curr->arr1[0] > curr->arr1[2] ? curr->arr1[0] : curr->arr1[2];
                    int column1 = curr->arr1[1] < curr->arr1[3] ? curr->arr1[1] : curr->arr1[3], column2 = curr->arr1[1] > curr->arr1[3] ? curr->arr1[1] : curr->arr1[3];

                    for(int i = row1;i <= row2;i++){
                        for(int j = column1;j <= column2;j++){
                            if(board1[i][j] != 'E')
                                is_colmplete_explosion = 0;
                        }
                    }
                    if(is_colmplete_explosion == 1)
                    {
                        point2 += shot_point[find_length(curr->arr1)];
                        remove_ship1(head1, curr->arr1);

                        for(int i = row1;i <= row2;i++){
                            for(int j = column1;j <= column2;j++){
                                board1[i][j] = 'C';
                                fill_around(i, j, 1);
                            }
                        }
                        if(player == 2)
                            printf("\nNICE SHOT!! COMPLETE EXPLOSION\n");
                    }
                }
                curr = curr->next;
            }
        }
        else if(board1[row][column] != 'S'){
            status = 0;
            board1[row][column] = 'T';
            if(player == 2)
                printf("\nOops?! NO ship\n");
        }
        else
            status = 0;
    }
    return status;
}
void save_it(ships1** head11, ships2** head22, int turn){
    FILE* games = fopen("games.bin", "ab+");

    game curr;
    strcpy(curr.name1, name1);
    strcpy(curr.name2, name2);

    for(int i = 0;i < map_size;i++){
        for(int j = 0;j < map_size;j++)
        {
            curr.board1[i][j] = board1[i][j];
            curr.board2[i][j] = board2[i][j];
        }
    }

    for(int i = 0;i < 30;i++)
        curr.shot_point[i] = shot_point[i];

    curr.score1 = point1;
    curr.score2 = point2;

    curr.map_size = map_size;

    curr.rocket1 = rocket1;
    curr.rocket2 = rocket2;

    curr.radar1 = radar1;
    curr.radar2 = radar2;

    curr.turn = turn;

    ships1* curr1 = (*head11);
    ships2* curr2 = (*head22);

    for(int i = 0;i < 30;i++){
        if(curr1 != NULL)
        {
            curr.arr1[i][0] = curr1->arr1[0];
            curr.arr1[i][1] = curr1->arr1[1];
            curr.arr1[i][2] = curr1->arr1[2];
            curr.arr1[i][3] = curr1->arr1[3];
        }
        else
        {
            curr.arr1[i][0] = -10;
            curr.arr1[i][1] = -10;
            curr.arr1[i][2] = -10;
            curr.arr1[i][3] = -10;
        }
        if(curr2 != NULL)
        {
            curr.arr2[i][0] = curr2->arr2[0];
            curr.arr2[i][1] = curr2->arr2[1];
            curr.arr2[i][2] = curr2->arr2[2];
            curr.arr2[i][3] = curr2->arr2[3];
        }
        else
        {
            curr.arr2[i][0] = -10;
            curr.arr2[i][1] = -10;
            curr.arr2[i][2] = -10;
            curr.arr2[i][3] = -10;
        }
        if(curr1 != NULL)
        curr1 = curr1->next;

        if(curr2 != NULL)
        curr2 = curr2->next;
    }
    fwrite(&curr, sizeof(curr), 1, games);
    fclose(games);

    int num;
    FILE* number = fopen("games_number.bin", "rb+");
    fread(&num, 4, 1, number);
    fclose(number);
    number = fopen("games_number.bin", "wb+");
    num++;
    fwrite(&num, 4, 1, number);
    fclose(number);

    printf("\nthe game is saved properly\n");
}
void mutual_play(ships1** head11, ships2** head22, int save){
    ships1* head1 = (*head11);
    ships2* head2 = (*head22);
    int a;

    printf("\n\n\n\n\n\n\n\n\n\n\n\n lets begin the game. player1 will start the game. every turn you have to choose a correct location to attack. if you shot any ship you will get points and be able to shot the other player again. you have to enter the row and the column of your location in this way:row column. if you want to use a rocket enter:-2 -2.then you can decide whether to attack your enemy in a vertical or horizontal way. enter:v column or h row. you can use rocket once(it would cost 100 point). if you want to use the radar enter:-3 -3. then you have to enter the central point location of the 3*3 area(it would cost 75 points and can be used only ance). if you want to save the game enter:-1 -1\nsimple shot:row column\nsave:-1 -1\nrocket:-2 -2  -> then:h row  or:v column\nradar:-3 -3 then->row column\n");

    while(head1 != NULL && head2 != NULL && save != -1){
        int temp_row, temp_column, temp, turn;

        //player1 shots player2
        do {
            if(head1 != NULL && head2 != NULL && (save == 1 || save == 0)) {
                print_shotable(1);
                printf("%s:%d  --  %s:%d\n", name1, point1, name2, point2);
                printf("%s chose your target:\n", name1);
                scanf("%d %d", &temp_row, &temp_column);
                save = 0;
                if(temp_column == -1 && temp_row == -1) {
                    save_it(head11, head22, 1);
                    save = -1;
                }
                else if((temp_column == -3 && temp_row == -3)){
                    printf("%s enter the central point:\n", name1);
                    scanf("%d %d", &temp_row, &temp_column);

                    if(radar1 != 0 || point1 < 75){
                        printf("\nyou can not use radar.\n");

                        turn = 1;
                    }
                    else if(temp_row < 2 || temp_row > map_size - 1 || temp_column < 2 || temp_column > map_size - 1){
                        printf("\nyour inputs are not acceptable\n");

                        turn = 1;
                    }
                    else{
                        int row  = temp_row - 1, column = temp_column - 1;

                        for(int i = row - 1;i <= row + 1;i++){
                            printf("\n");
                            for(int j = column - 1;j <= column + 1;j++)
                                printf("%c", board2[i][j] == '\0' ? '_' : board2[i][j]);
                        }
                        printf("\n");

                        a = 1;
                        turn = 0;

                        radar1++;
                        point1 -= 75;
                    }
                }
                else if (temp_column == -2 && temp_row == -2) {
                    if(rocket1 > 0 || point1 < 100) {
                        printf("\nyou can not use rocket\n");
                        a = 0;
                        turn = 1;
                    }
                    else{
                        char rocket;
                        int i = 0;

                        a = 1;
                        turn = 0;

                        printf("%s choose your method to attack your enemy:\n", name1);
                        fflush(stdin);
                        scanf("%c %d", &rocket, &temp);
                        temp--;

                        if(temp < 0 || temp >= map_size || (rocket != 'v' && rocket != 'h')){
                            i = map_size;
                            turn = 1;
                            printf("\nyour inputs are not valid. choose another way.\n");
                            point1 += 100;
                            rocket1--;
                        }
                        if(rocket == 'v'){
                            while(turn == 0 && i < map_size){
                                if(board2[i][temp] != 'E' && board2[i][temp] != 'C' && board2[i][temp] != 'T') {
                                    turn = shot_it(&head1, &head2, i, temp, 1);
                                }
                                i++;
                            }
                        }
                        else if(rocket == 'h'){
                            while(turn == 0 && i < 10){
                                if(board2[temp][i] != 'E' && board2[temp][i] != 'C' && board2[temp][i] != 'T') {
                                    turn = shot_it(&head1, &head2, temp, i, 1);
                                }
                                i++;
                            }
                        }
                            point1 -= 100;
                            rocket1++;
                    }
                }
                else if(temp_column != -2 && temp_row != -1){
                a = is_shotable(temp_row - 1, temp_column - 1, 1);

                if(a)
                    turn = shot_it(&head1, &head2, temp_row - 1, temp_column - 1, 1);

                else
                    printf("\ntarget is not acceptable\n");
                }
            }
        }while((a == 0 || turn) && (save == 1 || save == 0) && head1 != NULL && head2 != NULL);

        //player2 shots player1
        do {
            if(head1 != NULL && head2 != NULL && (save == 2 || save == 0)) {
                print_shotable(2);
                printf("%s:%d  --  %s:%d\n", name1, point1, name2, point2);
                printf("%s chose your target:\n", name2);
                fflush(stdin);
                scanf("%d %d", &temp_row, &temp_column);
                save = 0;
                if(temp_column == -1 && temp_row == -1) {
                    save_it(head11, head22, 2);
                    save = -1;
                }
                else if((temp_column == -3 && temp_row == -3)){
                    printf("%s enter the central point:\n", name1);
                    scanf("%d %d", &temp_row, &temp_column);

                    if(radar2 != 0 || point2 < 75){
                        printf("\nyou can not use radar.\n");

                        turn = 1;
                    }
                    else if(temp_row < 2 || temp_row > map_size - 1 || temp_column < 2 || temp_column > map_size - 1){
                        printf("\nyour inputs are not acceptable\n");

                        turn = 1;
                    }
                    else{
                        int row  = temp_row - 1, column = temp_column - 1;

                        for(int i = row - 1;i <= row + 1;i++){
                            printf("\n");
                            for(int j = column - 1;j <= column + 1;j++)
                                printf("%c", board1[i][j] == '\0' ? '_' : board1[i][j]);
                        }
                        printf("\n");

                        a = 1;
                        turn = 0;

                        radar2++;
                        point2 -= 75;
                    }
                }
                else if (temp_column == -2 && temp_row == -2) {
                    if(rocket2 > 0 || point2 < 100) {
                        printf("\nyou can not use rocket\n");
                        a = 0;
                        turn = 1;
                    }
                    else{
                        char rocket;
                        int i = 0;

                        a = 1;
                        turn = 0;

                        printf("\n%s choose your method to attack your enemy\n", name2);
                        fflush(stdin);
                        scanf("%c %d", &rocket, &temp);
                        temp--;

                        if(temp < 1 || temp > map_size || (rocket != 'v' && rocket != 'h')){
                            i = map_size;
                            turn = 1;
                            rocket2--;point2 += 100;

                            printf("\nyour inputs are not valid. choose another way.\n");
                        }
                        if(rocket == 'v'){
                            while(turn == 0 && i < map_size){
                                if(board1[i][temp] != 'E' && board1[i][temp] != 'C' && board1[i][temp] != 'T') {
                                    turn = shot_it(&head1, &head2, i, temp, 2);
                                }
                                i++;
                            }
                        }
                        else if(rocket == 'h'){
                            while(turn == 0 && i < 10){
                                if(board1[temp][i] != 'E' && board1[temp][i] != 'C' && board1[temp][i] != 'T') {
                                    turn = shot_it(&head1, &head2, temp, i, 2);
                                }
                                i++;
                            }
                        }
                        point2 -= 100;
                        rocket2++;
                    }
                }
                else if(temp_column != -2 && temp_row != -1){
                    a = is_shotable(temp_row - 1, temp_column - 1, 2);

                    if(a)
                        turn = shot_it(&head1, &head2, temp_row - 1, temp_column - 1, 2);

                    else
                        printf("\ntarget is not acceptable\n");
                }
            }
        }while((a == 0 || turn) && (save == 2 || save == 0) && head1 != NULL && head2 != NULL);
    }
    if(save != -1){
        printf("\nthe %s board is as bellow\n", name1);
        print(1);
        printf("\nthe %s board is as bellow\n", name2);
        print(2);

        printf("\n\n%s:%d  --  %s:%d", name1, point1, name2, point2);
        printf("\nthe winner is ");

        if (head1 == NULL) {
            point1 /= 2;

            printf("%s\n", name2);
        }
        else {
            point2 /= 2;

            printf("%s\n", name1);
        }
        printf("\nthe information is shown above\n\n*_* THE GAME IS OVER *_*\n");

        FILE *users = fopen("users.txt", "a+");
        fprintf(users, "%s %d\n", name1, point1);
        fprintf(users, "%s %d\n", name2, point2);
        fclose(users);

        int num;
        FILE *number = fopen("number.txt", "r+");
        fscanf(number, "%d", &num);
        fclose(number);

        number = fopen("number.txt", "w+");
        fprintf(number, "%d", num + 2);
        fclose(number);
    }
        //deleting the current information for another game if the user wants
        for (int i = 0; i < map_size; i++) {
            for (int j = 0; j < map_size; j++) {
                board1[i][j] = '\0';
                board2[i][j] = '\0';
            }
        }
        int temp_arr[4];

        for (int i = 0; i < 30; i++) {
            if (head1 != NULL) {
                temp_arr[0] = head1->arr1[0];
                temp_arr[1] = head1->arr1[1];
                temp_arr[2] = head1->arr1[2];
                temp_arr[3] = head1->arr1[3];
                remove_ship1(&head1, temp_arr);
            }
            if (head2 != NULL) {
                temp_arr[0] = head2->arr2[0];
                temp_arr[1] = head2->arr2[1];
                temp_arr[2] = head2->arr2[2];
                temp_arr[3] = head2->arr2[3];
                remove_ship2(&head2, temp_arr);
            }
        }
        //preparing head1 and head2 for another game
    int arr[4] = {-5, -5, -5, -5};
    ships1* head3 = new_ship1(arr);
    ships2* head4 = new_ship2(arr);
    head11 = &head3;
    head22 = &head4;
}
void bot_play(ships1** head11, ships2** head22, int save){
    ships1* head1 = (*head11);
    ships2* head2 = (*head22);
    int a;
    srand(time(0));

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n lets begin the game. player1 will start the game. every turn you have to choose a correct location to attack. if you   shot any ship you will get points and be able to shot the other player again. you have to enter the row and the column of your location in this way:row column. if you want to use a rocket enter:-2 -2.then you can decide whether to attack your enemy in a vertical or horizontal way. enter:v column or h row. you can use rocket once(it would cost 100 point). if you got any problem enter your target column again and a junk number. if you want to use the radar enter:-3 -3. then you have to enter the central point location of the 3*3 area(it would cost 75 points and can be used only ance). if you want to save the game enter:-1 -1\nsimple shot:row column\nsave:-1 -1\nrocket:-2 -2  -> then:h row  or:v column\nradar:-3 -3  then->central:row column\n");

    while(head1 != NULL && head2 != NULL && save != -1){
        int temp_row, temp_column, temp, turn;

        //player1 shots player2
        do {
            if(head1 != NULL && head2 != NULL && (save == 0 || save == 3)) {
                print_shotable(1);
                printf("%s:%d  --  %s:%d\n", name1, point1, name2, point2);
                printf("%s chose your target:\n", name1);
                fflush(stdin);
                scanf("%d %d", &temp_row, &temp_column);
                save = 0;

                if(temp_column == -1 && temp_row == -1) {
                    save_it(head11, head22, 3);
                    save = -1;
                }
                else if((temp_column == -3 && temp_row == -3)){
                    printf("%s enter the central point:\n", name1);
                    scanf("%d %d", &temp_row, &temp_column);

                    if(radar1 != 0 || point1 < 75){
                        printf("\nyou can not use radar.\n");

                        turn = 1;
                    }
                    else if(temp_row < 2 || temp_row > map_size - 1 || temp_column < 2 || temp_column > map_size - 1){
                        printf("\nyour inputs are not acceptable\n");

                        turn = 1;
                    }
                    else{
                        int row  = temp_row - 1, column = temp_column - 1;

                        for(int i = row - 1;i <= row + 1;i++){
                            printf("\n");
                            for(int j = column - 1;j <= column + 1;j++)
                                printf("%c", board2[i][j] == '\0' ? '_' : board2[i][j]);
                        }
                        printf("\n");

                        a = 1;
                        turn = 0;

                        radar1++;
                        point1 -= 75;
                    }
                }
                else if (temp_column == -2 && temp_row == -2) {
                    if(rocket1 > 0 || point1 < 100) {
                        printf("\nyou can not use rocket\n");

                        a = 0;
                        turn = 1;
                    }
                    else{
                        char rocket;
                        int i = 0;

                        a = 1;
                        turn = 0;

                        printf("%s choose your method to attack your enemy:\n", name1);
                        fflush(stdin);
                        scanf("%c %d", &rocket, &temp);
                        temp--;

                        if(temp < 0 || temp >= map_size || (rocket != 'v' && rocket != 'h')){
                            i = map_size;
                            turn = 1;
                            printf("\nyour inputs are not valid. choose another way.\n");
                            point1 += 100;
                            rocket1--;
                        }
                        if(rocket == 'v'){
                            while(turn == 0 && i < map_size){
                                if(board2[i][temp] != 'E' && board2[i][temp] != 'C' && board2[i][temp] != 'T') {
                                    turn = shot_it(&head1, &head2, i, temp, 1);
                                }
                                i++;
                            }
                        }
                        else if(rocket == 'h'){
                            while(turn == 0 && i < 10){
                                if(board2[temp][i] != 'E' && board2[temp][i] != 'C' && board2[temp][i] != 'T') {
                                    turn = shot_it(&head1, &head2, temp, i, 1);
                                }
                                i++;
                            }
                        }
                        point1 -= 100;
                        rocket1++;
                    }
                }
                else if(temp_column != -2 && temp_row != -1){
                    a = is_shotable(temp_row - 1, temp_column - 1, 1);

                    if(a)
                        turn = shot_it(&head1, &head2, temp_row - 1, temp_column - 1, 1);

                    else
                        printf("\ntarget is not acceptable\n");
                }
            }
        }while((a == 0 || turn) && (save == 3 || save == 0) && head1 != NULL && head2 != NULL);

        //bot shots player1
        do {
            if(head1 != NULL && head2 != NULL && save == 0) {
                int count = 0;
                int row = map_size, column = map_size;

                for(int i = 0;i < map_size;i++) {
                    for (int j = 0; j < map_size; j++) {
                        if (board1[i][j] == 'E' && (i <= row && j <= column)) {
                            row = i;
                            column = j;
                        }
                        if(board1[i][j] == 'E')
                            count++;
                    }
                }
                if(count == 0) {
                    do {
                        temp_column = rand() % map_size;
                        temp_row = rand() % map_size;
                        a = is_shotable(temp_row, temp_column, 2);
                    }while(!a);
                }
                else if(count == 1){
                    if(row - 1 > -1 && board1[row - 1][column] != 'T' && is_shotable(row - 1, column, 2)){
                            temp_column = column;
                            temp_row = row - 1;
                    }
                    else if(column + 1 < map_size && board1[row][column + 1] != 'T' && is_shotable(row, column + 1, 2)){
                        temp_column = column + 1;
                        temp_row = row;
                    }
                    else if(row + 1 < map_size && board1[row + 1][column] != 'T' && is_shotable(row + 1, column, 2)){
                        temp_column = column;
                        temp_row = row + 1;
                    }
                    else if(column - 1 > -1 && board1[row][column - 1] != 'T' && is_shotable(row, column - 1, 2)){
                            temp_column = column - 1;
                            temp_row = row;
                    }
                }
                else if(count > 1){
                    int i = 1, b = 1;

                    if(column + 1 < map_size && board1[row][column + 1] == 'E') {
                        if (column - 1 > -1 && is_shotable(row, column - 1, 2)) {
                            temp_row = row;
                            temp_column = column - 1;
                            b = 0;
                        }
                        while (b && column + i < map_size){
                            if(column + i < map_size && board1[row][column + i] == 'E'){
                                i++;
                                if(column + i < map_size && is_shotable(row, column + i, 2)){
                                    temp_row = row;
                                    temp_column = column + i;
                                    b = 0;
                                }
                            }
                            else
                                b = 0;
                        }
                    }
                    else if(row + 1 < map_size && board1[row + 1][column] == 'E'){
                        if(row - 1 > -1 && is_shotable(row - 1, column, 2)){
                            temp_column = column;
                            temp_row = row - 1;
                            b = 0;
                        }
                        while(b && row + i < map_size){
                            if(row + i < map_size && board1[row + i][column] == 'E'){
                                i++;
                                if(row + i < map_size && is_shotable(row + i, column, 2)){
                                    temp_column = column;
                                    temp_row = row + i;
                                    b = 0;
                                }
                            }
                            else
                                b = 0;
                        }
                    }
                }
                    turn = shot_it(&head1, &head2, temp_row, temp_column, 3);
                    print_shotable(2);
                    printf("\nthe bot shot is shown above\n");
            }
        }while(turn && save == 0 && head1 != NULL && head2 != NULL);
    }
    if(save != -1)
    {
        printf("\nthe %s board is as bellow\n", name1);
        print(1);
        printf("\nthe %s board is as bellow\n", name2);
        print(2);

        printf("\n\n%s:%d  --  %s:%d", name1, point1, name2, point2);
        printf("\nthe winner is ");

        if (head1 == NULL) {
            point1 /= 2;

            printf("%s\n", name2);
        }
        else {
            point2 /= 2;

            printf("%s\n", name1);
        }
        printf("\nthe information is shown above\n\n*_* THE GAME IS OVER *_*\n");

        FILE *users = fopen("users.txt", "a+");
        fprintf(users, "%s %d\n", name1, point1);
        fclose(users);

        int num;
        FILE *number = fopen("number.txt", "r+");
        fscanf(number, "%d", &num);
        fclose(number);

        number = fopen("number.txt", "w+");
        fprintf(number, "%d", num + 1);
        fclose(number);
    }
    //deleting the current information for another game if the user wants
    for(int i = 0;i < map_size;i++) {
        for (int j = 0; j < map_size; j++) {
            board1[i][j] = '\0';
            board2[i][j] = '\0';
        }
    }
    int temp_arr[4];

    for(int i = 0;i < 30;i++) {
        if (head1 != NULL) {
            temp_arr[0] = head1->arr1[0];
            temp_arr[1] = head1->arr1[1];
            temp_arr[2] = head1->arr1[2];
            temp_arr[3] = head1->arr1[3];
            remove_ship1(&head1, temp_arr);
        }
        if (head2 != NULL) {
            temp_arr[0] = head2->arr2[0];
            temp_arr[1] = head2->arr2[1];
            temp_arr[2] = head2->arr2[2];
            temp_arr[3] = head2->arr2[3];
            remove_ship2(&head2, temp_arr);
        }
    }
    //preparing head1 and head2 for another game
    int arr[4] = {-5, -5, -5, -5};
    ships1* head3 = new_ship1(arr);
    ships2* head4 = new_ship2(arr);
    head11 = &head3;
    head22 = &head4;
}

int main(void){
///reseting the bin files
//    FILE* f = fopen("games.bin", "wb+");
//    fclose(f);
//    f = fopen("games_number.bin", "wb+");
//    int num = 0;
//    fwrite(&num, 4, 1, f);
//    fclose(f);
    setbuf(stdout, 0);
///basic settings
    int arr[4] = {-5, -5, -5, -5};
    ships1* head1 = new_ship1(arr);
    ships2* head2 = new_ship2(arr);
    shot_point[1] = 25;
    shot_point[2] = 12;
    shot_point[3] = 8;
    shot_point[5] = 5;
    arr_size[1] = 4;
    arr_size[2] = 3;
    arr_size[3] = 2;
    arr_size[5] = 1;
////basic settings 
    int choice = 0;

    while(choice != 7) {
        printf("\nplease choose one of the choices\n1. Play with a friend\n2. Play with a bot\n3. Load games\n4. Load last game\n5. Settings\n6. Score board\n7. Exit\n");
        scanf("%d", &choice);

        if (choice == 1) {
            int i = 0, score, num, check = 0, k = 0;

            printf("\nFirst player, choose your name:\n1. chose from available users\n2. new user\n");
            scanf("%d", &choice);

            if (choice == 1) {
                char name[50] = "\0";

                FILE *users = fopen("users.txt", "r+");
                FILE *number = fopen("number.txt", "r+");
                fscanf(number, "%d", &num);

                while (k != num) {
                    fscanf(users, "%s %d\n", name, &score);
                    printf("\n%d. %s %d", k + 1, name, score);
                    k++;
                }
                printf("\nchoose an index:");
                scanf("%d", &k);
                rewind(users);

                for (int j = 0; j < k; j++)
                    fscanf(users, "%s %d\n", name, &score);

                strcpy(name1, name);
                point1 = score;

                fclose(users);
                fclose(number);
            }
            if (choice == 2) {
                check = 0;
                char name_temp[50] = "\0", name[50] = "\0";

                printf("\nplease enter the new name:");
                scanf("%s", name_temp);

                FILE *users = fopen("users.txt", "r+");
                FILE *number = fopen("number.txt", "r+");
                fscanf(number, "%d", &num);

                while (check == 0) {
                    rewind(users);
                    i = 0;

                    while (i != num) {
                        fscanf(users, "%s %d\n", name, &score);
                        if (strcmp(name_temp, name) == 0)
                            check = 1;

                        i++;
                    }
                    if(check == 0)
                        check = 5;

                    if (check == 1) {
                        printf("\nwe have a user with that name. enter another name:");
                        fflush(stdin);
                        scanf("%s", &name_temp);
                        check = 0;
                    }
                }
                fclose(users);
                fclose(number);

                strcpy(name1, name_temp);
                point1 = score;
            }
            printf("\n%s choose an item:\n1. put ships automatically\n2. put ships manually\n", name1);
            scanf("%d", &choice);

            if (choice == 1) {
                get_inputs(head1, head2, 3);

                if(count_loops2 > 20000)
                {
                    printf("sorry, please try the manual way.\n");
                    get_inputs(head1, head2, 1);
                }
                print(1);
            }

            if (choice == 2)
                get_inputs(head1, head2, 1);

            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nSecond player:\n1. choose from available users\n2. new user\n");
            scanf("%d", &choice);

            if (choice == 1){

                char name[50] = "\0";

                FILE *users = fopen("users.txt", "r+");
                FILE *number = fopen("number.txt", "r+");
                fscanf(number, "%d", &num);

                while (i != num) {
                    fscanf(users, "%s %d\n", name, &score);
                    printf("\n%d. %s %d", i + 1, name, score);
                    i++;
                }
                i = k;

                while(k == i)
                {
                    printf("\nchoose an index:");
                    scanf("%d", &i);

                    if(k == i)
                        printf("\nplayer one has choosen that name. choose another index.\n");
                }

                rewind(users);

                for (int j = 0; j < i; j++)
                    fscanf(users, "%s %d\n", name, &score);

                strcpy(name2, name);
                point2 = score;

                fclose(users);
                fclose(number);
            }
            if (choice == 2) {
                check = 0;
                char name_temp[50] = "\0", name[50] = "\0";

                printf("\nplease enter the new name:");
                fflush(stdin);
                scanf("%s", &name_temp);

                FILE *users = fopen("users.txt", "r+");
                FILE *number = fopen("number.txt", "r+");
                fscanf(number, "%d", &num);

                while (check == 0) {
                    rewind(users);
                    i = 0;

                    while (i != num) {
                        fscanf(users, "%s %d\n", name, &score);
                        if (strcmp(name_temp, name) == 0 || strcmp(name1, name_temp) == 0)
                            check = 1;

                        i++;
                    }
                    if(check == 0)
                        check = 5;

                    if (check == 1) {
                        printf("\nwe have a user with that name. enter another name:");
                        fflush(stdin);
                        scanf("%s", &name_temp);
                        check = 0;
                    }
                }
                fclose(users);
                fclose(number);

                strcpy(name2, name_temp);
                point2 = score;
            }

            printf("\n%s choose from bellow\n1. put ships automatically\n2. put ships manually\n", name2);
            scanf("%d", &choice);

            if (choice == 1) {
                get_inputs(head1, head2, 4);
                print(2);
            }

            if (choice == 2)
                get_inputs(head1, head2, 2);

            mutual_play(&head1, &head2, 0);
            choice = 10;
        }
        if (choice == 2) {
            int i = 0, score, num, check = 0;

            printf("\nFirst player, choose your name:\n1. chose from available users\n2. new user\n");
            scanf("%d", &choice);

            if (choice == 1) {
                char name[50] = "\0";

                FILE *users = fopen("users.txt", "r+");
                FILE *number = fopen("number.txt", "r+");
                fscanf(number, "%d", &num);

                while (i != num) {
                    fscanf(users, "%s %d\n", name, &score);
                    printf("\n%d. %s %d", i + 1, name, score);
                    i++;
                }
                printf("\nchoose an index:");
                scanf("%d", &i);
                rewind(users);

                for (int j = 0; j < i; j++)
                    fscanf(users, "%s %d\n", name, &score);

                strcpy(name1, name);
                point1 = score;

                fclose(users);
                fclose(number);
                choice = 10;
            }
            if (choice == 2) {
                check = 0;
                char name_temp[50] = "\0", name[50] = "\0";

                printf("\nplease enter the new name:");
                fflush(stdin);
                scanf("%s", &name_temp);

                FILE *users = fopen("users.txt", "r+");
                FILE *number = fopen("number.txt", "r+");
                fscanf(number, "%d", &num);

                while (check == 0) {
                    rewind(users);
                    i = 0;

                    while (i != num) {
                        fscanf(users, "%s %d\n", name, &score);
                        if (strcmp(name_temp, name) == 0)
                            check = 1;

                        i++;
                    }
                    if(check == 0)
                        check = 5;

                    if (check == 1) {
                        printf("\nwe have a user with that name. enter another name:");
                        fflush(stdin);
                        scanf("%s", &name_temp);
                        check = 0;
                    }
                }
                fclose(users);
                fclose(number);

                strcpy(name1, name_temp);
                point1 = score;
                choice = 10;
            }
            printf("\n%s choose one choice:\n1. put ships automatically\n2. put ships manually\n", name1);
            scanf("%d", &choice);

            if (choice == 1) {
                get_inputs(head1, head2, 3);
                print(1);
            }

            if (choice == 2)
                get_inputs(head1, head2, 1);

            name2[0] = 'b';
            name2[1] = 'o';
            name2[2] = 't';
            name2[3] = '\0';

            printf("\n\n\n\n\n\n\n\n\n\n\nplease make decision to locate the bot ships:\n1. automatically\n2. manually(another person can set it for you)\n3. based on the constant map\n");
            scanf("%d", &choice);

            if(choice == 1)
                get_inputs(head1, head2, 4);

            if(choice == 2)
                get_inputs(head1, head2, 2);

            if(choice == 3)
                get_inputs(head1, head2, 5);

            bot_play(&head1, &head2, 0);
            choice = 10;
        }
        if (choice == 3) {
            printf("\nplease decide:\n1. loading a saved game\n2. removing a game\n");
            scanf("%d", &choice);

            if(choice == 1) {
                int num;
                FILE *number = fopen("games_number.bin", "rb+");
                fread(&num, 4, 1, number);
                fclose(number);

                FILE *games = fopen("games.bin", "rb+");
                game temp;

                for (int i = 0;i < num;i++) {
                    fread(&temp, sizeof(game), 1, games);
                    printf("\n%d. %s --  %s", i + 1, temp.name1, temp.name2);
                }
                printf("\nplease choose one game:");
                scanf("%d", &choice);

                fseek(games, (choice - 1) * sizeof(temp), SEEK_SET);
                fread(&temp, sizeof(temp), 1, games);

                point1 = temp.score1;
                point2 = temp.score2;

                map_size = temp.map_size;

                radar1 = temp.radar1;
                radar2 = temp.radar2;

                rocket1 = temp.rocket1;
                rocket2 = temp.rocket2;

                strcpy(name1, temp.name1);
                strcpy(name2, temp.name2);

                for(int i = 0;i < 30;i++)
                    shot_point[i] = temp.shot_point[i];

                int save = temp.turn;

                for (int i = 0; i < map_size; i++) {
                    for (int j = 0; j < map_size; j++) {
                        board1[i][j] = temp.board1[i][j];
                        board2[i][j] = temp.board2[i][j];
                    }
                }
                ships1* curr1 = head1;
                ships2* curr2 = head2;
                int temp_arr1[4], temp_arr2[4];

                for (int j = 0; j < 4; j++) {
                    head1->arr1[j] = temp.arr1[0][j];
                    head2->arr2[j] = temp.arr2[0][j];
                }

                for (int i = 1; i < 30; i++) {
                    for (int j = 0; j < 4; j++) {
                        temp_arr1[j] = temp.arr1[i][j];
                        temp_arr2[j] = temp.arr2[i][j];
                    }
                    if (temp_arr1[0] != -10)
                        add_end1(head1, new_ship1(temp_arr1));

                    if (temp_arr2[0] != -10)
                        add_end2(head2, new_ship2(temp_arr2));
                }
                if (save == 1 || save == 2)
                    mutual_play(&head1, &head2, save);
                else
                    bot_play(&head1, &head2, save);

                choice == 10;
            }

            if(choice == 2){
                FILE* games = fopen("games.bin", "rb+");
                FILE* num = fopen("games_number.bin", "rb+");

                int number, check;
                game data[50];

                check =  fread(&number, 4, 1, num);
                fclose(num);

                if(check == 0 || number == 0)
                    printf("\nthere is no game\n");

                else{
                    int i = 0, check = 1;
                    printf("\n");

                    while(check){
                        check = fread(&data[i], sizeof(game), 1, games);

                        if(check)
                        printf("%d. %s -- %s\n", i + 1, data[i].name1, data[i].name2);

                        i++;
                    }
                    fclose(games);

                    printf("please choose a game:\n");
                    scanf("%d", &choice);

                    games = fopen("games.bin", "wb+");

                    i = 0;

                    while(i != number) {
                        if (i + 1 != choice)
                            fwrite(&data[i], sizeof(game), 1, games);

                        i++;
                    }

                    fclose(games);

                    number--;

                    num = fopen("games_number.bin", "wb+");
                    fwrite(&number, 4, 1, num);
                    fclose(num);

                    printf("\nthe games is removed properly\n");
                }
                choice = 10;
            }
        }
        if (choice == 4) {
            int num;
            FILE* number = fopen("games_number.bin", "rb+");
            fread(&num, 4, 1, number);
            fclose(number);

            FILE* games = fopen("games.bin", "rb+");
            game temp;

            fseek(games, -1 * sizeof(temp), SEEK_END);
            fread(&temp, sizeof(temp), 1, games);

            point1 = temp.score1;
            point2 = temp.score2;

            map_size = temp.map_size;

            radar1 = temp.radar1;
            radar2 = temp.radar2;

            rocket1 = temp.rocket1;
            rocket2 = temp.rocket2;

            strcpy(name1, temp.name1);
            strcpy(name2, temp.name2);

            for(int i = 0;i < 30;i++)
                shot_point[i] = temp.shot_point[i];

            int save = temp.turn;

            for(int i = 0;i < map_size;i++){
                for(int j = 0;j < map_size;j++){
                    board1[i][j] = temp.board1[i][j];
                    board2[i][j] = temp.board2[i][j];
                }
            }
            ships1* curr1 = head1;
            ships2* curr2 = head2;
            int temp_arr1[4], temp_arr2[4];

            for(int j = 0;j < 4;j++){
                temp_arr1[j] = temp.arr1[0][j];
                temp_arr2[j] = temp.arr2[0][j];
            }
            head1 = new_ship1(temp_arr1);
            head2 = new_ship2(temp_arr2);

            for(int i = 1;i < map_size;i++){
                for(int j = 0;j < 4;j++){
                    temp_arr1[j] = temp.arr1[i][j];
                    temp_arr2[j] = temp.arr2[i][j];
                }
                if(temp_arr1[0] != -10)
                    add_end1(head1, new_ship1(temp_arr1));
                if(temp_arr2[0] != -10)
                    add_end2(head2, new_ship2(temp_arr2));
            }
            if(save == 1 || save == 2)
                mutual_play(&head1, &head2, save);
            else
                bot_play(&head1, &head2, save);

            choice == 10;
        }
        if (choice == 5) {
            int choicee;
            printf("\nplease choose one item:\n1. ships\n2. map size\n");
            scanf("%d", &choicee);

            if (choicee == 1) {
                int temp = 1, temp_num;
                printf("\nplease enter the new ships with their length(finish your input with 0 as length). the maximum length is 25:");

                for(int i = 0;i < 30;i++)
                    arr_size[i] = 0;

                for(int i = 0;i < 30;i++)
                    shot_point[i] = 0;

                printf("\nplease enter the length of the ship you want to have:");
                scanf("%d", &temp);

                while (temp != 0) {
                    printf("\nplease enter the number of the ships with the %d length:", temp);
                    scanf("%d", &temp_num);

                    arr_size[temp] = temp_num;

                    printf("\nplease enter the length of the ship you want to have:");
                    scanf("%d", &temp);
                }
                for(int i = 0;i < 30;i++) {
                    if (arr_size[i] != 0)
                        temp = i;
                }

                max_size = temp;

                for(int i = 0;i < 30;i++)
                    if(arr_size[i] != 0)
                        shot_point[i] = (5 * temp) / i;

                ships_sum = 0;
                for(int i = 0;i < 30;i++)
                    if(arr_size[i] != 0)
                        ships_sum += i * arr_size[i];


                printf("\nDONE\n");
            }
            if (choicee == 2) {
                int check = 1, temp;

                while(check) {
                    printf("\nplease enter the new map size:");
                    scanf("%d", &temp);

                    if(temp * temp > ships_sum * 2 - temp)
                        check = 0;

                    else
                        printf("\nyour input is not acceptable.");
                }
                map_size = temp;
                printf("\nDONE\n");
            }
        }
        if (choice == 6) {
            printf("\nplease decide:\n1. show scoreboard\n2. removing a person from scoreboard\n");
            scanf("%d", &choice);

            if(choice == 1) {
                char name_data[500][50], temp_name[50];
                int score_data[500] = {0}, temp;
                int num, i = 0;

                FILE *users = fopen("users.txt", "r+");
                FILE *number = fopen("number.txt", "r+");
                fscanf(number, "%d", &num);

                while (i != num) {
                    fscanf(users, "%s %d\n", name_data[i], &score_data[i]);
                    i++;
                }
                fclose(users);
                fclose(number);

                for (int j = 0; j < num; j++) {
                    for (i = 0; i < num - 1; i++) {
                        if (score_data[i] < score_data[i + 1]) {
                            temp = score_data[i + 1];
                            score_data[i + 1] = score_data[i];
                            score_data[i] = temp;
                            strcpy(temp_name, name_data[i]);
                            strcpy(name_data[i], name_data[i + 1]);
                            strcpy(name_data[i + 1], temp_name);
                        }
                    }
                }
                for (i = 0; i < num; i++)
                    printf("\n%d. %s %d", i + 1, name_data[i], score_data[i]);
                printf("\n");
            }
            if(choice == 2){
                char name_data[500][50], temp_name[50];
                int score_data[500] = {0}, temp;
                int num, i = 0, line;

                FILE *users = fopen("users.txt", "r+");
                FILE *number = fopen("number.txt", "r+");
                fscanf(number, "%d", &num);

                while (i != num) {
                    fscanf(users, "%s %d\n", name_data[i], &score_data[i]);
                    printf("\n%d. %s %d", i + 1, name_data[i], score_data[i]);
                    i++;
                }
                fclose(users);
                fclose(number);

                printf("\nplease enter the index of the person you want to remove:");
                scanf("%d", &line);

                users = fopen("users.txt", "w+");

                for(int k = 0;k < num;k++)
                    if(k != line - 1)
                        fprintf(users, "%s %d\n", name_data[k], score_data[k]);

                fclose(users);
                users = fopen("number.txt", "w+");
                fprintf(users, "%d", num - 1);
                fclose(users);
                printf("\nDONE\n");
            }

            choice = 10;
        }
    }


    return 0;
}