#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"

int main() {
  input_file = fopen(file_path, "rb");
  if (!input_file) {
    perror("Error opening binary file");
    return 1;
  }

  struct sigaction action;  ///< Structure for handling signals
  struct sigevent sev;      ///< Structure for timer event notification
  timer_t timer_id;         ///< Timer ID for the POSIX timer
  struct itimerspec
      timer;  ///< Timer specification for initial and interval settings

  // Setup signal handler
  action.sa_handler = &DrawTrace;
  action.sa_flags = SA_RESTART;
  if (sigaction(SIGALRM, &action, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  // Configure the timer
  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIGALRM;
  sev.sigev_value.sival_ptr = &timer_id;

  // Create the POSIX timer
  if (timer_create(CLOCK_REALTIME, &sev, &timer_id) == -1) {
    perror("timer_create");
    exit(EXIT_FAILURE);
  }

  // Configure the timer interval and initial expiration
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_nsec = 10000000;  // 10 milliseconds
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_nsec = 10000000;  // 10 milliseconds

  // Start the timer
  if (timer_settime(timer_id, 0, &timer, NULL) == -1) {
    perror("timer_settime");
    exit(EXIT_FAILURE);
  }

  // Print start message
  printf("Starting mouse plotter. Press Ctrl+C to stop.\n");
  sleep(1);  // Brief delay

  // Initialize ncurses
  initscr();
  cbreak();
  noecho();
  curs_set(0);  // Hide the cursor

  while (1) {
    pause();  // Wait for signals
  }

  // Clean up (this will likely not be reached due to infinite loop)
  endwin();
  fclose(input_file);
  return 0;
}
