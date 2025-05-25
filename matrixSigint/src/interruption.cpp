#include <signal.h>
#include <stdio.h>
#include "interruption.h"
#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

extern volatile sig_atomic_t currentI;
extern volatile sig_atomic_t currentJ;

extern volatile sig_atomic_t sigintReceived;

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

void signalHandler(int signum) {
    if (signum == SIGINT && !sigintReceived) {
        sigintReceived = 1;

        char *buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
        if (!buffer) {
            return;
        }

        int length = snprintf(buffer, BUFFER_SIZE, "\nSIGINT received at [i = %d][j = %d]\n", currentI, currentJ);
        write(STDOUT_FILENO, buffer, length);

        signal(SIGINT, SIG_DFL);
    }
}

void setSignalHandler() {
    signal(SIGINT, signalHandler);
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

void sigactionHandler(int signum, siginfo_t *info, void *context) {
    if (signum == SIGINT && !sigintReceived) {
        sigintReceived = 1;

        char *buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
        if (!buffer) {
            return;
        }

        int length = snprintf(buffer, BUFFER_SIZE, "\nSIGINT received at [i = %d][j = %d]\n", currentI, currentJ);
        write(STDOUT_FILENO, buffer, length);

        struct sigaction sa = {};
        
        sa.sa_handler = SIG_DFL;
        sa.sa_flags = 0;

        sigemptyset(&sa.sa_mask);
        sigaction(SIGINT, &sa, NULL);
    }
}


void setSigactionHandler() {
    struct sigaction sa = {};

    sa.sa_sigaction = sigactionHandler;
    sa.sa_flags = SA_SIGINFO;

    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //