#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 

// the child process that runs the ls command
void child() {
    close(1); //closes stdout
    open("output.txt",O_RDWR | O_CREAT, 0777); // opens a new output file making it if not already
    int rc;
    rc = execlp("/bin/ls", "/bin/ls", "-l", "/usr/bin/", NULL); // runs the ls command in "usr/bin"
    fprintf(stderr,"Error during execution\n"); // if an error occurs, print to stderr
    return;
}


int main(int arg, char ** argv){
    int status;
    pid_t pid = fork( ); // creates a child process
    if (pid == 0) {
        child(); // runs the child process
    } else {
        waitpid(pid, &status, 0); // waits for the child to finish
        printf("Child exited: %d\n", status);
    }
    exit(0);
}
