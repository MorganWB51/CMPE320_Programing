#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 

// the parent process that send strings into the pipe
void parent(int fd[2]) {
    char buffer [50];
    
    close(fd[0]); //closes the read end of the pipe

    sprintf(buffer,"hello world\n"); // adds statement to buffer
    write(fd[1], buffer,12); // writes the statement to the write end of the pipe

    sprintf(buffer,"Watson, come quickly\n"); // adds statement to buffer
    write(fd[1], buffer,21 ); // writes the statement to the write end of the pipe

    sprintf(buffer,"it worked!\n"); // adds statement to buffer
    write(fd[1], buffer, 11); // writes the statement to the write end of the pipe

    close(fd[1]);
}

// the child process that runs the cat command
void child(int fd[2]) {
    close(fd[1]); //closes the write end of the pipe
    close(0); //closes stdin
    dup(fd[0]); //dups the read end of the pipe
    int rc;
    rc = execlp("/bin/cat", "/bin/cat", NULL); // runs the cat command
    fprintf(stderr,"Error during execution\n"); // if an error occurs, print to stder
}


int main(int arg, char ** argv){
    // char buffer [50];
    int status;
    int fd[2];
    pipe(fd);
    pid_t pid = fork( ); // creates a child process
    if (pid == 0) {
        child(fd); // runs the child process
    } else {
        parent(fd); // runs the parent process
        waitpid(pid, &status, 0); // waits for the childe to finish
        printf("Child exited: %d\n", status);
    }
    exit(0);
}
