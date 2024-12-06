#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 100

int main() {
    int pipe1[2]; 
    int pipe2[2];

    pid_t pid;
    char parentMessage[BUFFER_SIZE] = "Msg from parent!";
    char childMessage[BUFFER_SIZE] = "Msg from child!";
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    // Create the pipes
    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    pid = fork();
   

    if (pid == 0) { 
        close(pipe1[1]);
        close(pipe2[0]);

        // Read message from parent
        bytesRead = read(pipe1[0], buffer, BUFFER_SIZE);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            printf("Child received: %s\n", buffer);
        }
        
        // Send message to parent
        write(pipe2[1], childMessage, strlen(childMessage) + 1);

        close(pipe1[0]);
        close(pipe2[1]);
        exit(EXIT_SUCCESS);
    } else { // Parent process
        close(pipe1[0]); 
        close(pipe2[1]); 

        // Send message to child
        write(pipe1[1], parentMessage, strlen(parentMessage) + 1);
        
        // Read message from child
        bytesRead = read(pipe2[0], buffer, BUFFER_SIZE);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            printf("Parent received: %s\n", buffer);
        }

        close(pipe1[1]);
        close(pipe2[0]);
        wait(NULL); 
    }

    return 0;
}

