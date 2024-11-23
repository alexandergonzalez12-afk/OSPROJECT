#include "functions.h"
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern FILE *output_file;       // Declare external reference to the output file
extern float min_x, max_x;      // External reference for min/max X values
extern float min_y, max_y;      // External reference for min/max Y values

void HandleSigint(int sig) {
    if (output_file) {
        fclose(output_file);
        printf("\nFile closed safely. Exiting...\n");
    }
    printf("Min X: %.2f, Max X: %.2f\n", min_x, max_x);
    printf("Min Y: %.2f, Max Y: %.2f\n");
    exit(0);
}

FILE *EnsureFileExists(const char *filePath) {
    if (access(filePath, F_OK) == -1) {
        printf("File '%s' does not exist. Creating it...\n", filePath);
        FILE *file = fopen(filePath, "wb");
        if (!file) {
            perror("Error creating file");
            exit(1);
        }
        fclose(file);  // Close immediately after creating it
    }

    // Open the file for appending data
    FILE *file = fopen(filePath, "ab");
    if (!file) {
        perror("Error opening file for appending");
        exit(1);
    }

    return file;
}

void CreateSymlink() {
    const char *sourcePath = "mouse_data.dat";                // Source file in mouse_saver/build
    const char *targetPath = "../../mouse_plotter/build/mouse_data.dat"; // Symbolic link in mouse_plotter/build

    // Remove any existing symbolic link
    unlink(targetPath);

    // Create a new symbolic link
    if (symlink(sourcePath, targetPath) == -1) {
        perror("Error creating symbolic link");
    } else {
        printf("Symbolic link created: %s -> %s\n", targetPath, sourcePath);
    }
}
