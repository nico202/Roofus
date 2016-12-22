#define VERSION "0.01"

#define N 9
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void menu ();
int play ();
void start (); // initial condition
void settings ();
void printm (); // print matrix
void move (char *dir); // move pointer
void ranking ();
int win ();
void lose (); // print lose message
int error (); // error message, return 1

int matrix[N][N] = {0};
int my_x;
int my_y;
int moves;
int grid = 9; // grid size
int lost = 0;

int main () {
	srand (time(NULL));
    
    menu ();
    
    printf ("\n\n\n\nv%s", VERSION);
    return 0;
}

// Select mode
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

// edit grid size
void settings () {
    char setting[5];
    int menu = 0;
    system("clear");
    while (menu == 0) {
        printf ("Menu: press 0\nGrid size? 5, 7 or 9? ");
        scanf ("%5s", setting);
        switch (setting[0]) {
            case '0': menu = 1; break;
            case '5': grid = 5; menu = 1; break;
            case '7': grid = 7; menu = 1; break;
            case '9': grid = 9; menu = 1; break;
            default: printf("Retry\n"); break;
        }
    }    
}

// not available
void ranking () {
    char back[5];
    system("clear");
    printf ("Not available. Press 0\n");
    scanf ("%5s", back);
}

int play () {
    char dir[5];
    char exit = 0;
    int end = 0; // if win or lose == 1
    my_x = grid / 2;  // starting position
    my_y = grid - 1;
    moves = 0;
    system("clear");
    start (); // create matrix
    printm ();
    while (!end) {
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
        if (win () == 1 || lost == 1)
            end = 1;
    }
    return 1;
}

// create matrix
void start () {
    int i, j, k;
    lost = 0;
    for (k = 0; k <= grid / 2; k++) {
        i = rand () % grid;
        j = rand () % grid;
        matrix[i][j] = rand () % 10 + 1;
    }
    matrix[my_y][my_x] = grid;
}

void move (char *dir) {
    int stop = 0;
    int finalvalue, initvalue = matrix[my_y][my_x];
    // move pointer
    matrix[my_y][my_x] = 0;
    switch (dir[0]) { 
        case 'a': 
            if (my_x > 0) {
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
    if (!stop) { // if moved
        finalvalue = matrix[my_y][my_x];
        if (finalvalue <= initvalue) {
            if (finalvalue != 0)
                initvalue++;
            matrix[my_y][my_x] = initvalue - 1 + finalvalue;
            printm ();
        } else {
            matrix[my_y][my_x] = initvalue - 1 + finalvalue;
            lost = 1;
            printm ();
            lose ();
        }
        moves++;
        if (matrix[my_y][my_x] == 0) {
            lost = 1;
            printm ();
            lose ();
        }
    } else {
        matrix[my_y][my_x] = initvalue;
    }
}

// print matrix
void printm () {
    int i, j;
    system("clear");
    printf ("\n\n");
    for (i = 0; i < grid; i++) {
        for (j = 0;j < grid; j++) {
            if (i == my_y && j == my_x && matrix[i][j] != 0 && lost != 1)
                printf ("< ");
            if (matrix[i][j] != 0) {
                    printf ("%d", matrix[i][j]);
                    if (i == my_y && j == my_x && lost != 1)
                        printf (" >");
                    printf ("\t");
                } else {
                    printf (" .\t");
                }
        }
        printf ("\n\n");
    }
}

// win checker
int win () {
    int i, j, count=0;
    for (i = 0; i < grid; i++) {
        for (j = 0; j < grid; j++) {
            if (matrix[i][j] == 0)
                count++;
        }
    }
    if (!lost) {
        if (count == grid * grid - 1) {
            printf ("\nYou win! Moves: %d\n", moves);
            getchar ();
            return 1;
        } else {
            printf ("\nYou still need to delete %d numbers. Moves: %d", grid * grid - 1 - count, moves);
            return 0;
        }
    }
    return 1;
}

// lose message
void lose () {
    printf ("You lose!");
    getchar ();
}

// error message
int error () {
    printf ("ERROR!!!");
    return 1;
}
