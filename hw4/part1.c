/**
 * @author Morgan Williams-Burrell
 * This file contains part 1 of OS homework 4 on threads
 * thread function will take a single integer argument, n the number of seconds to sleep.
 * After sleeping for n seconds, the thread will print “Hello from n seconds in the past”.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 3

/**
 * A structed that hole the amount of time the thread with sleep
 */
struct workunit {
    int sleepTime;
};

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
void timeElapsed(struct timespec start, struct timespec end) {
    unsigned long startTime = 1000000 * start.tv_sec + start.tv_nsec;
    unsigned long endTime = 1000000 * end.tv_sec + end.tv_nsec;
    printf("It took %lu usecs\n", endTime - startTime);
}

/**
 * the function that a thread runs
 * Puts the thread to sleep for a time equal to a workunits sleepTime
 * @param ptr - a void pointer the get casted to a workunit
 */
void* threadStart(void *ptr){
    struct workunit *work = (struct workunit *)ptr;
    sleep(work->sleepTime);
    printf("Hello from %d seconds in the past\n", work->sleepTime);
    return NULL;
}

/**
 * waits to all thread to finshin running
 * @param ThreadID - an array contain all thread ids
 */
void waitForThread(pthread_t threadID[]){

     for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threadID[i],NULL);
     }
}

/**
 * Makes threads equal to the value  of NUM_THREADS
 */
void launchThread(){
    pthread_t threadID[NUM_THREADS];
    struct workunit work[NUM_THREADS];

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for(int i = 0; i < NUM_THREADS; i++) {
        work[i].sleepTime = i+1;
        pthread_create(&threadID[i], &attr, threadStart, &work[i]);
    }
    waitForThread(threadID);

}

/** 
 * records the processes start and end time
 */
int main(int argc, char **argv) {
    struct timespec start = recordTime();
    launchThread();
    struct timespec end = recordTime();
    timeElapsed(start,end);
}