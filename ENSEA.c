#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_INPUT_SIZE 1024

// Function to print a welcome message in the shell
void printWelcomeMessage() {
    write(STDOUT_FILENO, "Welcome to the ENSEA Shell.\n", 29);
    write(STDOUT_FILENO, "To exit, type 'exit'.\n", 22);
}

// Function to print the shell prompt, including exit code, signal code, and elapsed time
void printPrompt(int exitCode, int signalCode, long elapsedTime) {
    if (WIFEXITED(exitCode)) {
        char exitMsg[50];
        // Format the exit message
        sprintf(exitMsg, "enseash [exit:%d|%ldms] %% ", WEXITSTATUS(exitCode), elapsedTime);
        // Write the exit message to STDOUT
        write(STDOUT_FILENO, exitMsg, strlen(exitMsg));
    } else if (WIFSIGNALED(exitCode)) {
        char signMsg[50];
        // Format the signal message
        sprintf(signMsg, "enseash [sign:%d|%ldms] %% ", WTERMSIG(exitCode), elapsedTime);
        // Write the signal message to STDOUT
        write(STDOUT_FILENO, signMsg, strlen(signMsg));
    } else {
        // Default prompt if no exit code or signal
        write(STDOUT_FILENO, "enseash % ", 11);
    }
}

// Function to measure the execution time of the command and execute the command with arguments
void measureAndExecute(char* args[]) {
    struct timespec start, end;
    long elapsedTime;

    // Record the start time using clock_gettime
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Fork a child process to execute the command
    int child_pid = fork();

    if (child_pid == 0) {
        // Child process: execute the command with arguments using execvp
        execvp(args[0], args);
        
        // Exit the child process with a failure code in case of an error
        perror("Error during command execution");
        exit(EXIT_FAILURE);
    } else if (child_pid > 0) {
        // Parent process
        int status;
        // Wait for the child process to finish and get the status
        waitpid(child_pid, &status, 0);

        // Record the end time using clock_gettime
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Calculate the elapsed time in milliseconds
        elapsedTime = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;

        // Print the shell prompt with exit code, signal code, and elapsed time
        printPrompt(status, 0, elapsedTime);
    } else {
        // Error handling for fork failure
        perror("Error creating child process");
        exit(EXIT_FAILURE);
    }
}
int main() {
    // Print the welcome message at the start
    printWelcomeMessage();

    while (1) {
        // Print the shell prompt with default values for exit code, signal code, and elapsed time
        printPrompt(0, 0, 0);

        // Read user input
        char input[MAX_INPUT_SIZE];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle end-of-file (Ctrl+D) scenario
            write(STDOUT_FILENO, "Bye bye (end of file)...\n", 26);
            break;
        }

        // Remove the newline character from the input
        input[strcspn(input, "\n")] = '\0';

        // Check for the exit command
        if (strcmp(input, "exit") == 0) {
            // Print a goodbye message and exit the loop
            write(STDOUT_FILENO, "Bye bye...\n", 11);
            break;
        }

        // Check if the input is not empty
        if (strlen(input) > 0) {
            // Tokenize the input into command and arguments
            char *tokens[MAX_INPUT_SIZE];
            int tokenCount = 0;

            // Tokenize the input using space as a delimiter
            char *token = strtok(input, " ");
            while (token != NULL && tokenCount < MAX_INPUT_SIZE - 1) {
                tokens[tokenCount++] = token;
                token = strtok(NULL, " ");
            }
            tokens[tokenCount] = NULL;  // Set the last element to NULL for execvp

            // Measure the time of command execution and execute the command with arguments
            measureAndExecute(tokens);
        }
    }

    return 0;
}



