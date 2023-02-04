#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define width 80
#define height 25

void input(int** uni);
int **next_generation(int **universe);
int alive_neighbour(int **universe, int row, int column);
void copy(int **old, int **new);
void output(int **matrix);
void key(int *speed);

int main() {
    initscr();
    noecho();
    nodelay(stdscr, true);
    int delay = 1;
    int **universe = calloc(height, sizeof(int *));
    for (int i = 0; i < height; i++) {
        universe[i] = calloc(width, sizeof(int));
    }
    input(universe);
    freopen("/dev/tty","r", stdin);
    while (1) {
        output(universe);
        printw("%d", delay);
        refresh();
        key(&delay);
        usleep(delay * 100000); 
        int **nuniverse = next_generation(universe);
        copy(nuniverse, universe);
        
    }
    endwin();
    return 0;
}

void key(int *speed) {
    char c = getch();
    if (c == '-' && (*speed) > 0) {
        (*speed)--;
    }
    if (c == '+') {
        (*speed)++;
    }
}

void copy(int **old, int **new) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            new[i][j] = old[i][j];
        }
        free(old[i]);
    }
    free(old);
}

void input(int **uni) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            scanf("%d", &uni[i][j]);
        }
    }
}

int **next_generation(int **universe) {
    int **update = calloc(height, sizeof(int *));
    for (int i = 0; i < height; i++) {
        update[i] = calloc(width, sizeof(int));
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int a = alive_neighbour(universe, i, j);

            if (universe[i][j] == 1) {
                if (a == 2 || a == 3) update[i][j] = 1;

            } else if (universe[i][j] == 0) {
                if (a == 3) update[i][j] = 1;

            } else {
                update[i][j] = 0;
            }
        }
    }
    return update;
}

int alive_neighbour(int **universe, int row, int column) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int indexI = (row + i + height) % height;
            int indexJ = (column + j + width)  % width;
            if (universe[indexI][indexJ] == 1) count += 1;
        }
    }
    if (universe[row][column] == 1) count--;

    return count;
}

void output(int **matrix) {
    clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printw("%c", matrix[i][j] == 0 ? ' ' : '#');

        }

    printw("\n");
    }
}