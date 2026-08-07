#ifndef MENU_H
#define MENU_H

#include "utility.h"

/* ---------------------------------------------------------
 * menu.h
 * Each Restaurant owns a Linked List of Food items (its menu).
 * This module implements insertion, deletion, update, search
 * and traversal of that Food linked list.
 * --------------------------------------------------------- */

typedef struct Food {
    int id;
    char name[NAME_LEN];
    char category[CATEGORY_LEN];
    float price;
    struct Food *next;
} Food;

/* Create a new food node (not linked yet) */
Food *createFood(int id, const char *name, const char *category, float price);

/* Insert a food item at the end of a restaurant's menu (head passed by ref) */
void addFood(Food **head, Food *newFood);

/* Delete a food item by id from the menu */
int deleteFood(Food **head, int id);

/* Update an existing food item's details */
int updateFood(Food *head, int id, const char *name, const char *category, float price);

/* Find a food item by id */
Food *findFoodById(Food *head, int id);

/* Find a food item by name (linear search, case-insensitive) */
Food *findFoodByName(Food *head, const char *name);

/* Check whether a food id already exists in this menu */
int foodIdExists(Food *head, int id);

/* Print a single food's details */
void printFood(Food *f);

/* Print the whole menu of a restaurant */
void viewMenu(Food *head);

/* Free the entire food linked list */
void freeFoodList(Food *head);

/* Count number of items in the food list */
int countFoods(Food *head);

#endif /* MENU_H */
