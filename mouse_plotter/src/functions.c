#include "functions.h"

#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Define global variables here
FILE *input_file = NULL;
const char *file_path = "mouse_data.dat";  // Path to the binary file
int trail_x[TRAIL_LENGTH] = {0};           // Trail X positions
int trail_y[TRAIL_LENGTH] = {0};           // Trail Y positions
int trail_index = 0;                       // Current trail index
int x = 0, y = 0;                          // Coordinates from file

void DrawTrace(int signum) {
  fread(&x, sizeof(int), 1, input_file);
  fread(&y, sizeof(int), 1, input_file);

  // Plot the current position
  mvaddch(y, x, '*');
  refresh();

  // Clear trail character
  if (trail_index >= TRAIL_LENGTH) {
    int clear_x = trail_x[trail_index % TRAIL_LENGTH];
    int clear_y = trail_y[trail_index % TRAIL_LENGTH];
    mvaddch(clear_y, clear_x, ' ');  // Clear old character
  }

  // Update trail
  trail_x[trail_index % TRAIL_LENGTH] = x;
  trail_y[trail_index % TRAIL_LENGTH] = y;
  trail_index++;

  if (feof(input_file)) {
    mvprintw(0, 0, "End of data. Exiting...");
    refresh();
    sleep(1);
    endwin();
    fclose(input_file);
    exit(EXIT_SUCCESS);
  } else if (ferror(input_file)) {
    mvprintw(0, 0, "Error reading file.");
    refresh();
  }
}
