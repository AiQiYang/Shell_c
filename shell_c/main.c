#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFSIZE 1024
#define TOK_BUFSIZE 64

int lsh_cd(char **command);
// int lsh_help(char **command);
int lsh_exit(char **command);

// Build in function
char *build_in_name[] = {
    "cd",
    // "help",
    "exit"
};

int (*build_in_func[])(char **) = {
    lsh_cd,
    // lsh_help,
    lsh_exit
};

int lsh_cd(char **command) {
    if (command[0] == NULL) {
        
    } else {
        if (chdir(command[1]) != 0) {
            perror("Error: cd");
        }
    }
    return 1;
}

// int lsh_help(char **command) {

// }
int lsh_exit(char **command) {
    return 0;
}

char *shell_read_line () {
    size_t bufSize = 0;
    char *buffer = malloc(sizeof(char) * BUFSIZE);

    ssize_t readLine = getline(&buffer, &bufSize, stdin);
    if (readLine == -1) {
        perror("Error reading line");
    }
    return buffer;
}

char **split_line(char *line) {
    size_t bufSize = TOK_BUFSIZE;
    char **tokens = malloc(bufSize * (sizeof(char *)));
    char *token;
    const char *delim = " \t\n\a";
    token = strtok(line, delim);
    int index = 0;
    while (token != NULL) {
        tokens[index++] = token;

        if (index > bufSize) {
            bufSize += bufSize;
            tokens = realloc(tokens, bufSize * (sizeof(char *)));
            if (!tokens) {
                perror("allocation error");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, delim);
    }
    // for (int i = 0; i < index; i++) {
    //     printf("%s ", tokens[i]);
    // }
    tokens[index] = NULL;
    return tokens;
}

int shell_launch(char **command) {
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(command[0], command) == -1) {
            perror(("Error: execvp()"));
        } else if (pid < 0) {
            perror("Error: pid");
        } else {
            waitpid(pid, &status, 0);
        }
    }
    return 1;
}

int shell_execute(char **command) {
    int build_in_name_len = sizeof(build_in_name) / sizeof(char *);
    for (int i = 0; i < build_in_name_len; i++) {
        if (strcmp(command[0], build_in_name[i]) == 0) {
            return (*build_in_func[i])(command);
        }
    }
    return shell_launch(command);
}

void shell_loop() {
    int status = 1;
    char* line;
    char **command;
    do {
        // Read line
        printf("$ ");

        line = shell_read_line();
        
        command = split_line(line);

        status = shell_execute(command);

    } while (status);
}

int main(int argc, char **argv) {
    shell_loop();
    return EXIT_SUCCESS;
}