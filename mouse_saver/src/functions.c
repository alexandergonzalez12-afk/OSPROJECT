/**
 * @file functions.c
 * @brief This file contains helper functions for the mouse_saver program.
 *
 * This file includes functions for handling signals,creating symbolic links,
 * and processing mouse data transformations.
 *
 * This code was created by Alexander J. Gonzalez Suarez and Alex J. Strubbe
 * Martinez.
 */

#include "functions.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Handles SIGINT (Ctrl+C) signal to safely close the output files and
 * exit.
 *
 * @param sig The signal number.
 */
void SignalHandler(int sig) {
  fclose(output_file);  // Close the file that stores raw mouse coordinates
  fopen("mouse_raw_data.dat", "rb");  // Open file in binary read mode (it’s
                                      // already opened in write mode in main)
  if (output_file && output_transformed_file) {
    rewind(output_file);  // Go back to the beginning of the raw data file
    rewind(output_transformed_file);  // Go back to the beginning of the
                                      // transformed data file (These rewinds
                                      // are just for assurance)

    // Process each pair of raw coordinates and apply scaling
    while (fread(&x, sizeof(float), 1, output_file) > 0 &&
           fread(&y, sizeof(float), 1, output_file) > 0) {
      // Apply scaling transformation to the raw coordinates
      int scaled_x = ((x - min_x) / (max_x - min_x)) * (TERM_WIDTH - 1) *
                     .80;  // Scale X to terminal width
      int scaled_y = ((y - min_y) / (max_y - min_y)) * (TERM_HEIGHT - 1) *
                     .80;  // Scale Y to terminal height

      // Write the transformed coordinates to the output file
      fwrite(&scaled_x, sizeof(int), 1,
             output_transformed_file);  // Write scaled X to file
      fwrite(&scaled_y, sizeof(int), 1,
             output_transformed_file);  // Write scaled Y to file

      // Debug print showing the transformation result
      printf("Scaled: X=%d, Y=%d | MAX: X=%.2f, Y=%.2f | MIN: X=%.2f, Y=%.2f\n",
             scaled_x, scaled_y, max_x, max_y, min_x, min_y);
    }

    // Close both files safely after writing
    fclose(output_file);
    fclose(output_transformed_file);
    printf("\nFile closed safely. Exiting...\n");
    printf("Min X: %.2f, Max X: %.2f\n", min_x, max_x);
    printf("Min Y: %.2f, Max Y: %.2f\n", min_y, max_y);
  }
  exit(0);  // Exit the program after handling the signal
}

/**
 * @brief Creates a symbolic link for the mouse data file in the mouse_plotter
 * build directory.
 */
void CreateSymbolicLink() {
  const char *sourcePath =
      "../../mouse_saver/build/mouse_data.dat";  // Source file from mouse_saver
                                                 // build directory
  const char *targetPath =
      "../../mouse_plotter/build/mouse_data.dat";  // Symbolic link target in
                                                   // mouse_plotter build
                                                   // directory

  // Remove any existing symbolic link if it exists
  unlink(targetPath);

  // Create a new symbolic link from the source to the target path
  if (symlink(sourcePath, targetPath) == -1) {
    perror("Error creating symbolic link");  // If symlink creation fails, print
                                             // an error
  } else {
    printf("Symbolic link created: %s -> %s\n", targetPath,
           sourcePath);  // Confirmation of symlink creation
  }
}
