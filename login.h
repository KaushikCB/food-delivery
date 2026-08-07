#ifndef LOGIN_H
#define LOGIN_H

#include "customer.h"
#include "graph.h"

/* ---------------------------------------------------------
 * login.h
 * Handles Admin login, Customer registration and Customer
 * login. Credentials are validated using Linear Search over
 * the Customer linked list (see customer.c).
 * --------------------------------------------------------- */

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

/* Prompts for admin username/password, returns 1 if correct */
int adminLogin(void);

/* Registers a new customer; returns pointer to the created
 * Customer on success, or NULL if the username already exists
 * or the entered location is not part of the delivery graph. */
Customer *customerRegister(Customer **head, Graph *graph, int *nextIdCounter);

/* Prompts for username/password and validates against the list.
 * Returns pointer to the logged in Customer, or NULL on failure. */
Customer *customerLoginPrompt(Customer *head);

#endif /* LOGIN_H */
