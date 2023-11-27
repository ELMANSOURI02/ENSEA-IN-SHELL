#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024

void printWelcomeMessage() {
    write(STDOUT_FILENO, "Bienvenue dans le Shell ENSEA.\n", 30);
    write(STDOUT_FILENO, "Pour quitter, tapez 'exit'.\n", 28);
}

void printPrompt() {
    write(STDOUT_FILENO, "enseash % ", 11);
}

int main() {
    printWelcomeMessage();

    while (1) {
        printPrompt();

        char input[MAX_INPUT_SIZE];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("Erreur de lecture de l'entrée");
            exit(EXIT_FAILURE);
        }

        input[strcspn(input, "\n")] = '\0';  // delete the new carater of the ligne

        if (strcmp(input, "exit") == 0) {
            write(STDOUT_FILENO, "Au revoir !\n", 12);
            break;
        }

        if (strlen(input) > 0) {
            int child_pid = fork();

            if (child_pid == 0) {
                //  process of the Child
                execlp(input, input, (char *)NULL);
                exit(EXIT_FAILURE);
            } else if (child_pid > 0) {
                // process of the Parent
                int status;
                waitpid(child_pid, &status, 0);

            } else {
                perror("Erreur lors de la création du processus fils");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
