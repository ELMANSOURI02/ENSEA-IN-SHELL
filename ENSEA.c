#include <sys/wait.h>
#include <stdio.h>
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
            // If fgets returns NULL, the user either used <Ctrl>+d or there is an error.
            write(STDOUT_FILENO, "Bye bye...\n", 11);
            break;
        }

        input[strcspn(input, "\n")] = '\0';  // delete the new caractere of the ligne 

        if (strcmp(input, "exit") == 0) {
            write(STDOUT_FILENO, "Bye bye...\n", 11);
            break;
        }

        if (strlen(input) > 0) {
            int child_pid = fork();

            if (child_pid == 0) {
                // the Processus of the enfant
                execlp(input, input, (char *)NULL);
                perror("Erreur lors de l'exécution de la commande");
                exit(EXIT_FAILURE);
            } else if (child_pid > 0) {
                // the Processus of the parent
                int status;
                waitpid(child_pid, &status, 0);

                char statusMessage[50];
                snprintf(statusMessage, sizeof(statusMessage), "Statut de sortie: %d\n", status);
                write(STDOUT_FILENO, statusMessage, strlen(statusMessage));
            } else {
                perror("Erreur lors de la création du processus fils");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
