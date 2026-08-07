/* ---------------------------------------------------------
 * menu.c
 * Implementation of the Food menu Linked List operations.
 * --------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "menu.h"

Food *createFood(int id, const char *name, const char *category, float price) {
    Food *f = (Food *) malloc(sizeof(Food));
    if (!f) {
        printf("Memory allocation failed for Food.\n");
        exit(EXIT_FAILURE);
    }
    f->id = id;
    strncpy(f->name, name, NAME_LEN - 1);
    f->name[NAME_LEN - 1] = '\0';
    strncpy(f->category, category, CATEGORY_LEN - 1);
    f->category[CATEGORY_LEN - 1] = '\0';
    f->price = price;
    f->next = NULL;
    return f;
}

void addFood(Food **head, Food *newFood) {
    if (*head == NULL) {
        *head = newFood;
        return;
    }
    Food *cur = *head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = newFood;
}

int deleteFood(Food **head, int id) {
    Food *cur = *head;
    Food *prev = NULL;

    while (cur != NULL) {
        if (cur->id == id) {
            if (prev == NULL) {
                *head = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

int updateFood(Food *head, int id, const char *name, const char *category, float price) {
    Food *f = findFoodById(head, id);
    if (f == NULL) {
        return 0;
    }
    strncpy(f->name, name, NAME_LEN - 1);
    f->name[NAME_LEN - 1] = '\0';
    strncpy(f->category, category, CATEGORY_LEN - 1);
    f->category[CATEGORY_LEN - 1] = '\0';
    f->price = price;
    return 1;
}

Food *findFoodById(Food *head, int id) {
    Food *cur = head;
    while (cur != NULL) {
        if (cur->id == id) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

Food *findFoodByName(Food *head, const char *name) {
    Food *cur = head;
    while (cur != NULL) {
        if (strcasecmp(cur->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

int foodIdExists(Food *head, int id) {
    return findFoodById(head, id) != NULL;
}

void printFood(Food *f) {
    if (f == NULL) return;
    printf("%-5d %-25s %-15s Tk. %.2f\n", f->id, f->name, f->category, f->price);
}

void viewMenu(Food *head) {
    if (head == NULL) {
        printf("No food items available in this menu.\n");
        return;
    }
    printf("%-5s %-25s %-15s %s\n", "ID", "Name", "Category", "Price");
    printLine();
    Food *cur = head;
    while (cur != NULL) {
        printFood(cur);
        cur = cur->next;
    }
}

void freeFoodList(Food *head) {
    Food *cur = head;
    while (cur != NULL) {
        Food *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

int countFoods(Food *head) {
    int count = 0;
    Food *cur = head;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    return count;
}
