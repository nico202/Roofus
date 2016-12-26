#define VERSION "1.00"

#define N 9
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void restoregrid ();
void menu ();
void play ();
void start ();
void move (char *dir);
void nolow ();
void settings ();
void ranking ();
void rules ();
void printm ();
int win ();
int error ();

int matrix[N][N] = {0};
int my_x;
int my_y;
int moves;
int grid;
int gridset = 0;
FILE *fp;

int main () {
	srand (time(NULL));
    fp = fopen ("roofus.txt", "a+");
    if (fp == NULL) {
        printf ("Error opening file");
    }else{
        restoregrid ();
        menu ();
    }
    fclose (fp);
    printf ("\n\n\n\nRoofus v%s", VERSION);
    return 0;
}

// restore last grid size
void restoregrid () {
    int gridtemp, length;
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, (length - 2), SEEK_SET); // go to grid size character
        gridtemp = fgetc(fp) - 48; // -48 to convert char in int
        if (gridtemp < 3 || gridtemp > 9)
            grid = 5;
        else
            grid = gridtemp;
}

void menu (){
    char mode[5];
    while (1) {
        system("clear");
        printf ("~~~ ROOFUS ~~~\n\n[1] Play\n[2] Ranking\n[3] Settings\n[4] Rules\n[0] Exit\n\n");
        scanf ("%5s", mode);
        switch (mode[0]) {
            case '0': return;
            case '1': play (); break;
            case '2': ranking (); break;
            case '3': settings (); break;
            case '4': rules (); break;
        }
    }
}

void play () {
    char dir[5], user[10];
    char exit = 0;
    int end = 0;
    char *text = malloc (sizeof (text) * N);
    moves = 0;
    system("clear");
    my_x = grid / 2;
    my_y = grid / 2;
    start ();
    printm ();
    while (end == 0) {
        printf ("\nLeft [a]; Up [w]; Right [d]; Down [s]. Menu [0]");
        printf ("\nDirection?  ");
        scanf ("%5s", dir);
        getchar ();
        if (dir[0] == '0') {
            printf ("\nAre you sure? [y/n]  ");
            scanf ("%c", &exit);
            if (exit == 'y') {
                return;
            } else {
                dir[0] = '1';
                getchar ();
            }
        }
        move (dir);
        printm ();
        end = win ();
        if (end != 0) {
            printf ("Enter your name: ");
            scanf ("%10s", user);
        }
    }
    sprintf (text, "%li %d %s %d\n", time(NULL), moves, user, grid);
    fputs (text, fp);
    free (text);
    return;
}

void start () {
    int i, j;
    for (i = 0; i < grid; i++) {
        for (j = 0; j < grid; j++) {
            matrix[i][j] = rand () % 19 + 1;
        }
    }
    nolow ();
}

void move (char *dir) {
    int xinit = my_x, yinit = my_y;
    int stop = 0, del = 0;
    int i, j;
    // move pointer
    switch (dir[0]) { 
        case 'a': 
            if (my_x != 0) {
                my_x--;
            } else {
                stop = error ();
            }
            break;
        case 'd': 
            if (my_x < grid - 1) {
                my_x++; 
            } else {
                stop = error ();
            }
            break;
        case 'w':                 
            if (my_y > 0) {
                my_y--; 
            } else {
                stop = error ();
            }
            break;
        case 's':
            if(my_y < grid - 1) {
                my_y++; 
            } else {
                stop = error ();
            }
            break;
        case 'o':
            stop = 1;
            break;
        default:
            stop = error ();
            break;
        }
    if (stop == 0) {
        // change values
        if (*dir == 'd') {
            for (j = my_x; j < grid; j++) {
                if (matrix[my_y][j] != 0) {
                    matrix[my_y][j] = matrix[my_y][j] - matrix[yinit][xinit];
                    if (matrix[my_y][j] == 0)
                        del = 1;
                    if (matrix[my_y][j] < 0 && matrix[yinit][xinit] != 0)
                        matrix[my_y][j] = -matrix[my_y][j];
                }
            }
        }
        if (*dir == 'a') {
            for (j = 0; j < xinit; j++) {
                if (matrix[my_y][j] != 0) {
                    matrix[my_y][j] = matrix[my_y][j] - matrix[my_y][xinit];
                    if (matrix[my_y][j] == 0)
                        del = 1;
                    if (matrix[my_y][j] < 0 && matrix[yinit][xinit] != 0)
                        matrix[my_y][j] = - matrix[my_y][j];
                }
            }
        }
         if (*dir == 'w'){
            for (i = my_y; i >= 0; i--) {
                if (matrix[i][my_x] != 0) {
                    matrix[i][my_x] = matrix[i][my_x] - matrix[yinit][xinit];
                    if (matrix[i][my_x] == 0)
                        del = 1;
                    if (matrix[i][my_x] < 0 && matrix[yinit][xinit] != 0)
                        matrix[i][my_x] = -matrix[i][my_x];
                }
            }
        }
         if (*dir == 's'){
            for (i = my_y; i < grid; i++) {
                if(matrix[i][my_x] != 0) {
                    matrix[i][my_x] = matrix[i][my_x] - matrix[yinit][xinit];
                    if (matrix[i][my_x] == 0)
                        del = 1;
                    if (matrix[i][my_x] < 0 && matrix[yinit][xinit] != 0)
                        matrix[i][my_x] = -matrix[i][my_x];
                }
            }
        }
        if (del == 1)
            matrix[yinit][xinit] = 0;
        nolow ();
        moves++;
    }
}

