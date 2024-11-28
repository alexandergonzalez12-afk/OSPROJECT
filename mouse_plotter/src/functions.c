/**
 * @file functions.c
 * @brief This file contains the implementation of the function that draws the
 * mouse trail.
 *
 * The program reads coordinates from a binary file and displays the mouse trail
 * in a terminal using ncurses. The trail follows the mouse's movements, and it
 * clears the previous position after the trail reaches a certain length.
 *
 * This code was created by Alexander J. Gonzalez Suarez and Alex J. Strubbe
 * Martinez.
 */

#include "functions.h"

#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Global variables for storing mouse data and trail information
FILE *input_file =
    NULL;  // File pointer to the input binary file containing mouse data
const char *file_path =
    "mouse_data.dat";  // Path to the binary file containing mouse data

int trail_x[TRAIL_LENGTH] = {
    0};  // Array to store X coordinates for the mouse trail
int trail_y[TRAIL_LENGTH] = {
    0};               // Array to store Y coordinates for the mouse trail
int trail_index = 0;  // Current index in the trail arrays

int x = 0, y = 0;  // Coordinates read from the input file

/**
 * @brief Signal handler function to read mouse data and update the display.
 *
 * This function is triggered by a signal (such as a timer). It reads the next
 * set of coordinates from the input file and updates the terminal display with
 * the new mouse position. It also manages the trail of previous positions by
 * clearing the oldest position and drawing the new one.
 *
 * @param signum The signal number that triggered the handler.
 */
void DrawTrace(int signum) {
  // Read the next set of coordinates from the binary file
  fread(&x, sizeof(int), 1, input_file);
  fread(&y, sizeof(int), 1, input_file);

  // Plot the current position using ncurses
  mvaddch(y, x, '*');
  refresh();  // Update the screen

  // Clear the character at the previous position in the trail (if applicable)
  if (trail_index >= TRAIL_LENGTH) {
    int clear_x =
        trail_x[trail_index %
                TRAIL_LENGTH];  // Get the oldest X position in the trail
    int clear_y =
        trail_y[trail_index %
                TRAIL_LENGTH];  // Get the oldest Y position in the trail
    mvaddch(clear_y, clear_x,
            ' ');  // Clear the old position by placing a blank space
  }

  // Update the trail with the current mouse position
  trail_x[trail_index % TRAIL_LENGTH] =
      x;  // Store the current X position in the trail array
  trail_y[trail_index % TRAIL_LENGTH] =
      y;          // Store the current Y position in the trail array
  trail_index++;  // Move to the next index in the trail array

  // Check if we've reached the end of the file
  if (feof(input_file)) {
    mvprintw(0, 0,
             "End of data. Exiting...");  // Display a message on the screen
    refresh();
    sleep(1);
    endwin();            // End ncurses mode
    fclose(input_file);  // Close the input file
    exit(EXIT_SUCCESS);  // Exit the program successfully
  } else if (ferror(input_file)) {
    mvprintw(0, 0, "Error reading file.");  // Display an error message if
                                            // there's an issue reading the file
    refresh();
  }
}
