#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>


#define WIDTH 30
#define HEIGHT 30


typedef char World[HEIGHT][WIDTH];

// ANSI color codes
#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"

// Array of colors for generations
const char* colors[] = {COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN};


#define COLOR_COUNT (sizeof(colors)/sizeof(colors[0]))


// ------------------- Terminal Input Non-blocking -------------------
void enable_nonblocking_input() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);

    ttystate.c_lflag &= ~ICANON; //buffered input
    ttystate.c_lflag &= ~ECHO;   // print / echo
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // non-blocking
}


int kbhit() {
    int ch = getchar();
    if (ch != EOF) return ch;
    return 0;
}


// ------------------- World Functions -------------------
void clear_screen() {
    printf("\033[H\033[J");
}

void draw(World world, int generation) {
    clear_screen();
    const char* color = colors[generation % COLOR_COUNT];
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (world[y][x])
                printf("%s@ " COLOR_RESET, color);
            else
                printf("  ");
        }
        printf("\n");
    }
}

void evolution(World world) {
    World new_world;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int lives = 0;
            for (int dy = -1; dy <= 1; dy++)
                for (int dx = -1; dx <= 1; dx++)
                    if (world[(y + dy + HEIGHT) % HEIGHT][(x + dx + WIDTH) % WIDTH])
                        lives++;
            if (world[y][x]) lives--;
            new_world[y][x] = (lives == 3 || (lives == 2 && world[y][x]));
        }
    }
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            world[y][x] = new_world[y][x];
}

void init_world(World world) {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            world[y][x] = rand() < RAND_MAX / 10 ? 1 : 0;
}






