#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

enum argtype {
    WORD, DASH
};
// holds a word, its type, and its list
struct argument {
    enum argtype type;
    char *contents;
    struct list_head list;
};

// Adds a node to the front on the list
void list_add(struct list_head *new, struct list_head *head) {
    struct list_head *next = head->next;
    next->prev = new;
    new->next = next;

    new->prev = head;
    head->next = new;
}

// Adds a node to the end list
void list_add_tail(struct list_head *new, struct list_head *head) {
    struct list_head *next = head->prev;
    next->next = new;
    new->prev = next;

    new->next = head;
    head->prev = new;
}

// Deletes a node from the list
void list_del(struct list_head *entry) {
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;
}

// Returns if the list is empty
int list_empty(struct list_head *head) {
    return head->next == head;
}

// Joins two lists by insert list immediately after the head
void list_splice(struct list_head *list, struct list_head *head) {
    struct list_head *prev = head;
    struct list_head *next = head->next;
    struct list_head *first = list->next;
    struct list_head *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

//prints the list
void print_list(struct list_head *list) {
    struct list_head *curr;
    struct argument *entry;
    for (curr = list->next; curr != list; curr = curr->next) {
            entry = list_entry(curr, struct argument, list);
            printf("%s\n", entry->contents);
        }
}

int main(int argc, char **argv) {
    int i;
    // Creates the two lists
    LIST_HEAD(list_words);
    LIST_HEAD(list_dashes);

    // Takes the input and store it into the proper list
    for(i = 1; i < argc; i++) {
        struct argument *arg = malloc(sizeof(struct argument));
        arg->contents = strdup(argv[i]);
        if (argv[i][0] == '-') {
            arg->type = DASH;
            // Store into list_dashes
            list_add(&arg->list, &list_dashes);
        }
        else {
            arg->type = WORD;
            // Store into list_
            list_add(&arg->list, &list_words);
        }
    }

   

    // Prints the list with dashes
    if (!list_empty(&list_dashes)) {
        printf("Arguments with dashes\n");
        print_list(&list_dashes);
    }

    // Prints the list with just words
    if (!list_empty(&list_words)){ 
        printf("Arguments with words\n");
        print_list(&list_words);
    }

}
