/* ---------------------------------------------------------
 * main.c
 * Console-Based Food Delivery Management System
 * Data Structure Lab Project
 *
 * Ties together every module: Login, Customer, Restaurant,
 * Menu, Cart, Order, Queue, Stack, Sorting, Graph (Dijkstra)
 * and File persistence.
 * --------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"
#include "login.h"
#include "customer.h"
#include "restaurant.h"
#include "menu.h"
#include "cart.h"
#include "order.h"
#include "admin.h"
#include "queue.h"
#include "stack.h"
#include "sorting.h"
#include "graph.h"
#include "file.h"


/* ---------------------------------------------------------
 * Seeds sample restaurants and food items the first time
 * the program runs.
 * --------------------------------------------------------- */
static Restaurant *seedSampleData(int *nextRestId, int *nextFoodId)
{
    Restaurant *head = NULL;

    Restaurant *r1 = createRestaurant(
        (*nextRestId)++,
        "KFC Banani",
        "Banani",
        4.5f
    );

    addFood(
        &r1->menu,
        createFood(
            (*nextFoodId)++,
            "Zinger Burger",
            "Burger",
            320.0f
        )
    );

    addFood(
        &r1->menu,
        createFood(
            (*nextFoodId)++,
            "Fried Chicken Bucket",
            "Chicken",
            850.0f
        )
    );

    addFood(
        &r1->menu,
        createFood(
            (*nextFoodId)++,
            "French Fries",
            "Sides",
            150.0f
        )
    );

    addRestaurant(&head, r1);


    Restaurant *r2 = createRestaurant(
        (*nextRestId)++,
        "Pizza Hut Gulshan",
        "Gulshan",
        4.2f
    );

    addFood(
        &r2->menu,
        createFood(
            (*nextFoodId)++,
            "Margherita Pizza",
            "Pizza",
            650.0f
        )
    );

    addFood(
        &r2->menu,
        createFood(
            (*nextFoodId)++,
            "Pepperoni Pizza",
            "Pizza",
            750.0f
        )
    );

    addFood(
        &r2->menu,
        createFood(
            (*nextFoodId)++,
            "Garlic Bread",
            "Sides",
            200.0f
        )
    );

    addRestaurant(&head, r2);


    Restaurant *r3 = createRestaurant(
        (*nextRestId)++,
        "Star Kabab Mohakhali",
        "Mohakhali",
        4.0f
    );

    addFood(
        &r3->menu,
        createFood(
            (*nextFoodId)++,
            "Beef Kabab",
            "Kabab",
            280.0f
        )
    );

    addFood(
        &r3->menu,
        createFood(
            (*nextFoodId)++,
            "Chicken Biriyani",
            "Rice",
            220.0f
        )
    );

    addRestaurant(&head, r3);

    saveRestaurants(head);

    return head;
}


/* ---------------------------------------------------------
 * Checkout Flow
 *
 * Calculates delivery route using Dijkstra, selects payment
 * method, creates an order and adds it to Pending Queue.
 * --------------------------------------------------------- */
