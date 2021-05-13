#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// The signal handler for the death of the child process
void handler(int arg) {
    printf("** CHILD DIED %d**\n", arg);
}

// the child process that runs the ls command
void child() {
   execlp("/bin/ls", "/bin/ls", "-l", "/usr/bin/", NULL); // runs the ls command in "usr/bin"
   fprintf(stderr,"Error during execution\n"); // if an error occurs, print to stderr
   return;
}

int main(int arg, char ** argv){
    int status;
    pid_t pid = fork(); // creates a child process
    signal(SIGCHLD, handler); // listens for the SIGNCHLD signal
    if (pid == 0) {
        child(); // runs the child process
    } else {
        waitpid(pid, &status, 0); // waits for the child to finish
        printf("Child exited: %d\n", status);
    }
    exit(0);
}