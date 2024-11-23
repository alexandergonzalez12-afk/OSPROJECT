#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "functions.h"
#include <time.h>

// Global variables for tracking file and mouse movements
FILE *output_file = NULL;
volatile int timer_expired = 0;

int main() {
    const char *device = "/dev/input/mice";  // Path to the mouse device
    int fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("Error opening mouse device");
        return 1;
    }

    float x = 50.0, y = 12.5;
    int term_width = 100, term_height = 25;

    output_file = EnsureFileExists("mouse_data.dat");

    signal(SIGINT, HandleSigint);

    CreateSymlink();

    timer_t timerId;
    SetupPosixTimer(&timerId, 10);  // Timer interval: 10ms

    signed char data[3];
    printf("Capturing mouse movements. Press Ctrl+C to stop.\n");

    while (1) {
        if (timer_expired) {
            timer_expired = 0;

            if (read(fd, data, sizeof(data)) > 0) {
                int dx = data[1];
                int dy = data[2];

                x += dx;
                y -= dy;

                if (x < 0) x = 0;
                if (x > term_width) x = term_width;
                if (y < 0) y = 0;
                if (y > term_height) y = term_height;

                int x_int = (int)x;
                int y_int = (int)y;
                fwrite(&x_int, sizeof(int), 1, output_file);
                fwrite(&y_int, sizeof(int), 1, output_file);

                printf("Saved: X=%d, Y=%d\n", x_int, y_int);
            }
        }
    }

    close(fd);
    return 0;
}
