/* ---------------------------------------------------------
 * login.c
 * Implementation of Admin login, Customer registration and
 * Customer login.
 * --------------------------------------------------------- */

#include <stdio.h>
#include <string.h>
#include "login.h"

int adminLogin(void) {
    char username[USER_LEN];
    char password[PASS_LEN];

    getStringInput("Admin Username: ", username, USER_LEN);
    getStringInput("Admin Password: ", password, PASS_LEN);

    if (strcmp(username, "Kaushik") == 0 &&
        strcmp(password, "Summer2026") == 0) {

        printf("\nAdmin login successful!\n");
        return 1;
    }

    printf("\nIncorrect admin username or password.\n");
    return 0;
}

Customer *customerRegister(Customer **head, Graph *graph, int *nextIdCounter) {
    char username[USER_LEN];
    char password[PASS_LEN];
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char location[LOC_LEN];

    getStringInput("Choose a Username: ", username, USER_LEN);

    /* Duplicate username check using Linear Search */
    if (usernameExists(*head, username)) {
        printf("\nThat username is already taken. Please try again.\n");
        return NULL;
    }

    getStringInput("Choose a Password: ", password, PASS_LEN);
    getStringInput("Full Name: ", name, NAME_LEN);
    getStringInput("Phone Number: ", phone, PHONE_LEN);

    printAllLocations(graph);
    getStringInput("Your Delivery Location (must match one above): ", location, LOC_LEN);

    if (!isValidLocation(graph, location)) {
        printf("\nThat location is not part of our delivery network. Registration cancelled.\n");
        return NULL;
    }

    Customer *c = createCustomer(*nextIdCounter, username, password, name, phone, location);
    addCustomer(head, c);
    (*nextIdCounter)++;

    printf("\nRegistration successful! You can now log in as \"%s\".\n", username);
    return c;
}

Customer *customerLoginPrompt(Customer *head) {
    char username[USER_LEN];
    char password[PASS_LEN];

    getStringInput("Username: ", username, USER_LEN);
    getStringInput("Password: ", password, PASS_LEN);

    /* Linear Search for the username, then validate password */
    Customer *c = findCustomerByUsername(head, username);
    if (c != NULL && strcmp(c->password, password) == 0) {
        printf("\nWelcome back, %s!\n", c->name);
        return c;
    }
    printf("\nIncorrect username or password.\n");
    return NULL;
}
