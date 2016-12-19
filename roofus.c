#define VERSION "0.02"

#define N 5
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void start ();
void printm ();
void move (char *dir);
void nolow ();
int win ();
int error ();

int matrix[N][N];
int my_x = N / 2;
int my_y = N / 2;
int moves = 0;

int main () {
    char dir[20];
    char exit;
    int end = 0;
	srand (time(NULL));
    
    start ();
    printm ();
    while (end == 0) {
        printf ("\nLeft: a. Up: w. Right: d. Down: s. Exit: 0");
        printf ("\nDirection?  ");
        scanf ("%s", dir);
        getchar ();
        if (dir[0] == '0') {
            printf ("\nExit? [y/n]  ");
            scanf ("%c", &exit);
            if (exit == 'y') {
                printf ("\n\n\n\nv%s", VERSION);
                return moves;
            } else {
                dir[0] = '1';
                getchar ();
            }
        }
        move (dir);
        printm ();
        end = win ();
    }
    return 0;
}

void start () {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            matrix[i][j] = rand () % 19 + 1;
        }
    }
    nolow ();
}

void move (char *dir) {
    int xinit, yinit;
    int stop = 0;
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
            if (my_x < N-1) {
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
            if(my_y < N-1) {
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
            for (j = 1 + xinit; j <= 4; j++) {
                if (matrix[yinit][j] != 0) {
                    matrix[yinit][j] = matrix[yinit][j] - matrix[yinit][xinit];
                    if (matrix[yinit][j] < 0 && matrix[yinit][xinit] != 0)
                        matrix[yinit][j] = -matrix[yinit][j];
                }
            }
        }
        if (*dir == 'a') {
            for (j = 0; j < xinit; j++) {
                if (matrix[yinit][j] != 0) {
                    matrix[yinit][j] = matrix[yinit][j] - matrix[yinit][xinit];
                    if (matrix[yinit][j] < 0 && matrix[yinit][xinit] != 0)
                        matrix[yinit][j] = -matrix[yinit][j];
                }
            }
        }
         if (*dir == 'w'){
            for (i = my_y;i >= 0; i--) {
                if (matrix[i][my_x] != 0) {
                    matrix[i][my_x] = matrix[i][my_x] - matrix[yinit][xinit];
                    if (matrix[i][my_x] < 0 && matrix[yinit][xinit] != 0)
                        matrix[i][my_x] = -matrix[i][my_x];
                }
            }
        }
         if (*dir == 's'){
            for (i = my_y; i < N; i++) {
                if(matrix[i][my_x] != 0) {
                    matrix[i][my_x] = matrix[i][my_x] - matrix[yinit][xinit];
                    if (matrix[i][my_x] < 0 && matrix[yinit][xinit] != 0)
                        matrix[i][my_x] = -matrix[i][my_x];
                }
            }
        }
        nolow ();
        moves++;
    }
}

// if 1 or 2 -> random number
void nolow () {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if ( matrix[i][j] < 3 && matrix[i][j] > 0)
                matrix[i][j] = rand () % 10 - 10;
        }
    }
}

// print matrix
void printm () {
    int i, j;
    printf ("\n\n");
    for (i = 0; i < N; i++) {
        for (j = 0;j < N; j++) {
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

int win () { // 20+ zeros
    int i, j, count=0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (matrix[i][j] == 0)
                count++;
        }
    }
    if (count >= N * (N-1)) {
        printf ("\nYou win! Moves: %d", moves);
        return 1;
    }else{
        printf ("\nYou still need to delete %d numbers", N * (N-1) - count);
        return 0;
    }
}

int error () {
    printf ("ERROR!!!");
    return 1;
}
