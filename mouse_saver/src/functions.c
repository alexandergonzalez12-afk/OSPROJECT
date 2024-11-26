#include "functions.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * @brief Ensures the binary file exists. If not, it creates it and opens it for appending.
 * @param filePath The path to the binary file.
 * @return FILE* Pointer to the opened file.
 */
FILE *EnsureFileExists(const char *filePath)
{
    if (access(filePath, F_OK) == -1)
    {
        printf("File '%s' does not exist. Creating it...\n", filePath);
        FILE *file = fopen(filePath, "wb");
        if (!file)
        {
            perror("Error creating file");
            exit(1);
        }
        fclose(file); // Close immediately after creating it
    }

    // Open the file for appending data
    FILE *file = fopen(filePath, "rb+");
    if (!file)
    {
        perror("Error opening file for appending");
        exit(1);
    }

    return file;
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
