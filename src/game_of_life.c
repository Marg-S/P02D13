#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define width 80
#define height 25
#define max_speed 20

void input(int uni[height][width]);
void next_generation(int universe[height][width], int update[height][width]);
int alive_neighbour(int universe[height][width], int row, int column);
void copy(int old[height][width], int new[height][width]);
void output(int universe[height][width]);
void key(int *speed);
int stop(int old[height][width], int new[height][width]);

int main() {
    int delay = max_speed / 2;
    int universe[height][width];
    input(universe);
    initscr();
    noecho();
    nodelay(stdscr, true);
    freopen("/dev/tty", "r", stdin);
    int nuniverse[height][width];
    while (1) {
        output(universe);

        printw("current speed: %d", max_speed - delay);
        refresh();
        key(&delay);
        usleep(delay * 50000);
        next_generation(universe, nuniverse);
        if (stop(universe, nuniverse)) {
            break;
        }
        copy(nuniverse, universe);
    }
   // endwin();
    return 0;
}

void key(int *speed) {
    char c = getch();
    if (c == '+' && (*speed) > 1) {
        (*speed)--;
    }
    if (c == '-' && (*speed) < 20) {
        (*speed)++;
    }
}

int stop(int old[height][width], int new[height][width]) {
    int flag = 1;
    int count = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (new[i][j] == 1) {
                count++;
            }
            if (new[i][j] != old[i][j]) {
                flag = 0;
            }
        }
    }
    return flag || !count;
}

void copy(int old[height][width], int new[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            new[i][j] = old[i][j];
        }
    }
}

void input(int uni[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            scanf("%d", &uni[i][j]);
        }
    }
}

void next_generation(int universe[height][width], int update[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            update[i][j] = 0;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int a = alive_neighbour(universe, i, j);
            printw("%d", a);
            if (universe[i][j] == 1) {
                if (a == 2 || a == 3) update[i][j] = 1;

            } else if (universe[i][j] == 0) {
                if (a == 3) update[i][j] = 1;

            } else {
                update[i][j] = 0;
            }
        }
        printw("\n");
    }
}

int alive_neighbour(int universe[height][width], int row, int column) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int indexI = (row + i + height) % height;
            int indexJ = (column + j + width) % width;
            if (universe[indexI][indexJ] == 1) count += 1;
        }
    }
    if (universe[row][column] == 1) count--;

    return count;
}

void output(int matrix[height][width]) {
    clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printw("%c", matrix[i][j] == 0 ? ' ' : '#');
        }

        printw("\n");
    }
}