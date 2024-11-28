#include "functions.h"

#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


/**
 * @file functions.c
/**
 * @brief File pointer to the input binary file containing mouse data.
 */
FILE *input_file = NULL;

/**
 * @brief Path to the binary file containing mouse data.
 */
const char *file_path = "mouse_data.dat";  // Path to the binary file

/**
 * @brief Array to store the X positions of the trail.
 */
int trail_x[TRAIL_LENGTH] = {0};           // Trail X positions

/**
 * @brief Array to store the Y positions of the trail.
 */
int trail_y[TRAIL_LENGTH] = {0};           // Trail Y positions

/**
 * @brief Current index in the trail arrays.
 */
int trail_index = 0;                       // Current trail index

/**
 * @brief Coordinates read from the file.
 */
int x = 0, y = 0;                          // Coordinates from file

/**
 * @brief Signal handler function to read mouse data and update the display.
 * 
 * This function is called when a signal is received. It reads the next set of coordinates
 * from the input file, updates the display with the new position, and manages the trail of
 * previous positions.
 * 
 * @param signum The signal number that triggered the handler.
 */
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