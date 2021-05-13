#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// store the dimensions of a rectangle
struct dimensions {
    double length;
    double width;
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

 // Deletes a node from the list
void list_del(struct list_head *entry) {
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;
}

// Returns if the list is empty
int list_empty(struct list_head *head) {
    return head->next == head;
}

//prints the area of each rectangel
void print_area(struct list_head *list) {
    struct list_head *curr;
    struct dimensions *entry;

    for (curr = list->next; curr != list; curr = curr->next) {
            entry = list_entry(curr, struct dimensions, list);
            printf("area %.3lf * %.3lf = %.3lf\n", entry->length, entry->width, entry->length * entry->width);
        }
}
// prints the total area of all rectangel
void print_total(struct list_head *list) {
    struct list_head *curr;
    struct dimensions *entry;
    double total = 0;
    for (curr = list->next; curr != list; curr = curr->next) {
            entry = list_entry(curr, struct dimensions, list);
            total += entry->length * entry->width;
    } 
    printf("Total area: %.3lf\n", total);
}

// removes any rectangel with an area under 10
void remove_small(struct list_head *list) {
    printf("Removing small rectangles\n");

    struct list_head *curr;
    struct dimensions *entry;
    double area = 0;
    curr = list->next;
    while (curr != list){
            entry = list_entry(curr, struct dimensions, list);
            area = entry->length * entry->width;
            curr = curr->next;
            // If the area is less than 10 it is removed from the list
            if (area < 10) {
                list_del(&entry->list);
                free(entry);
            }
    }
 
}

// cleans up the code 
void clear_list(struct list_head *list) {
    struct dimensions *entry;
    while (!list_empty(list)) {
        entry = list_entry(list->next, struct dimensions, list);
        list_del(&entry->list);
        free(entry);
    }
}

int main(int argc, char **argv) {
    double length;
    double width;
    LIST_HEAD(input);
    while(1) {
    // Enter two doubles for input until either is 0
    printf("Enter a length and width: ");
    scanf("%lf %lf", &length, &width);
    if (width == 0 || length == 0) {
        break;
    }

    struct dimensions *arg = malloc(sizeof(struct dimensions));
    arg->length = length;
    arg->width = width;
    // Adds the dimenstions to the list of inputs
    list_add(&arg->list, &input);
    }

    // Prints the area and then the total area
    print_area(&input);
    print_total(&input);
    // Remove any areas under 10
    remove_small(&input);
    // Prints the area and then the total area
    print_area(&input);
    print_total(&input);
    // Clean up the memoery used for the list
    clear_list(&input);

}
