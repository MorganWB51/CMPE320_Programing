#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXCHAR 4096 // The max input size

struct SentenceTracker {
    char **sentences; // Hold each sentence as a string
    int count; // The number of sentences
};

int getNumberOfSentences(char *temp);
void storeSentences(char *temp, struct SentenceTracker *data);
void printResults(struct SentenceTracker data);

int main(void) {
    // initialize a tracker to hold basic information
    struct SentenceTracker data;

    // Allocates memory for the user input
    char *temp = calloc(sizeof(char) , MAXCHAR);
    fgets(temp, MAXCHAR,stdin);

    // Stores the number of sentences in the input
    data.count = getNumberOfSentences(temp);

    // Store the input into the tracker
    storeSentences(temp, &data);

    // Prints the result in the given format
    printResults(data);

    // Frees the memory used for the input
    free(temp);
    
    // Frees the memory used to store each sentence in the tracker
    for(int i =0; i < data.count; i++) {
        free(data.sentences[i]);
    }
    free(data.sentences);
}

 // Gets the number of periods
int getNumberOfSentences(char *temp) {
    int count = 0;
     for(int i = 0; i < MAXCHAR; i++) {
        if(temp[i] == '.') {
             count++;
         }
     }
     return count;
}

// Stores the given sentence into the trackers 2D array
void storeSentences(char *temp, struct SentenceTracker *data) {
    // allocates space and memory in for the 2d array
    data->sentences  = calloc(data->count, sizeof(char*));
    for(int i = 0; i < data->count; i++) {
       data->sentences[i] = malloc(sizeof(char)* MAXCHAR);
    }
    // Splits the input at the first '.'
    char *token = strtok(temp, ".");
    int i = 0;
    while(token != NULL && i < data->count) {
        // Removes the leading space after a period
        if(token[0] == ' ') {
            token++;
        }
        // Copies the string into the tracker
        strcpy(data->sentences[i], token);
       
        i++;
        // Goes to next '.'
        token = strtok(NULL, ".");
    }   
    
}

//Prints the output in the given format
void printResults(struct SentenceTracker data) {
    printf("num: %d\n", data.count); 
    for(int i = 0; i<data.count; i++) {
        printf("%d : (%s.)\n",i,data.sentences[i]);
    }
}