static void checkoutFlow(
    CartItem **cart,
    Customer *customer,
    Restaurant *restHead,
    Graph *graph,
    Queue *pending,
    int *nextOrderId
)
{
    if (*cart == NULL) {
        printf("Your cart is empty. Add some food first!\n");
        return;
    }

    viewCart(*cart);

    Restaurant *r =
        findRestaurantById(restHead, (*cart)->restaurantId);

    if (r == NULL) {
        printf("Something went wrong: restaurant not found.\n");
        return;
    }

    /* Calculate shortest delivery path using Dijkstra */
    PathResult route =
        dijkstraShortestPath(
            graph,
            r->location,
            customer->location
        );

    printf("\n--- Delivery Route ---\n");

    printf("Restaurant:\n%s\n\n", r->name);

    printf("Customer:\n%s\n\n", customer->name);

    printPathResult(graph, &route);

    if (!route.found) {
        printf(
            "\nSorry, delivery is not currently possible "
            "to your location.\n"
        );

        return;
    }


    /* -----------------------------------------------------
     * Payment Method
     * ----------------------------------------------------- */

    printf(
        "\nPayment Methods:\n"
        "1. Cash on Delivery\n"
        "2. Card\n"
        "3. bKash\n"
        "4. Nagad\n"
    );

    int pay = getIntInput("Choose payment method: ");

    const char *paymentNames[] = {
        "Cash on Delivery",
        "Card",
        "bKash",
        "Nagad"
    };

    const char *paymentMethod;

    if (pay >= 1 && pay <= 4) {
        paymentMethod = paymentNames[pay - 1];
    } else {
        paymentMethod = "Cash on Delivery";
    }


    /* -----------------------------------------------------
     * Create Order
     * ----------------------------------------------------- */

    Order o;

    memset(&o, 0, sizeof(Order));

    o.orderId = (*nextOrderId)++;

    o.customerId = customer->id;

    strncpy(
        o.customerUsername,
        customer->username,
        USER_LEN - 1
    );

    o.restaurantId = r->id;

    strncpy(
        o.restaurantName,
        r->name,
        NAME_LEN - 1
    );

    strncpy(
        o.customerLocation,
        customer->location,
        LOC_LEN - 1
    );

    buildCartSummary(
        *cart,
        o.itemsSummary,
        MAX_ORDER_LINE
    );

    o.totalAmount = calculateCartTotal(*cart);

    strncpy(
        o.paymentMethod,
        paymentMethod,
        19
    );

    o.status = PENDING;

    o.distanceKm = route.totalDistance;

    o.estimatedTimeMin = route.totalTime;


    /* -----------------------------------------------------
     * Build route string:
     *
     * A -> B -> C
     * ----------------------------------------------------- */

    o.route[0] = '\0';

    int i;

    for (i = 0; i < route.pathLength; i++) {

        strncat(
            o.route,
            graph->names[route.pathNodes[i]],
            PATH_LEN - strlen(o.route) - 1
        );

        if (i != route.pathLength - 1) {

            strncat(
                o.route,
                " -> ",
                PATH_LEN - strlen(o.route) - 1
            );
        }
    }

    getCurrentDateTime(
        o.dateTime,
        DATE_LEN
    );


    /* Add order to Pending Queue */
    enqueue(pending, o);

    /* Empty cart after successful checkout */
    freeCart(cart);


    printf(
        "\nOrder placed successfully! "
        "Your Order ID is #%d.\n",
        o.orderId
    );

    printf(
        "Total Paid: Tk. %.2f via %s\n",
        o.totalAmount,
        paymentMethod
    );
}


/* ---------------------------------------------------------
 * Customer Dashboard
 * --------------------------------------------------------- */
