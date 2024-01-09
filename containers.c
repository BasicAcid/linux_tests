#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE (1024 * 1024)

char container_stack[STACK_SIZE];

int
container_function()
{
    printf("Container - Inside the containerized process!\n");

    // Run bash.
    execlp("/bin/bash", "/bin/bash", NULL);

    // If execlp fails.
    perror("execlp");
    exit(EXIT_FAILURE);
}

int
main()
{
    printf("Outside the container\n");

    // Create a new namespace with clone.
    int container_pid = clone(container_function, container_stack + STACK_SIZE,
                              CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);

    // Check if
    if(container_pid == -1)
    {
        perror("clone");
        exit(EXIT_FAILURE);
    }

    // Wait for the container process to finish.
    waitpid(container_pid, NULL, 0);

    printf("Container process exited.\n");

    return 0;
}
