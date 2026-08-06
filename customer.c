/* ---------------------------------------------------------
 * customer.c
 * Implementation of the Customer Linked List operations.
 * --------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer.h"

Customer *createCustomer(int id, const char *username, const char *password,
                          const char *name, const char *phone, const char *location) {
    Customer *c = (Customer *) malloc(sizeof(Customer));
    if (!c) {
        printf("Memory allocation failed for Customer.\n");
        exit(EXIT_FAILURE);
    }
    c->id = id;
    strncpy(c->username, username, USER_LEN - 1); c->username[USER_LEN - 1] = '\0';
    strncpy(c->password, password, PASS_LEN - 1); c->password[PASS_LEN - 1] = '\0';
    strncpy(c->name, name, NAME_LEN - 1); c->name[NAME_LEN - 1] = '\0';
    strncpy(c->phone, phone, PHONE_LEN - 1); c->phone[PHONE_LEN - 1] = '\0';
    strncpy(c->location, location, LOC_LEN - 1); c->location[LOC_LEN - 1] = '\0';
    c->next = NULL;
    return c;
}

void addCustomer(Customer **head, Customer *newCust) {
    if (*head == NULL) {
        *head = newCust;
        return;
    }
    Customer *cur = *head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = newCust;
}

int deleteCustomer(Customer **head, int id) {
    Customer *cur = *head;
    Customer *prev = NULL;

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

Customer *findCustomerById(Customer *head, int id) {
    Customer *cur = head;
    while (cur != NULL) {
        if (cur->id == id) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

Customer *findCustomerByUsername(Customer *head, const char *username) {
    /* Linear Search as required by the login module */
    Customer *cur = head;
    while (cur != NULL) {
        if (strcmp(cur->username, username) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

int usernameExists(Customer *head, const char *username) {
    return findCustomerByUsername(head, username) != NULL;
}

int customerIdExists(Customer *head, int id) {
    return findCustomerById(head, id) != NULL;
}

void printCustomer(Customer *c) {
    if (c == NULL) return;
    printf("%-5d %-15s %-20s %-15s %s\n", c->id, c->username, c->name, c->phone, c->location);
}

void viewCustomers(Customer *head) {
    if (head == NULL) {
        printf("No customers registered yet.\n");
        return;
    }
    printf("%-5s %-15s %-20s %-15s %s\n", "ID", "Username", "Name", "Phone", "Location");
    printLine();
    Customer *cur = head;
    while (cur != NULL) {
        printCustomer(cur);
        cur = cur->next;
    }
}

void freeCustomerList(Customer *head) {
    Customer *cur = head;
    while (cur != NULL) {
        Customer *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

int countCustomers(Customer *head) {
    int count = 0;
    Customer *cur = head;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    return count;
}
