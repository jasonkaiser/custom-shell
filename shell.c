#include "shell.h"

// Builds the shell prompt string with username, hostname, and current directory
void get_prompt(char *prompt) {
    char hostname[HOST_NAME_MAX];
    char *username;
    char cwd[PATH_MAX];	

    gethostname(hostname, sizeof(hostname));  // Get system hostname
    username = getenv("USER");                // Try to get username from environment

    // If getenv fails, fallback to getpwuid
    if (!username) {
        struct passwd *pw = getpwuid(getuid());
        username = pw->pw_name;
    }
    
    getcwd(cwd, sizeof(cwd)); // Get current working directory

    // Format and store the prompt string with color codes
    snprintf(prompt, SHELL_MAX_INPUT, 
             "\033[1;35m[OS-P Shell]\033[0m "  
             "\033[1;32m%s@%s\033[0m:"           
             "\033[1;36m%.800s\033[0m"             
             "\033[1;33m$\033[0m ",          
             username, hostname, cwd);
}

// Parses and executes a command line input
void execute_command(char *input) {
    char *args[SHELL_MAX_INPUT];
    int i = 0;
    char *token = strtok(input, " ");
    char *outfile = NULL;
    int append = 0;

    // Tokenize the input and handle output redirection
    while (token != NULL) {
        if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0) {
            append = (strcmp(token, ">>") == 0); 
            token = strtok(NULL, " ");            
            if (token == NULL) {
                printf("Syntax error: expected filename after %s\n", append ? ">>" : ">");
                return;
            }
            outfile = token;  // Save output file name
            break;            // Stop collecting args after redirection
        }
        args[i++] = token;    // Store command arguments
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // Null-terminate the arguments array

    if (i == 0) return;  // Empty input, do nothing

    // Handle built-in 'rm' command
    if (strcmp(args[0], "rm") == 0) {
        if (args[1] == NULL) {
            printf("Usage: rm [-f] <filename>\n");
        } else if (strcmp(args[1], "-f") == 0 && args[2] != NULL) {
            unlink(args[2]);  // Force remove
        } else {
            if (remove(args[1]) != 0) {
                perror("rm failed");
            }
        }
        return;
    }

    // Handle 'cowsay' command
    if (strcmp(args[0], "cowsay") == 0) {
        if (args[1] == NULL) {
            printf("Usage cowsay [-r] message\n");
        } else if (strcmp(args[1], "-r") == 0 && args[2] != NULL) {
            // Angry random face
            printf("   < %s >\n", args[2]);
            printf("     \\    (=' _ '=)\n");
            printf("      \\   (o.o   )\n");
            printf("          ( > < )\n");
        } else {
            // Normal cow
            printf("   <%s >\n", args[1]);
            printf("    \\   ^__^\n");
            printf("     \\   (oo)\\______\n ");
            printf("        (__)\\       )\\/\\\n");
            printf("           ||------w |\n");
            printf("           ||        ||\n");
        }
        return;
    }

    // Clear terminal screen
    if (strcmp(args[0], "clear") == 0) {
        printf("\033[H\033[J");
        return;
    }

    // Display file contents
    if (strcmp(args[0], "cat") == 0) {
        if (args[1] == NULL) {
            printf("Usage: cat <filename>\n");
        } else {
            FILE *file = fopen(args[1], "r");
            if (!file) {
                perror("cat failed");
                return;
            }

            char line[MAX_INPUT];
            while (fgets(line, sizeof(line), file)) {
                printf("%s", line);
            }
            fclose(file);
        }
        return;
    }

    // Kill a process using its PID
    if (strcmp(args[0], "kill") == 0) {
        if (args[1] == NULL) {
            printf("Usage: kill <PID>\n");
            return;
        }
        pid_t pid = atoi(args[1]);
        if (kill(pid, SIGKILL) == 0) {
            printf("Process %d killed successfully\n", pid);
        } else {
            perror("kill failed");
        }
        return;
    }

    // Fork and execute external command
    if (fork() == 0) {
        // Handle output redirection if specified
        if (outfile) {
            int fd = open(outfile, append ? (O_WRONLY | O_CREAT | O_APPEND)
                                          : (O_WRONLY | O_CREAT | O_TRUNC),
                          0644);
            if (fd == -1) {
                perror("Error opening file");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);  // Redirect stdout to file
            close(fd);
        }

        execvp(args[0], args);  // Execute command
        perror("execvp failed"); // If exec fails
        exit(1);
    } else {
        wait(NULL); // Parent waits for child to finish
    }
}

int main(void) {
    char input[SHELL_MAX_INPUT];
    char prompt[SHELL_MAX_INPUT];
   
    while (1) {
        get_prompt(prompt);           // Build prompt
        printf("%s", prompt);         // Display prompt

        if (!fgets(input, SHELL_MAX_INPUT, stdin)) {
            printf("\n");
            break;                    // Exit on Ctrl+D
        }

        input[strcspn(input, "\n")] = 0;  // Remove newline
        if (strcmp(input, "exit") == 0) break;  // Exit on 'exit'

        execute_command(input);      // Run the command
    }

    return 0;
}
