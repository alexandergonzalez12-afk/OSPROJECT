#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

extern FILE *output_file;

void HandleSigint(int sig) {
    if (output_file) {
        fclose(output_file);
        printf("\nFile closed safely. Exiting...\n");
    }
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
        fclose(file);
    }

    FILE *file = fopen(filePath, "ab");
    if (!file) {
        perror("Error opening file for appending");
        exit(1);
    }

    return file;
}

dispatch_source_t SetupGCDTimer(int intervalMs, void (^handler)(void)) {
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
    dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, 0), intervalMs * NSEC_PER_MSEC, 0);
    dispatch_source_set_event_handler(timer, handler);
    dispatch_resume(timer);
    return timer;
}
