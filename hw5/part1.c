/**
 * @author Morgan Williams-Burrell
 * This file contains part 1 of OS homework 5
 * Ping will lock while it works and signal pong when it is done
 * The program will alternate between printing ping and pong equal to the amount of RALLY times
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#define RALLY 10 // the term used for hitting the ball back and forth in ping pong

/**
 * A struct that holds the a shared int, a semaphore, and mutex
 */
struct work{
    int i;
    sem_t sem;
    pthread_mutex_t lock;
    pthread_cond_t cond;
};

/**
 * the function that a thread runs
 * waits for the pong function to start, lockes the mutex, prints and increments i, then signals pong 
 * @param ptr - a void pointer the get casted to a work
 */
void* ping(void *ptr){
    struct work *work = (struct work *)ptr;
    
    while(work->i < RALLY) {
    sem_wait(&work->sem);
    pthread_mutex_lock(&work->lock);
    work->i++;
    printf("PING: %d\n", work->i);
    pthread_mutex_unlock(&work->lock);
    pthread_cond_signal(&work->cond);
    }

    return NULL;
}

/**
 * the function that a thread runs
 * waits for ping to change i then prints i
 * @param ptr - a void pointer the get casted to a work
 */
void* pong(void *ptr){
    struct work *work = (struct work *)ptr;
    while(work->i < RALLY) {
    sem_post(&work->sem);
    pthread_cond_wait(&work->cond,&work->lock);
    printf("PONG: %d\n", work->i);
    }
    return NULL;
}

/**
 * waits to all thread to finshin running
 * @param ThreadID - an array contain all thread ids
 */
void waitForThread(pthread_t threadID[]){
    for (int i = 0; i < 2; i++) {
        pthread_join(threadID[i],NULL);
    }
}

/**
 * creates and run two thread that run ping and pong
 * @param ThreadID - an array contain all thread ids
 */ 
void launchThread(pthread_t threadID[]){

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    //define the work unit for both threads
    static struct work work;
    work.i=0;
    sem_init(&work.sem,0,0);
    pthread_mutex_init(&work.lock,NULL);
    pthread_cond_init(&work.cond,NULL);

    //run and wait for the threads
    pthread_create(&threadID[0], &attr, ping, &work);
    pthread_create(&threadID[1], &attr, pong, &work);
}

int main(){
    pthread_t threadID[2];
    launchThread(threadID);
    waitForThread(threadID);
}