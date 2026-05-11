/* main.c - entry point for GreenScan */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"
#include "product.h"
#include "history.h"
#include "admin.h"

void clearScreen(void) {
    system("cls");
}

void mainMenu(void) {
    printf("\n========== GreenScan Menu ==========");
    printf("\n1. Register");
    printf("\n2. Login");
    printf("\n3. Exit");
    printf("\n==================================\n");
}

void userMenu(void) {
    printf("\n========= User Menu ==========");
    printf("\n1. Search Product");
    printf("\n2. View My Search History");
    printf("\n3. View Most Searched Products");
    printf("\n4. Logout");
    printf("\n=============================\n");
}

void adminMenu(void) {
    printf("\n========= Admin Menu ==========");
    printf("\n1. Add Product");
    printf("\n2. Edit Product");
    printf("\n3. Delete Product");
    printf("\n4. View All Products");
    printf("\n5. View Registered Users");
    printf("\n6. View Most Searched Products");
    printf("\n7. Logout");
    printf("\n==============================\n");
}

int handleUserMenu(User loggedIn) {
    int choice;
    char barcode[20];
    Product result;

    while (1) {
        userMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // consume newline

        switch (choice) {
            case 1:  // Search Product
                printf("Search Options:\n");
                printf("1. Search by Barcode\n");
                printf("2. Search by Name\n");
                printf("Enter choice: ");
                int search_choice;
                scanf("%d", &search_choice);
                getchar();  // consume newline

                if (search_choice == 1) {
                    printf("Enter barcode to search: ");
                    fgets(barcode, sizeof(barcode), stdin);
                    barcode[strcspn(barcode, "\n")] = '\0';

                    if (searchByBarcode(barcode, &result) == 0) {
                        displayProduct(result);
                        displayVariants(result);
                        saveHistory(loggedIn.username, result);
                        updateSearchCount(result);
                        recommendAlternatives(result);
                    } else {
                        printf("Product not found\n");
                    }
                } else if (search_choice == 2) {
                    printf("Enter product name to search: ");
                    char name_query[100];
                    fgets(name_query, sizeof(name_query), stdin);
                    name_query[strcspn(name_query, "\n")] = '\0';

                    Product results[MAX_PRODUCTS];
                    int res_count = 0;
                    if (searchByName(name_query, results, &res_count) == 0 && res_count > 0) {
                        if (res_count == 1) {
                            result = results[0];
                            displayProduct(result);
                            displayVariants(result);
                            saveHistory(loggedIn.username, result);
                            updateSearchCount(result);
                            recommendAlternatives(result);
                        } else {
                            printf("Multiple products found:\n");
                            for (int i = 0; i < res_count; i++) {
                                printf("%d. %s (Barcode: %s)\n", i+1, results[i].name, results[i].barcode);
                            }
                            printf("Enter the number to view details: ");
                            int pick;
                            scanf("%d", &pick);
                            getchar();  // consume newline
                            if (pick >= 1 && pick <= res_count) {
                                result = results[pick-1];
                                displayProduct(result);
                                displayVariants(result);
                                saveHistory(loggedIn.username, result);
                                updateSearchCount(result);
                                recommendAlternatives(result);
                            } else {
                                printf("Invalid choice\n");
                            }
                        }
                    } else {
                        printf("No products found\n");
                    }
                } else {
                    printf("Invalid choice\n");
                }
                break;

            case 2:  // View History
                viewHistory(loggedIn.username);
                break;

            case 3:  // View Most Searched
                viewMostSearched();
                break;

            case 4:  // Logout
                printf("Logged out successfully\n");
                return 0;

            default:
                printf("Invalid choice\n");
        }
        printf("Press Enter to continue...");
        getchar();
        clearScreen();
    }
}

int handleAdminMenu(User loggedIn) {
    int choice;

    while (1) {
        adminMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // consume newline

        switch (choice) {
            case 1:  // Add Product
                adminAddProduct();
                break;

            case 2:  // Edit Product
                adminEditProduct();
                break;

            case 3:  // Delete Product
                adminDeleteProduct();
                break;

            case 4:  // View All Products
                adminViewAllProducts();
                break;

            case 5:  // View Users
                adminViewUsers();
                break;

            case 6:  // View Most Searched
                viewMostSearched();
                break;

            case 7:  // Logout
                printf("Admin logged out successfully\n");
                return 0;

            default:
                printf("Invalid choice\n");
        }
        printf("Press Enter to continue...");
        getchar();
        clearScreen();
    }
}

int main(void) {
    int choice;
    char username[50], password[50];
    User loggedIn;

    printf("\n   ===== Welcome to GreenScan =====\n");
    printf("   Scan Green, Choose Green, Be Green!\n\n");

    while (1) {
        mainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // consume newline

        switch (choice) {
            case 1:  // Register
                printf("\n=== Register New User ===\n");
                printf("Username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0';

                printf("Password: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = '\0';

                if (registerUser(username, password, "user") == 0) {
                    printf("Registration successful!\n");
                } else {
                    printf("Registration failed!\n");
                }
                break;

            case 2:  // Login
                printf("\n=== Login ===\n");
                printf("Username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0';

                printf("Password: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = '\0';

                if (loginUser(username, password, &loggedIn) == 0) {
                    printf("\nLogin successful!\n");
                    printf("Press Enter to continue...");
                    getchar();
                    clearScreen();

                    if (isAdmin(loggedIn)) {
                        printf("\nWelcome Admin %s!\n", loggedIn.username);
                        handleAdminMenu(loggedIn);
                    } else {
                        printf("\nWelcome %s!\n", loggedIn.username);
                        handleUserMenu(loggedIn);
                    }
                    clearScreen();
                } else {
                    printf("Login failed!\n");
                }
                break;

            case 3:  // Exit
                printf("\nThank you for using GreenScan!\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
        printf("Press Enter to continue...");
        getchar();
        clearScreen();
    }

    return 0;
}
