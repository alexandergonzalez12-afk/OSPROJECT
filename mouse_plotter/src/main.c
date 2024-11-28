/**
 * @file main.c
 * @brief Main function to initialize and start the mouse plotter.
 *
 * This function sets up the signal handler, configures and starts the POSIX
 * timer, initializes ncurses, and enters an infinite loop to wait for signals.
 *
 * @return int Returns 0 on successful execution, or 1 if an error occurs.
 *
 * @authors
 * Alexander J. Gonzalez Suarez
 * Alex J. Strubbe Martinez
 */

#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"

/**
 * @brief Main function to initialize and start the mouse plotter.
 *
 * This function sets up the signal handler, configures and starts the POSIX
 * timer, initializes ncurses, and enters an infinite loop to wait for signals.
 *
 * @return int Returns 0 on successful execution, or 1 if an error occurs.
 */
int main() {
  // Open the input binary file containing mouse coordinates for reading
  input_file = fopen(file_path, "rb");
  if (!input_file) {
    perror("Error opening binary file");  // Print error if file doesn't open
    return 1;                             // Return 1 on error
  }

  struct sigaction action;  ///< Structure for handling signals
  struct sigevent sev;      ///< Structure for timer event notification
  timer_t timer_id;         ///< Timer ID for the POSIX timer
  struct itimerspec
      timer;  ///< Timer specification for initial and interval settings

  // Setup signal handler to trigger DrawTrace when SIGALRM is received
  action.sa_handler = &DrawTrace;  // Set the handler function
  action.sa_flags = SA_RESTART;    // Flags to restart the system call
  if (sigaction(SIGALRM, &action, NULL) == -1) {  // Install the signal handler
    perror("sigaction");  // Print error if signal handler setup fails
    exit(EXIT_FAILURE);   // Exit if signal handler setup fails
  }

  // Configure the timer to send SIGALRM signal on expiration
  sev.sigev_notify = SIGEV_SIGNAL;  // Notify via signal
  sev.sigev_signo = SIGALRM;        // Signal to be sent
  sev.sigev_value.sival_ptr =
      &timer_id;  // Timer ID to associate with the signal

  // Create the POSIX timer using the real-time clock
  if (timer_create(CLOCK_REALTIME, &sev, &timer_id) == -1) {
    perror("timer_create");  // Print error if timer creation fails
    exit(EXIT_FAILURE);      // Exit if timer creation fails
  }

  // Configure the timer for initial expiration and interval
  timer.it_value.tv_sec = 0;          // Initial expiration in seconds
  timer.it_value.tv_nsec = 10000000;  // 10 milliseconds for initial expiration
  timer.it_interval.tv_sec = 0;       // Interval in seconds
  timer.it_interval.tv_nsec = 10000000;  // 10 milliseconds for the interval

  // Start the timer with the specified settings
  if (timer_settime(timer_id, 0, &timer, NULL) == -1) {
    perror("timer_settime");  // Print error if timer fails to start
    exit(EXIT_FAILURE);       // Exit if timer start fails
  }

  // Print a message to indicate the start of the plotter
  printf("Starting mouse plotter. Press Ctrl+C to stop.\n");
  sleep(1);  // Brief delay to allow the user to see the message

  // Initialize ncurses for terminal-based graphics
  initscr();
  cbreak();     // Disable line buffering
  noecho();     // Prevent input characters from being echoed to the screen
  curs_set(0);  // Hide the cursor to improve the display

  // Main loop to wait for signals indefinitely
  while (1) {
    pause();  // Wait for the signal (SIGALRM) to be triggered by the timer
  }

  // Clean up (though this part is not likely to be reached due to infinite
  // loop)
  endwin();            // End ncurses mode
  fclose(input_file);  // Close the input file
  return 0;            // Return 0 to indicate successful execution
}
