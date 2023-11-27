#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    puts("Bienvenue dans le Shell ENSEA.");
    puts("Pour quitter, tapez 'exit'.");

    while (1) {
        fputs("enseash % ", stdout);                                //  Display the prompt
       
        char x[1024];
        fgets(x, sizeof(x), stdin);
        x[strcspn(x, "\n")] = '\0';                         // Remove the newline character



        if (strcmp(x, "exit") == 0) {                            // Check the command for exit
            puts("Au revoir !");
            break;
        }

        puts("Exécution de la commande: ");                         // Execute the command and display information
        int y= system(x);

        printf("Statut de sortie: %d\n", y);              // // Display the exit status
    }

    return 0;
}