// if 1 or 2 -> random number
void nolow () {
    int i, j;
    for (i = 0; i < grid; i++) {
        for (j = 0; j < grid; j++) {
            if ( matrix[i][j] < 3 && matrix[i][j] > 0)
                matrix[i][j] = rand () % 10 - 10;
        }
    }
}

void settings () {
    char setting[5];
    int menu = 0;
    system("clear");
    printf ("> Settings\n\n");
    while (menu == 0) {
        printf ("Menu [0]\nGrid size? [3-9] ");
        scanf ("%5s", setting);
        switch (setting[0]) {
            case '0': menu = 1; break;
            case '3': grid = 3; gridset = 1; menu = 1; break;
            case '4': grid = 4; gridset = 1; menu = 1; break;
            case '5': grid = 5; gridset = 1; menu = 1; break;
            case '6': grid = 6; gridset = 1; menu = 1; break;
            case '7': grid = 7; gridset = 1; menu = 1; break;
            case '8': grid = 8; gridset = 1; menu = 1; break;
            case '9': grid = 9; gridset = 1; menu = 1; break;
            default: printf("Retry\n"); break;
        }
    }
}

void ranking () {
    int i = 0, j;
    struct res {
        int moves;
        int grid;
        char user[10];
        time_t res_time;
    };
    struct res result[1000];
    struct res temp;
    char *time_string[1000];
    int grid_val = grid;
    int printed;
    rewind (fp);
    getchar ();
    do {
        fscanf (fp, "%li %d %s %d", &result[i].res_time, &result[i].moves, result[i].user, &result[i].grid);
        i++;  
    } while (result[i-1].moves > 0);
    for (i = 0; result[i].moves != 0; i++) {
        for (j = i + 1; result[j].moves != 0; j++) {
            if (result[i].moves > result[j].moves) {
                temp = result[i];
                result[i] = result[j];
                result[j] = temp;
            }
        }
    }
    do {
        system("clear");
        if (grid_val >= 3 && grid_val <= 9)
            printf ("Best results (%dx%d matrix):\n\n", grid_val, grid_val);
        else if (grid_val == 1)
            printf ("Best results:\n\n");
        printed = 0;
        for (i = 0; result[i].moves != 0; i++) {
            if ((result[i].grid == grid_val || grid_val == 1) && printed < 9) {
                time_string[i] = ctime(&result[i].res_time);
                printf ("# %d - %3d moves\t- (%dx%d matrix) - %s\t- %s", printed + 1, result[i].moves, result[i].grid, result[i].grid, result[i].user, time_string[i]);
                printed++;
            }
        }
        if (printed == 0) {
            printf ("Nothing to show...\n");
        }
        do {
            printf ("\nMenu [0]; To filter by matrix size [3-9]; No filter [1] ");
            scanf ("%1d", &grid_val);
            getchar();
        } while ((grid_val < 3 || grid_val > 9) && grid_val != 1 && grid_val != 0);
    } while (grid_val != 0);
}

void rules () {
    system("clear");
    getchar ();
    printf ("> Rules\n\n");
    printf ("Delete numbers on the table by moving the pointer.\n");
    printf ("That's all you need to know\n");
    getchar ();
}

// print matrix
void printm () {
    int i, j;
    system("clear");
    printf ("\n\n");
    for (i = 0; i < grid; i++) {
        for (j = 0;j < grid; j++) {
            if (i == my_y && j == my_x)
                printf ("> ");
            if (matrix[i][j] != 0)
                    printf ("%d\t", matrix[i][j]);
                else
                    printf (" \t");
        }
        printf ("\n\n");
    }
}

int win () {
    int i, j, count=0;
    for (i = 0; i < grid; i++) {
        for (j = 0; j < grid; j++) {
            if (matrix[i][j] == 0)
                count++;
        }
    }
    if (count >= grid * (grid - 1)) {
        printf ("\nYou win! Moves: %d\n", moves);
        getchar ();
        return 1;
    }else{
        printf ("\nYou still need to delete %d numbers. Moves: %d", grid * (grid - 1) - count, moves);
        return 0;
    }
}

int error () {
    printf ("ERROR!!!");
    return 1;
}
