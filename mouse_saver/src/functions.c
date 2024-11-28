#include "functions.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * @brief Handles SIGINT (Ctrl+C) to safely close the output file and exit.
 */

void SignalHandler(int sig)
{
    fclose(output_file);
    fopen(output_file, rb);
    if (output_file && output_transformed_file)
    {
        rewind(output_file);             // Go back to the beginning of the raw data file
        rewind(output_transformed_file); // Go back to the beginning of the raw data file

        // Process each pair of raw coordinates and apply the scaling
        while (fread(&x, sizeof(float), 1, output_file) > 0 &&
               fread(&y, sizeof(float), 1, output_file) > 0)
        {
            // Apply scaling transformation to the raw coordinates
            int scaled_x = ((x - min_x) / (max_x - min_x)) * (TERM_WIDTH - 1);
            int scaled_y = ((y - min_y) / (max_y - min_y)) * (TERM_HEIGHT - 1);

            // Write the scaled coordinates to a new file
            fwrite(&scaled_x, sizeof(int), 1, output_transformed_file);
            fwrite(&scaled_y, sizeof(int), 1, output_transformed_file);

            // Debug print
            printf("Scaled: X=%d, Y=%d | MAX: X=%.2f, Y=%.2f | MIN: X=%.2f, Y=%.2f\n",
                   scaled_x, scaled_y, max_x, max_y, min_x, min_y);
        }

        // Close both files safely
        fclose(output_file);
        fclose(output_transformed_file);
        printf("\nFile closed safely. Exiting...\n");
        printf("Min X: %.2f, Max X: %.2f\n", min_x, max_x);
        printf("Min Y: %.2f, Max Y: %.2f\n", min_y, max_y);
    }
    exit(0);
}

/**
 * @brief Creates a symbolic link for the mouse data file in the mouse_plotter build directory.
 */
void CreateSymbolicLink()
{
    const char *sourcePath = "../../mouse_saver/build/mouse_data.dat";   // Source file in mouse_saver/build
    const char *targetPath = "../../mouse_plotter/build/mouse_data.dat"; // Symbolic link in mouse_plotter/build

    // Remove any existing symbolic link
    unlink(targetPath);

    // Create a new symbolic link
    if (symlink(sourcePath, targetPath) == -1)
    {
        perror("Error creating symbolic link");
    }
    else
    {
        printf("Symbolic link created: %s -> %s\n", targetPath, sourcePath);
    }
}