static void customerDashboard(
    Customer *customer,
    Customer **customerHead,
    Restaurant *restHead,
    Graph *graph,
    Queue *pending,
    Stack *history,
    int *nextOrderId
)
{
    CartItem *cart = NULL;

    int choice;

    do {

        printf(
            "\n===================== "
            "Customer Dashboard "
            "=====================\n"
        );

        printf(
            "Logged in as: %s (%s)\n",
            customer->name,
            customer->location
        );

        printf("1. View Restaurants\n");
        printf("2. Search Restaurant\n");
        printf("3. Search Food\n");
        printf("4. View Menu\n");
        printf("5. Cart\n");
        printf("6. Order History\n");
        printf("7. Delete Account\n");
        printf("8. Logout\n");

        choice = getIntInput("Choose an option: ");


        switch (choice) {

            /* -------------------------------------------------
             * 1. View Restaurants
             * ------------------------------------------------- */
            case 1: {

                printf("\n--- Sort Restaurants By ---\n");

                printf(
                    "1. Name (A-Z) [Bubble Sort]\n"
                );

                printf(
                    "2. Rating High->Low [Selection Sort]\n"
                );

                printf(
                    "3. Rating Low->High [Merge Sort]\n"
                );

                printf("4. No sorting\n");

                int sortChoice =
                    getIntInput("Choose: ");


                Restaurant *arr[MAX_SORT_ITEMS];

                int n =
                    restaurantsToArray(
                        restHead,
                        arr,
                        MAX_SORT_ITEMS
                    );


                if (sortChoice == 1) {

                    bubbleSortRestaurantsByName(
                        arr,
                        n
                    );

                } else if (sortChoice == 2) {

                    selectionSortRestaurantsByRating(
                        arr,
                        n,
                        1
                    );

                } else if (sortChoice == 3) {

                    merKaNCgLvMEXxNzMxj2F7FYi1AdRrTo6Nhu(
                        arr,
                        n,
                        0
                    );
                }


                if (n == 0) {

                    printf(
                        "No restaurants available.\n"
                    );

                } else {

                    printf(
                        "%-5s %-25s %-15s %s\n",
                        "ID",
                        "Name",
                        "Location",
                        "Rating (Items)"
                    );

                    printLine();

                    int i;

                    for (i = 0; i < n; i++) {

                        printRestaurant(arr[i]);
                    }
                }

                break;
            }


            /* -------------------------------------------------
             * 2. Search Restaurant
             * ------------------------------------------------- */
            case 2: {

                char name[NAME_LEN];

                getStringInput(
                    "Restaurant name to search: ",
                    name,
                    NAME_LEN
                );

                /* Linear Search */
                Restaurant *r =
                    findRestaurantByName(
                        restHead,
                        name
                    );

                if (r != NULL) {

                    printRestaurant(r);

                } else {

                    printf(
                        "No restaurant found "
                        "with that name.\n"
                    );
                }

                break;
            }


            /* -------------------------------------------------
             * 3. Search Food
             * ------------------------------------------------- */
            case 3: {

                int restId =
                    getIntInput(
                        "Restaurant ID to search its food: "
                    );

                Restaurant *r =
                    findRestaurantById(
                        restHead,
                        restId
                    );

                if (r == NULL) {

                    printf(
                        "Restaurant not found.\n"
                    );

                    break;
                }


                char foodName[NAME_LEN];

                getStringInput(
                    "Food name to search: ",
                    foodName,
                    NAME_LEN
                );


                /* Linear Search */
                Food *f =
                    findFoodByName(
                        r->menu,
                        foodName
                    );


                if (f != NULL) {

                    printFood(f);

                } else {

                    printf(
                        "No such food item "
                        "found on this menu.\n"
                    );
                }

                break;
            }


            /* -------------------------------------------------
             * 4. View Menu
             * ------------------------------------------------- */
            case 4: {

                int restId =
                    getIntInput(
                        "Restaurant ID: "
                    );

                Restaurant *r =
                    findRestaurantById(
                        restHead,
                        restId
                    );


                if (r == NULL) {

                    printf(
                        "Restaurant not found.\n"
                    );

                    break;
                }


                printf(
                    "\n--- Sort Menu By Price ---\n"
                );

                printf(
                    "1. Low -> High [Bubble Sort]\n"
                );

                printf(
                    "2. High -> Low [Selection Sort]\n"
                );

                printf(
                    "3. Low -> High [Merge Sort]\n"
                );

                printf("4. No sorting\n");


                int sortChoice =
                    getIntInput("Choose: ");


                Food *arr[MAX_SORT_ITEMS];

                int n =
                    foodsToArray(
                        r->menu,
                        arr,
                        MAX_SORT_ITEMS
                    );


                if (sortChoice == 1) {

                    bubbleSortFoodsByPrice(
                        arr,
                        n,
                        1
                    );

                } else if (sortChoice == 2) {

                    selectionSortFoodsByPrice(
                        arr,
                        n,
                        0
                    );

                } else if (sortChoice == 3) {

                    mergeSortFoodsByPrice(
                        arr,
                        n,
                        1
                    );
                }


                if (n == 0) {

                    printf(
                        "No food items available "
                        "in this menu.\n"
                    );

                } else {

                    printf(
                        "%-5s %-25s %-15s %s\n",
                        "ID",
                        "Name",
                        "Category",
                        "Price"
                    );

                    printLine();

                    int i;

                    for (i = 0; i < n; i++) {

                        printFood(arr[i]);
                    }


                    /* Add food to cart */

                    char addMore[5];

                    getStringInput(
                        "\nAdd an item to cart? (y/n): ",
                        addMore,
                        5
                    );


                    if (
                        addMore[0] == 'y' ||
                        addMore[0] == 'Y'
                    ) {

                        int foodId =
                            getIntInput(
                                "Food ID to add: "
                            );


                        Food *f =
                            findFoodById(
                                r->menu,
                                foodId
                            );


                        if (f == NULL) {

                            printf(
                                "Invalid food ID.\n"
                            );

                        } else {

                            int qty =
                                getIntInput(
                                    "Quantity: "
                                );


                            if (qty <= 0) {
                                qty = 1;
                            }


                            if (
                                addToCart(
                                    &cart,
                                    f->id,
                                    f->name,
                                    f->price,
                                    qty,
                                    r->id,
                                    r->name
                                )
                            ) {

                                printf(
                                    "Added to cart.\n"
                                );
                            }
                        }
                    }
                }

                break;
            }


            /* -------------------------------------------------
             * 5. Cart
             * ------------------------------------------------- */
            case 5: {

                int cartChoice;

                do {

                    printf("\n--- Cart ---\n");

                    viewCart(cart);

                    printf(
                        "\n1. Remove Item\n"
                        "2. Update Quantity\n"
                        "3. Checkout\n"
                        "4. Back\n"
                    );

                    cartChoice =
                        getIntInput(
                            "Choose: "
                        );


                    switch (cartChoice) {

                        case 1: {

                            int id =
                                getIntInput(
                                    "Food ID to remove: "
                                );


                            if (
                                removeFromCart(
                                    &cart,
                                    id
                                )
                            ) {

                                printf(
                                    "Removed.\n"
                                );

                            } else {

                                printf(
                                    "Item not found "
                                    "in cart.\n"
                                );
                            }

                            break;
                        }


                        case 2: {

                            int id =
                                getIntInput(
                                    "Food ID to update: "
                                );

                            int qty =
                                getIntInput(
                                    "New quantity: "
                                );


                            if (qty <= 0) {

                                removeFromCart(
                                    &cart,
                                    id
                                );

                                printf(
                                    "Quantity 0 or less "
                                    "- item removed.\n"
                                );

                            } else if (
                                updateCartQuantity(
                                    cart,
                                    id,
                                    qty
                                )
                            ) {

                                printf(
                                    "Quantity updated.\n"
                                );

                            } else {

                                printf(
                                    "Item not found "
                                    "in cart.\n"
                                );
                            }

                            break;
                        }


                        case 3:

                            checkoutFlow(
                                &cart,
                                customer,
                                restHead,
                                graph,
                                pending,
                                nextOrderId
                            );

                            break;


                        case 4:

                            break;


                        default:

                            printf(
                                "Invalid choice.\n"
                            );
                    }

                } while (
                    cartChoice != 3 &&
                    cartChoice != 4
                );

                break;
            }


            /* -------------------------------------------------
             * 6. Order History
             * ------------------------------------------------- */
            case 6:

                printf(
                    "\n--- Your Order History "
                    "(latest first) ---\n"
                );

                printHistoryForCustomer(
                    history,
                    customer->id
                );

                break;


            /* -------------------------------------------------
             * 7. Delete Account
             * ------------------------------------------------- */
            case 7: {

                char confirm[10];

                getStringInput(
                    "\nDelete your account? (yes/no): ",
                    confirm,
                    10
                );


                if (
                    strcmp(confirm, "yes") == 0 ||
                    strcmp(confirm, "YES") == 0
                ) {

                    /*
                     * Delete the customer from the
                     * linked list.
                     */
                    if (
                        deleteCustomer(
                            customerHead,
                            customer->id
                        )
                    ) {

                        /*
                         * Save the updated customer
                         * list to the file.
                         */
                        saveCustomers(
                            *customerHead
                        );


                        printf(
                            "\nAccount deleted successfully.\n"
                        );

                        printf(
                            "Logging out...\n"
                        );


                        /*
                         * 8 = Logout
                         */
                        choice = 8;

                    } else {

                        printf(
                            "\nAccount deletion failed.\n"
                        );
                    }

                } else {

                    printf(
                        "\nAccount deletion cancelled.\n"
                    );
                }

                break;
            }


            /* -------------------------------------------------
             * 8. Logout
             * ------------------------------------------------- */
            case 8:

                printf(
                    "Logging out...\n"
                );

                break;


            default:

                printf(
                    "Invalid choice.\n"
                );
        }

    } while (choice != 8);


    /* ---------------------------------------------------------
     * Free remaining cart items when customer logs out.
     * --------------------------------------------------------- */
    if (cart != NULL) {

        printf(
            "Note: items left in your cart "
            "were discarded on logout.\n"
        );

        freeCart(&cart);
    }
}


