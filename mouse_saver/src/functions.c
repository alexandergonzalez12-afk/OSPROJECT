#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

// External global variables
extern FILE *output_file;
extern volatile int timer_expired;

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

void SetupPosixTimer(timer_t *timerId, int intervalMs) {
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = timerId;

    if (timer_create(CLOCK_REALTIME, &sev, timerId) == -1) {
        perror("Error creating timer");
        exit(1);
    }

    struct itimerspec timerSpec;
    timerSpec.it_value.tv_sec = intervalMs / 1000;
    timerSpec.it_value.tv_nsec = (intervalMs % 1000) * 1000000;
    timerSpec.it_interval.tv_sec = intervalMs / 1000;
    timerSpec.it_interval.tv_nsec = (intervalMs % 1000) * 1000000;

    if (timer_settime(*timerId, 0, &timerSpec, NULL) == -1) {
        perror("Error setting timer");
        exit(1);
    }
}

void TimerHandler(int sig) {
    timer_expired = 1;
}

void CreateSymlink() {
    const char *sourcePath = "mouse_data.dat";
    const char *targetPath = "../../mouse_plotter/build/mouse_data.dat";

    unlink(targetPath);

    if (symlink(sourcePath, targetPath) == -1) {
        perror("Error creating symbolic link");
    } else {
        printf("Symbolic link created: %s -> %s\n", targetPath, sourcePath);
    }
}
