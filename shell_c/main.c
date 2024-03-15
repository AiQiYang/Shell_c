#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define BUFSIZE 1024
#define TOK_BUFSIZE 64

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
    const char *delim = "\t\n\r";
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
    return tokens;
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


    } while (status);
}

int main(int argc, char **argv) {
    shell_loop();

    return EXIT_SUCCESS;
}