/* ---------------------------------------------------------
 * Main Menu / Program Entry Point
 * --------------------------------------------------------- */
int main(void)
{
    Graph graph;

    initGraph(&graph);


    int nextCustomerId = 1;
    int nextRestId = 1;
    int nextFoodId = 1;
    int nextOrderId = 1;


    /* -----------------------------------------------------
     * Load Customers
     * ----------------------------------------------------- */
    Customer *customerHead =
        loadCustomers(
            &nextCustomerId
        );


    /* -----------------------------------------------------
     * Load Restaurants
     * ----------------------------------------------------- */
    Restaurant *restHead =
        loadRestaurants(
            &nextRestId,
            &nextFoodId
        );


    /* Create sample data if no restaurants exist */
    if (restHead == NULL) {

        restHead =
            seedSampleData(
                &nextRestId,
                &nextFoodId
            );
    }


    /* -----------------------------------------------------
     * Initialize Queues
     * ----------------------------------------------------- */
    Queue pending;
    Queue accepted;
    Queue preparing;
    Queue delivery;

    initQueue(&pending);
    initQueue(&accepted);
    initQueue(&preparing);
    initQueue(&delivery);


    /* Load existing orders */
    loadOrders(
        &pending,
        &accepted,
        &preparing,
        &delivery,
        &nextOrderId
    );


    /* -----------------------------------------------------
     * Initialize Order History Stack
     * ----------------------------------------------------- */
    Stack history;

    initStack(&history);

    loadHistory(&history);


    /* -----------------------------------------------------
     * Welcome Screen
     * ----------------------------------------------------- */
    printf(
        "============================================================\n"
    );

    printf(
        "   WELCOME TO FOODDELIVERY - "
        "Console Food Delivery System\n"
    );

    printf(
        "============================================================\n"
    );


    /* -----------------------------------------------------
     * Main Menu
     * ----------------------------------------------------- */

    int choice;

    do {

        printf(
            "\n===================== Main Menu "
            "=====================\n"
        );

        printf("1. Admin Login\n");
        printf("2. Customer Login\n");
        printf("3. Customer Registration\n");
        printf("4. Exit\n");


        choice =
            getIntInput(
                "Choose an option: "
            );


        switch (choice) {

            /* -------------------------------------------------
             * Admin Login
             * ------------------------------------------------- */
            case 1:

                if (adminLogin()) {

                    adminDashboard(
                        &restHead,
                        customerHead,
                        &graph,
                        &pending,
                        &accepted,
                        &preparing,
                        &delivery,
                        &history,
                        &nextRestId,
                        &nextFoodId
                    );
                }

                break;


            /* -------------------------------------------------
             * Customer Login
             * ------------------------------------------------- */
            case 2: {

                Customer *c =
                    customerLoginPrompt(
                        customerHead
                    );


                if (c != NULL) {

                    /*
                     * Pass &customerHead so that the
                     * customer dashboard can delete
                     * the logged-in customer.
                     */
                    customerDashboard(
                        c,
                        &customerHead,
                        restHead,
                        &graph,
                        &pending,
                        &history,
                        &nextOrderId
                    );


                    saveOrders(
                        &pending,
                        &accepted,
                        &preparing,
                        &delivery
                    );

                    saveHistory(
                        &history
                    );
                }

                break;
            }


            /* -------------------------------------------------
             * Customer Registration
             * ------------------------------------------------- */
            case 3: {

                Customer *c =
                    customerRegister(
                        &customerHead,
                        &graph,
                        &nextCustomerId
                    );


                if (c != NULL) {

                    saveCustomers(
                        customerHead
                    );
                }

                break;
            }


            /* -------------------------------------------------
             * Exit
             * ------------------------------------------------- */
            case 4:

                printf(
                    "Saving data and exiting. Goodbye!\n"
                );

                break;


            default:

                printf(
                    "Invalid choice. "
                    "Please select 1-4.\n"
                );
        }

    } while (choice != 4);


    /* ---------------------------------------------------------
     * Final Save on Exit
     * --------------------------------------------------------- */

    saveCustomers(customerHead);
    saveRestaurants(restHead);
    saveOrders(&pending, &accepted, &preparing, &delivery);
    saveHistory(&history);


    /* ---------------------------------------------------------
     * Free Dynamically Allocated Memory
     * --------------------------------------------------------- */

    freeCustomerList(customerHead );
    freeRestaurantList(restHead);
    freeQueue(&pending);
    freeQueue(&accepted);
    freeQueue(&preparing);
    freeQueue(&delivery);
    freeStack(&history);


    return 0;
}