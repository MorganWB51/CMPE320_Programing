#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

enum argtype {
    WORD, DASH
};

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

}

// Deletes a node from the list
void list_del(struct list_head *entry) {
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;
}

// Returns if the list is empty
int list_empty(struct list_head *head) {
   
    return head->next != head;
}

// Joins two lists by insert list immediately after the head
void list_splice(struct list_head *list, struct list_head *head) {

}

//
void clear_list(struct list_head *list) {
    struct argument *entry;
    while (! list_empty(list)) {
        entry = list_entry(list->next, struct argument, list);
        list_del(&entry->list);
        free(entry->contents);
        free(entry);
    }
}

/
int main(int argc, char **argv)
{
    int i;
    LIST_HEAD(list_words);
    LIST_HEAD(list_dashes);

    for(i = 1; i < argc; i++) {
        struct argument *arg = malloc(sizeof(struct argument));
        arg->contents = strdup(argv[i]);
        if (argv[i][0] == '-') {
            arg->type = DASH;
            list_add(&arg->list, &list_dashes);
        }
        else {
            arg->type = WORD;
            list_add(&arg->list, &list_words);
        }
    }
struct list_head *curr;
struct argument *entry;



if (list_empty(list_dashes.next)) {
    printf("Arguments with dashes\n");
    for (curr = list_dashes.next; curr != &list_dashes; curr = curr->next) {
        entry = list_entry(curr, struct argument, list);
        printf("%s\n", entry->contents);
    }
}

if (list_empty(list_words.next)){ 
    printf("Arguments with words\n");
    for (curr = list_words.next; curr != &list_words; curr = curr->next) {
        entry = list_entry(curr, struct argument, list);
        printf("%s\n", entry->contents);
    }
}


}
