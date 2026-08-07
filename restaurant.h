#ifndef RESTAURANT_H
#define RESTAURANT_H

#include "utility.h"
#include "menu.h"

/* ---------------------------------------------------------
 * restaurant.h
 * Restaurants are stored in a Linked List. Each Restaurant
 * owns its own Food Linked List (its menu).
 * --------------------------------------------------------- */

typedef struct Restaurant {
    int id;
    char name[NAME_LEN];
    char location[LOC_LEN];   /* must match a node name in the delivery graph */
    float rating;             /* 0.0 - 5.0 */
    Food *menu;                /* head of this restaurant's food linked list */
    struct Restaurant *next;
} Restaurant;

/* Create a new restaurant node (not linked yet) */
Restaurant *createRestaurant(int id, const char *name, const char *location, float rating);

/* Insert restaurant at the end of the list */
void addRestaurant(Restaurant **head, Restaurant *newRest);

/* Delete a restaurant (and its whole menu) by id */
int deleteRestaurant(Restaurant **head, int id);

/* Update restaurant's basic details */
int updateRestaurant(Restaurant *head, int id, const char *name, const char *location, float rating);

/* Find restaurant by id (linear search) */
Restaurant *findRestaurantById(Restaurant *head, int id);

/* Find restaurant by name (linear search, case-insensitive) */
Restaurant *findRestaurantByName(Restaurant *head, const char *name);

/* Check whether a restaurant id already exists */
int restaurantIdExists(Restaurant *head, int id);

/* Print one restaurant's summary line */
void printRestaurant(Restaurant *r);

/* Print all restaurants */
void viewRestaurants(Restaurant *head);

/* Free the whole restaurant list, including each restaurant's menu */
void freeRestaurantList(Restaurant *head);

/* Count number of restaurants */
int countRestaurants(Restaurant *head);

#endif /* RESTAURANT_H */
