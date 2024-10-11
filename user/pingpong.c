// user/pingpong.c
#include "user.h"

int
main(void)
{
    int p1[2]; // Pipe from parent to child
    int p2[2]; // Pipe from child to parent
    char* transferMessage = "Hello, world!"; // Message to send
    int messageLength = strlen(transferMessage);  
    char buffer[messageLength];

    // Create pipes
    pipe(p1);
    pipe(p2);

    int pid = fork();

    if (pid < 0) {
        // Fork failed
        fprintf(2, "Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Read message from parent (transfer through pipe 1) and load into buffer
        read(p1[0], buffer, messageLength);
        printf("%d: received ping\n", getpid());
        printf("%d: received message '%s' from parent\n", getpid(), buffer);

        // Send message back to parent (transfer through pipe 2)
        write(p2[1], transferMessage, messageLength);
        exit(0);
    } else {
        // Parent process
        // Send message to child (transfer through pipe 1)
        write(p1[1], transferMessage, messageLength);

        // Wait for child to send message back (transfer through pipe 2) and load into buffer
        read(p2[0], buffer, messageLength);
        printf("%d: received pong\n", getpid());
        printf("%d: received message '%s' from child\n", getpid(), buffer);
        // Wait for the child process to finish
        wait(0);
        exit(0);
    }
}
