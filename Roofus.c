#define VERSION "1.00"

#define N 9
#define USERNAME_LENGTH 10

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

void restoregrid ();
void menu ();
void play ();
void start ();
void move_ptr (int direction);
void nolow ();
void settings ();
void ranking ();
void rules ();
void printm ();
int win ();

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
        printw ("Error opening file");
        refresh();
    }else{
        restoregrid ();
        menu ();
    }
    fclose (fp);
    printw ("\n\n\n\nRoofus v%s", VERSION);
    refresh ();
    endwin();
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
    int mode;
  
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);

    while (1) {
        clear();
        printw ("~~~ ROOFUS ~~~\n\n[1] Play\n[2] Ranking\n[3] Settings\n[4] Rules\n[q] Quit\n\n");
        refresh();
        mode = getch();

        switch (mode) {
        case 'q': return;
        case '1': play (); break;
        case '2': ranking (); break;
        case '3': settings (); break;
        case '4': rules (); break;
        }
    }
    /* return 0; */
}

void play () {
    char user[USERNAME_LENGTH];
    int direction;
    int exit = 0;
    int end = 0;
    char *text = malloc (sizeof (text) * N);
    moves = 0;
    clear();
    my_x = grid / 2;
    my_y = grid / 2;
    start ();

    while (end == 0) {
        printw ("\nArrow to move, q to quit\n");
        refresh();

        direction = getch();
        if (direction == 'q') {
            printw ("\nAre you sure? [y/n]  ");
            refresh();
            exit = getch();
            if (exit == 'y') {
                return;
            } else {
                direction = '1';
                getchar ();
            }
        }

        move_ptr (direction);
        printm ();
        end = win ();
        if (end != 0) {
            echo();
            printw ("Enter your name: ");
            refresh();
            getnstr(user, USERNAME_LENGTH); // read at most
        }
    }
    sprintf (text, "%li %d %s %d\n", time(NULL), moves, user, grid);
    refresh();
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
    printm();
}

void move_ptr (int direction) {
    int xinit = my_x, yinit = my_y;
    int stop = 0, del = 0;
    int i, j;
    // move_ptr pointer
    switch (direction) { 
    case KEY_LEFT: 
        if (my_x != 0) {
            my_x--;
        } else {
            stop = 1;
        }
        break;
    case KEY_RIGHT: 
        if (my_x < grid - 1) {
            my_x++; 
        } else {
            stop = 1;
        }
        break;
    case KEY_UP:                 
        if (my_y > 0) {
            my_y--; 
        } else {
            stop = 1;
        }
        break;
    case KEY_DOWN:
        if(my_y < grid - 1) {
            my_y++; 
        } else {
            stop = 1;
        }
        break;
    default:
        stop = 1;
        break;
    }
    if (stop == 0) {
        // change values
        if (direction == KEY_RIGHT) {
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
        if (direction == KEY_LEFT) {
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
        if (direction == KEY_UP){
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
        if (direction == KEY_DOWN){
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
    int setting;
    int menu = 0;
    clear();
    printw ("> Settings\n\n");
    while (menu == 0) {
        printw ("Menu [q]\nGrid size? [3-9]");
        refresh();
        setting = getch();
        switch (setting) {
        case 'q': menu = 1; break;
        case '3': grid = 3; gridset = 1; menu = 1; break;
        case '4': grid = 4; gridset = 1; menu = 1; break;
        case '5': grid = 5; gridset = 1; menu = 1; break;
        case '6': grid = 6; gridset = 1; menu = 1; break;
        case '7': grid = 7; gridset = 1; menu = 1; break;
        case '8': grid = 8; gridset = 1; menu = 1; break;
        case '9': grid = 9; gridset = 1; menu = 1; break;
        default: printw("Retry\n"); break;
            refresh();
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
        clear();
        if (grid_val >= 3 && grid_val <= 9)
        {
            printw ("Best results (%dx%d matrix):\n\n", grid_val, grid_val);
            refresh();
        }
        else if (grid_val == 1)
            printw ("Best results:\n\n");
        printed = 0;
        for (i = 0; result[i].moves != 0; i++) {
            if ((result[i].grid == grid_val || grid_val == 1) && printed < 9) {
                time_string[i] = ctime(&result[i].res_time);
                printw ("# %d - %3d moves\t- (%dx%d matrix) - %s\t- %s", printed + 1, result[i].moves, result[i].grid, result[i].grid, result[i].user, time_string[i]);
                refresh();
                printed++;
            }
        }
        if (printed == 0) {
            printw ("Nothing to show...\n");
            refresh();
        }
        printw ("\nMenu [q]; To filter by matrix size [3-9]; No filter [1] ");
        refresh();

        do {
            grid_val = getch() - '0';
            // exit with 'q' - '0'
        } while ((grid_val < 3 || grid_val > 9) && grid_val != 1 && grid_val != 'q'-'0');
    } while (grid_val != ('q'-'0'));
}

void rules () {
    clear();
    printw ("> Rules\n\n");
    printw ("Delete numbers on the table by moving the pointer.\n");
    printw ("That's all you need to know\n");
    refresh();
    getch();
}

// print matrix
void printm () {
    int i, j;
    clear();
    printw ("\n\n");
    for (i = 0; i < grid; i++) {
        for (j = 0;j < grid; j++) {
            if (i == my_y && j == my_x)
                printw ("> ");
            if (matrix[i][j] != 0)
                printw ("%d\t", matrix[i][j]);
            else
                printw (" \t");
        }
        printw ("\n\n");
    }
    refresh();
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
        printw ("\nYou win! Moves: %d\n", moves);
        refresh ();
        getchar ();
        return 1;
    }else{
        printw ("\nYou still need to delete %d numbers. Moves: %d", grid * (grid - 1) - count, moves);
        refresh();
        return 0;
    }
}
