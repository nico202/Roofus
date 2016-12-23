#define VERSION "0.20"

#define N 9
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void menu ();
int play ();
void start ();
void settings ();
void printm ();
void move (char *dir);
void nolow ();
void ranking ();
int win ();
int error ();

int matrix[N][N] = {0};
int my_x;
int my_y;
int moves;
int grid = 5;
FILE *fp;

int main () {
	srand (time(NULL));
    fp = fopen ("roofus.txt", "a+");
    if (fp == NULL) {
        printf ("Error opening file");
    }else{
        menu ();
    }
    fclose (fp);
    printf ("\n\n\n\nv%s", VERSION);
    return 0;
}

void menu (){
    char mode[5];
    int menu = 1;
    while (menu == 1) {
        system("clear");
        printf ("Select mode:\n> 0: Play\n> 1: Settings\n> 2: Ranking\n> 3: Exit\n");
        scanf ("%5s", mode);
        switch (mode[0]) {
            case '0': menu = play (); break;
            case '1': settings (); break;
            case '2': ranking (); break;
            case '3': return;
        }
    }
}

void settings () {
    char setting[5];
    int menu = 0;
    system("clear");
    while (menu == 0) {
        printf ("Menu: press 0\nGrid size? 3, 5, 7 or 9? ");
        scanf ("%5s", setting);
        switch (setting[0]) {
            case '0': menu = 1; break;
            case '3': grid = 3; menu = 1; break;
            case '5': grid = 5; menu = 1; break;
            case '7': grid = 7; menu = 1; break;
            case '9': grid = 9; menu = 1; break;
            default: printf("Retry\n"); break;
        }
    }
}

void ranking () {
    int i = 0, j ,a;
    int best[1000] = {0};
    int grid_val[1000];
    time_t res_time[1000];
    char *time_string[1000];
    system("clear");
    rewind (fp);
    getchar ();
    do {
        fscanf (fp, "%d %li %d", &best[i], &res_time[i], &grid_val[i]);
        i++;  
    } while (best[i-1] > 0);
    for (i = 0; best[i] != 0; i++) {
        for (j = i + 1; best[j] !=0; j++) {
            if (best[i] > best[j]) {
                a = best[i];
                best[i] = best[j];
                best[j] = a;
            }
        }
    }
    printf ("Best results: \n\n");
    for (i=0; i < 10 && best[i] != 0; i++) { 
        time_string[i] = ctime(&res_time[i]);
//      printf ("#%d: %3d moves\t- %dx%d matrix\t- %s", i + 1, best[i], grid_val[i], grid_val[i], time_string[i]);
        printf ("#%d: %3d moves\n", i + 1, best[i]);
    }
    getchar ();
}

int play () {
    char dir[5];
    char exit = 0;
    int end = 0;
    char *text = malloc (sizeof (text) * N);
    my_x = grid / 2;
    my_y = grid / 2;
    moves = 0;    
    system("clear");
    start ();
    printm ();
    while (end == 0) {
        printf ("\nLeft: a. Up: w. Right: d. Down: s. Menu: m. Exit: 0");
        printf ("\nDirection?  ");
        scanf ("%5s", dir);
        getchar ();
        if (dir[0] == '0') {
            printf ("\nExit? [y/n]  ");
            scanf ("%c", &exit);
            if (exit == 'y') {
                return 0;
            } else {
                dir[0] = '1';
                getchar ();
            }
        }
        if (dir[0] == 'm') {
            printf ("\nAre you sure? [y/n]  ");
            scanf ("%c", &exit);
            if (exit == 'y') {
                return 1;
            } else {
                dir[0] = '1';
                getchar ();
            }
        }
        move (dir);
        printm ();
        end = win ();
    }
    sprintf (text, "%d %li %d\n", moves, time(NULL), grid);
    fputs (text, fp);
    free (text);
    return 1;
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
    int xinit, yinit;
    int stop = 0, del = 0;
    int i, j;

    xinit = my_x;
    yinit = my_y;
    
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
