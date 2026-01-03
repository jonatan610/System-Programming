//YEHONATAN_SHAPIRA_ID_208969246
#include <stdio.h>    
#include <stdlib.h>    
#include <string.h>    
#include <unistd.h>     
#include <sys/wait.h>  
#include <errno.h>      

#define MAX_INPUT 1024
#define MAX_ARGS 100

// --------------------------------------------------
// Print the shell prompt
// --------------------------------------------------
void print_prompt() {
    printf("bash-mini$ ");
    fflush(stdout);
}

// --------------------------------------------------
// Split the user input into command and arguments
// --------------------------------------------------
void parse_input(char* input, char** args) {
    int i = 0;
    args[i] = strtok(input, " \t\n");

    while (args[i] != NULL && i < MAX_ARGS - 1) {
        i++;
        args[i] = strtok(NULL, " \t\n");
    }

    args[i] = NULL;
}

// --------------------------------------------------
// Check if the command is built-in (cd or exit)
// --------------------------------------------------
int is_internal_command(char* command) {
    return (strcmp(command, "cd") == 0 || strcmp(command, "exit") == 0);
}

// --------------------------------------------------
// Handle built-in commands: 'cd' and 'exit'
// --------------------------------------------------
void handle_internal_command(char** args) {
    // Handle 'exit' command
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    // Handle 'cd' command
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            // No argument -> go to HOME directory
            char* home = getenv("HOME");
            if (home != NULL) {
                if (chdir(home) != 0) {
                    perror("cd");
                }
            }
        } else {
            // Change to specified directory
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
    }
}

// --------------------------------------------------
// Check if a file is executable
// --------------------------------------------------
int is_executable(const char* path) {
    return access(path, X_OK) == 0;
}

// --------------------------------------------------
// Search for the command in $HOME and then /bin
// --------------------------------------------------
char* find_command_path(char* command) {
    static char full_path[1024];

    // Try in $HOME
    char* home = getenv("HOME");
    if (home != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", home, command);
        if (is_executable(full_path)) return full_path;
    }

    // Try in /bin
    snprintf(full_path, sizeof(full_path), "/bin/%s", command);
    if (is_executable(full_path)) return full_path;

    return NULL; // Command not found
}

// --------------------------------------------------
// Execute an external (non-built-in) command
// --------------------------------------------------
void execute_external_command(char** args) {
    char* path = find_command_path(args[0]);

    if (path == NULL) {
        fprintf(stderr, "[%s]: Unknown Command\n", args[0]);
        return;
    }

    pid_t pid = fork();  // Create a new process

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        // Child process runs the command
        execv(path, args);
        // If exec fails
        perror("exec");
        exit(errno);
    } else {
        // Parent process waits for child to finish
        int status;
        waitpid(pid, &status, 0);
        printf("Command finished with return code: %d\n", WEXITSTATUS(status));
    }
}

// --------------------------------------------------
// Main program loop
// --------------------------------------------------
int main() {
    char input[MAX_INPUT];
    char* args[MAX_ARGS];

    while (1) {
        print_prompt();

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break; // Exit on Ctrl+D
        }

        // Skip empty lines
        if (strcmp(input, "\n") == 0) continue;

        // Split input into command and arguments
        parse_input(input, args);

        if (args[0] == NULL) continue;

        // Handle internal commands
        if (is_internal_command(args[0])) {
            handle_internal_command(args);
        } else {
            // Handle external commands
            execute_external_command(args);
        }
    }

    return 0;
}
