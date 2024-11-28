#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

#define TRAIL_LENGTH 10  // Length of the trail effect

// Declare variables as extern
extern FILE *input_file;
extern const char *file_path;
extern int trail_x[TRAIL_LENGTH];
extern int trail_y[TRAIL_LENGTH];
extern int trail_index;
extern int x, y;

// Function declarations
void DrawTrace(int signum);

#endif  // FUNCTIONS_H
