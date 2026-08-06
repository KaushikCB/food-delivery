#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "utility.h"

/* ---------------------------------------------------------
 * customer.h
 * Customers are stored in a Linked List.
 * --------------------------------------------------------- */

typedef struct Customer {
    int id;
    char username[USER_LEN];
    char password[PASS_LEN];
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char location[LOC_LEN];   /* must match a node name in the delivery graph */
    struct Customer *next;
} Customer;

Customer *createCustomer(int id, const char *username, const char *password,
                          const char *name, const char *phone, const char *location);

void addCustomer(Customer **head, Customer *newCust);

int deleteCustomer(Customer **head, int id);

Customer *findCustomerById(Customer *head, int id);

/* Linear search by username - used for login validation */
Customer *findCustomerByUsername(Customer *head, const char *username);

int usernameExists(Customer *head, const char *username);

int customerIdExists(Customer *head, int id);

void printCustomer(Customer *c);

void viewCustomers(Customer *head);

void freeCustomerList(Customer *head);

int countCustomers(Customer *head);

#endif /* CUSTOMER_H */
