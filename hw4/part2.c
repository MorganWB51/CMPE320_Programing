/**
 * @author Morgan Williams-Burrell
 * This file contains part 2 of OS homework 4 on threads
 * This program will make a thread that display its stake size
 * man pthread_attr_init
 */
#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/**
 * records the current system
 */
struct timespec recordTime(){
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time;
}

/**
 * Calculated the different between to points in time and prints that differnece in usec
 * @param start - the start time
 * @param end - the end time
 */
unsigned long timeElapsed(struct timespec start, struct timespec end) {
    unsigned long startTime = 1000000 * start.tv_sec + start.tv_nsec;
    unsigned long endTime = 1000000 * end.tv_sec + end.tv_nsec;
    return endTime - startTime;
}

/**
 * the function that a thread runs
 * Displays the stack size of the thread
 * @param ptr - a void pointer the get casted to a workunit
 */
void* threadStart(void *ptr){
    size_t size;
    pthread_attr_t gattr;
    pthread_getattr_np(pthread_self(),&gattr);
    pthread_attr_getstacksize(&gattr,&size);
    printf("My stack is %ld\n",size);
    return NULL;
}

/**
 * Makes a thread with no defind attr
 */
void launchDefaultThread(){
    struct timespec start, stop;
    start = recordTime();
    pthread_t threadID;
    pthread_create(&threadID, NULL, threadStart,NULL);
    pthread_join(threadID, NULL);
    stop = recordTime();
    printf("Default thread %ld usecs\n", timeElapsed(start,stop));
}

/**
 * Makes a thread with a with a stack size of 131072
 */
void launchCustomThread() {
    struct timespec start, stop;
    start = recordTime();
    pthread_t thread2;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 131072 );
    pthread_create(&thread2, &attr, threadStart,NULL);
    pthread_join(thread2, NULL);
    stop = recordTime();
    printf("Small stack thread %ld usecs\n", timeElapsed(start,stop));
}


int main(int argc, char **argv) {
    launchDefaultThread();
    launchCustomThread();
}