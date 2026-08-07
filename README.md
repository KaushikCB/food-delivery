# FoodDelivery — Console-Based Food Delivery Management System

A menu-driven C console application built for a **Data Structure Lab
Course**. It models a simplified Foodpanda/Zomato-style food delivery platform and
demonstrates practical implementations of core data structures and algorithms —
no external libraries, everything is written by hand.

---

## 1. Data Structures & Algorithms Implemented

| Concept                     | Where                          | Used For                                             |
|------------------------------|--------------------------------|-------------------------------------------------------|
| Singly Linked List           | `customer.c`, `restaurant.c`, `menu.c`, `cart.c` | Customers, Restaurants, each Restaurant's Food menu, Shopping Cart |
| Queue (FIFO)                  | `queue.c`                      | Pending / Accepted / Preparing / Out-for-Delivery order pipeline |
| Stack (LIFO)                  | `stack.c`                      | Customer order history (latest delivered order first) |
| Weighted Graph (adjacency matrix) | `graph.c`                  | Delivery road network between areas                   |
| Dijkstra's Shortest Path      | `graph.c`                      | Shortest delivery route, total distance & ETA at checkout |
| Linear Search                 | `login.c`, `customer.c`, `restaurant.c`, `menu.c` | Username/password validation, search by name/id |
| Binary Search                 | `sorting.c`                    | Searching a food item by exact price (after sorting)  |
| Bubble Sort                   | `sorting.c`                    | Sort restaurants by name / food by price               |
| Selection Sort                | `sorting.c`                    | Sort restaurants by rating / food by price              |
| Merge Sort                    | `sorting.c`                    | Sort restaurants by rating / food by price (bonus)      |
| File I/O                      | `file.c`                       | Persist & auto-reload every data structure to `.txt`   |

---

## 2. Project Structure

```
FoodDelivery/
│── main.c / main.o            → Program entry point, Main Menu, Customer Dashboard
│── login.c / login.h          → Admin login, Customer registration & login
│── customer.c / customer.h    → Customer Linked List
│── restaurant.c / restaurant.h→ Restaurant Linked List
│── menu.c / menu.h            → Food Linked List (each restaurant's menu)
│── cart.c / cart.h            → Shopping Cart Linked List
│── order.c / order.h          → Shared Order data type + status helpers
│── admin.c / admin.h          → Admin Dashboard (restaurant/food mgmt, order processing, sales)
│── queue.c / queue.h          → Manual FIFO Queue (order pipeline)
│── stack.c / stack.h          → Manual LIFO Stack (order history)
│── sorting.c / sorting.h      → Bubble / Selection / Merge Sort + Binary Search
│── graph.c / graph.h          → Delivery road network + Dijkstra's Algorithm
│── file.c / file.h            → Text-file persistence (auto load/save)
│── utility.c / utility.h      → Safe input helpers, formatting helpers
│── Makefile                   → Build script
│── README.md                  → This file
│── customers.txt              → Sample data (1 demo customer: johndoe / john123)
│── restaurants.txt            → Sample data (3 demo restaurants)
│── foods.txt                  → Sample data (8 demo food items)
│── orders.txt                 → Active order pipeline (auto-created, starts empty)
│── history.txt                → Delivered order history (auto-created, starts empty)
```

---

## 3. Compilation & Running

### Option A — using the Makefile (recommended)

```bash
make            # compiles everything into ./fooddelivery
./fooddelivery  # run the program
make clean      # removes object files and the binary
```

### Option B — manual GCC command

```bash
gcc -std=c99 -Wall -Wextra -o fooddelivery \
    main.c login.c customer.c restaurant.c menu.c cart.c order.c \
    admin.c queue.c stack.c sorting.c graph.c file.c utility.c

./fooddelivery
```

The program compiles cleanly with `gcc -Wall -Wextra -Wpedantic` — no warnings.

> **Note:** All `.txt` data files are read/written in the **current working
> directory**. Run the binary from inside the `FoodDelivery/` folder so it can
> find `customers.txt`, `restaurants.txt`, etc.

---

## 4. Default / Demo Credentials

| Role     | Username  | Password  |
|----------|-----------|-----------|
| Admin    | `Kaushik`   | `Summer2026`|
| Customer | `johndoe` | `john123` |

If `restaurants.txt` is missing (first run), the program automatically seeds 3
sample restaurants (KFC Banani, Pizza Hut Gulshan, Star Kabab Mohakhali) with
food items so the system is immediately demoable.

---

## 5. Menu Overview

**Main Menu**
```
1. Admin Login
2. Customer Login
3. Customer Registration
4. Exit
```

**Customer Dashboard**
```
1. View Restaurants     (sortable: Bubble / Selection / Merge Sort)
2. Search Restaurant    (Linear Search by name)
3. Search Food          (Linear Search by name)
4. View Menu            (sortable by price; add items to Cart)
5. Cart                 (remove / update qty / checkout)
6. Order History        (Stack — latest delivered order first)
7. Logout
```

**Admin Dashboard**
```
1. Restaurant Management  (Add / Delete / Update / View / Search)
2. Food Management        (Add / Delete / Update / View, per restaurant)
3. Customer List
4. Pending Orders
5. Process Orders         (advance orders through the Queue pipeline)
6. Sales Report           (from the delivered-orders Stack)
7. Logout
```

---

## 6. Delivery Route (Dijkstra) Example

The delivery road network is a small weighted graph of Dhaka-area nodes
(`Restaurant, Banani, Gulshan, Mohakhali, Badda, Bashundhara, Dhanmondi,
Uttara`). At checkout, Dijkstra's Algorithm computes the shortest path (by
distance) from the restaurant's location to the customer's location:

```
Restaurant:
KFC Banani

Customer:
Bashundhara

Shortest Path:
Banani -> Mohakhali -> Bashundhara

Distance:
4.8 KM

Estimated Time:
13 Minutes
```

---

## 7. Design Notes

- **One restaurant per cart.** Like real delivery apps, a cart can only hold
  items from a single restaurant at a time; adding from a different
  restaurant is rejected until the cart is cleared/checked out.
- **Order pipeline.** An order moves `Pending → Accepted → Preparing →
  Out-for-Delivery → Delivered`. The first four stages live in `Queue`
  structures (FIFO); once delivered, the order is pushed onto the history
  `Stack` (LIFO) so the customer always sees their most recent order first.
- **File format.** All text files use `|` as a field delimiter so that names
  containing spaces are stored safely. Files are rewritten on every meaningful
  change and reloaded automatically on startup — nothing is lost between runs.
- **Memory management.** Every linked structure (`Customer`, `Restaurant` +
  its `Food` menu, `Queue`, `Stack`) is freed on program exit to avoid leaks.
- **Input validation.** All numeric input is read via `getIntInput` /
  `getFloatInput`, which reprompt on non-numeric input instead of crashing or
  looping infinitely.

---

## 8. Possible Extensions (left as exercises)

- Multiple items per order broken into individual line entries (currently a
  single summarized string) for a more detailed sales report.
- Restaurant-side food category filtering.
- A weighted, non-linear "estimated time" model instead of static edge
  weights.
