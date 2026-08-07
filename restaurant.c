/* ---------------------------------------------------------
 * restaurant.c
 * Implementation of the Restaurant Linked List operations.
 * --------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "restaurant.h"

Restaurant *createRestaurant(int id, const char *name, const char *location, float rating) {
    Restaurant *r = (Restaurant *) malloc(sizeof(Restaurant));
    if (!r) {
        printf("Memory allocation failed for Restaurant.\n");
        exit(EXIT_FAILURE);
    }
    r->id = id;
    strncpy(r->name, name, NAME_LEN - 1);
    r->name[NAME_LEN - 1] = '\0';
    strncpy(r->location, location, LOC_LEN - 1);
    r->location[LOC_LEN - 1] = '\0';
    r->rating = rating;
    r->menu = NULL;
    r->next = NULL;
    return r;
}

void addRestaurant(Restaurant **head, Restaurant *newRest) {
    if (*head == NULL) {
        *head = newRest;
        return;
    }
    Restaurant *cur = *head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = newRest;
}

int deleteRestaurant(Restaurant **head, int id) {
    Restaurant *cur = *head;
    Restaurant *prev = NULL;

    while (cur != NULL) {
        if (cur->id == id) {
            if (prev == NULL) {
                *head = cur->next;
            } else {
                prev->next = cur->next;
            }
            freeFoodList(cur->menu);
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

int updateRestaurant(Restaurant *head, int id, const char *name, const char *location, float rating) {
    Restaurant *r = findRestaurantById(head, id);
    if (r == NULL) {
        return 0;
    }
    strncpy(r->name, name, NAME_LEN - 1);
    r->name[NAME_LEN - 1] = '\0';
    strncpy(r->location, location, LOC_LEN - 1);
    r->location[LOC_LEN - 1] = '\0';
    r->rating = rating;
    return 1;
}

Restaurant *findRestaurantById(Restaurant *head, int id) {
    Restaurant *cur = head;
    while (cur != NULL) {
        if (cur->id == id) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

Restaurant *findRestaurantByName(Restaurant *head, const char *name) {
    Restaurant *cur = head;
    while (cur != NULL) {
        if (strcasecmp(cur->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

int restaurantIdExists(Restaurant *head, int id) {
    return findRestaurantById(head, id) != NULL;
}

void printRestaurant(Restaurant *r) {
    if (r == NULL) return;
    printf("%-5d %-25s %-15s Rating: %.1f  (Items: %d)\n",
           r->id, r->name, r->location, r->rating, countFoods(r->menu));
}

void viewRestaurants(Restaurant *head) {
    if (head == NULL) {
        printf("No restaurants available.\n");
        return;
    }
    printf("%-5s %-25s %-15s %s\n", "ID", "Name", "Location", "Rating (Items)");
    printLine();
    Restaurant *cur = head;
    while (cur != NULL) {
        printRestaurant(cur);
        cur = cur->next;
    }
}

void freeRestaurantList(Restaurant *head) {
    Restaurant *cur = head;
    while (cur != NULL) {
        Restaurant *tmp = cur;
        cur = cur->next;
        freeFoodList(tmp->menu);
        free(tmp);
    }
}

int countRestaurants(Restaurant *head) {
    int count = 0;
    Restaurant *cur = head;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    return count;
}
