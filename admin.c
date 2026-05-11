/* admin.c - admin features implementation */

#include "admin.h"
#include "database.h"
#include "greenscore.h"
#include <stdio.h>
#include <string.h>

int adminAddProduct(void) {
    Product p;

    printf("\n=== Add New Product ===\n");
    printf("Barcode: ");
    fgets(p.barcode, sizeof(p.barcode), stdin);
    p.barcode[strcspn(p.barcode, "\n")] = '\0';

    printf("Name: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    printf("Category: ");
    fgets(p.category, sizeof(p.category), stdin);
    p.category[strcspn(p.category, "\n")] = '\0';

    printf("Subcategory: ");
    fgets(p.subcategory, sizeof(p.subcategory), stdin);
    p.subcategory[strcspn(p.subcategory, "\n")] = '\0';

    printf("Variant Group: ");
    fgets(p.variantGroup, sizeof(p.variantGroup), stdin);
    p.variantGroup[strcspn(p.variantGroup, "\n")] = '\0';

    printf("Ingredients: ");
    fgets(p.ingredients, sizeof(p.ingredients), stdin);
    p.ingredients[strcspn(p.ingredients, "\n")] = '\0';

    printf("Packaging: ");
    fgets(p.packaging, sizeof(p.packaging), stdin);
    p.packaging[strcspn(p.packaging, "\n")] = '\0';

    // Calculate green score automatically
    p.greenScore = calculateGreenScore(p);

    if (appendProduct(p) == 0) {
        printf("Product added successfully with Green Score: %d\n", p.greenScore);
        return 0;
    }
    printf("Failed to add product\n");
    return -1;
}

int adminEditProduct(void) {
    char barcode[20];
    Product products[MAX_PRODUCTS];
    int count = 0;

    printf("\n=== Edit Product ===\n");
    printf("Enter barcode to edit: ");
    fgets(barcode, sizeof(barcode), stdin);
    barcode[strcspn(barcode, "\n")] = '\0';

    if (loadProducts(products, &count) != 0) {
        printf("Failed to load products\n");
        return -1;
    }

    int found_idx = -1;
    for (int i = 0; i < count; ++i) {
        if (strcmp(products[i].barcode, barcode) == 0) {
            found_idx = i;
            break;
        }
    }

    if (found_idx == -1) {
        printf("Product not found\n");
        return -1;
    }

    printf("\nEnter new values (press Enter to keep current):\n");

    printf("Name [%s]: ", products[found_idx].name);
    char temp[100];
    fgets(temp, sizeof(temp), stdin);
    if (strlen(temp) > 1) {
        temp[strcspn(temp, "\n")] = '\0';
        strcpy(products[found_idx].name, temp);
    }

    printf("Category [%s]: ", products[found_idx].category);
    fgets(temp, sizeof(temp), stdin);
    if (strlen(temp) > 1) {
        temp[strcspn(temp, "\n")] = '\0';
        strcpy(products[found_idx].category, temp);
    }
    printf("Subcategory [%s]: ", products[found_idx].subcategory);
    fgets(temp, sizeof(temp), stdin);
    if (strlen(temp) > 1) {
        temp[strcspn(temp, "\n")] = '\0';
        strcpy(products[found_idx].subcategory, temp);
    }

    printf("Variant Group [%s]: ", products[found_idx].variantGroup);
    fgets(temp, sizeof(temp), stdin);
    if (strlen(temp) > 1) {
        temp[strcspn(temp, "\n")] = '\0';
        strcpy(products[found_idx].variantGroup, temp);
    }

    printf("Ingredients [%s]: ", products[found_idx].ingredients);
    char temp_ing[300];
    fgets(temp_ing, sizeof(temp_ing), stdin);
    if (strlen(temp_ing) > 1) {
        temp_ing[strcspn(temp_ing, "\n")] = '\0';
        strcpy(products[found_idx].ingredients, temp_ing);
    }

    printf("Packaging [%s]: ", products[found_idx].packaging);
    fgets(temp, sizeof(temp), stdin);
    if (strlen(temp) > 1) {
        temp[strcspn(temp, "\n")] = '\0';
        strcpy(products[found_idx].packaging, temp);
    }

    // Recalculate green score
    products[found_idx].greenScore = calculateGreenScore(products[found_idx]);

    if (saveProducts(products, count) == 0) {
        printf("Product updated successfully with new Green Score: %d\n", products[found_idx].greenScore);
        return 0;
    }
    printf("Failed to update product\n");
    return -1;
}

int adminDeleteProduct(void) {
    char barcode[20];
    Product products[MAX_PRODUCTS];
    int count = 0;

    printf("\n=== Delete Product ===\n");
    printf("Enter barcode to delete: ");
    fgets(barcode, sizeof(barcode), stdin);
    barcode[strcspn(barcode, "\n")] = '\0';

    if (loadProducts(products, &count) != 0) {
        printf("Failed to load products\n");
        return -1;
    }

    int found_idx = -1;
    for (int i = 0; i < count; ++i) {
        if (strcmp(products[i].barcode, barcode) == 0) {
            found_idx = i;
            break;
        }
    }

    if (found_idx == -1) {
        printf("Product not found\n");
        return -1;
    }

    // Shift remaining products
    for (int i = found_idx; i < count - 1; ++i) {
        products[i] = products[i + 1];
    }
    count--;

    if (saveProducts(products, count) == 0) {
        printf("Product deleted successfully\n");
        return 0;
    }
    printf("Failed to delete product\n");
    return -1;
}

int adminViewAllProducts(void) {
    Product products[MAX_PRODUCTS];
    int count = 0;

    if (loadProducts(products, &count) != 0) {
        printf("Failed to load products\n");
        return -1;
    }

    printf("\n=== All Products ===\n");
    if (count == 0) {
        printf("No products available\n");
        return 0;
    }

    for (int i = 0; i < count; ++i) {
        int score = calculateGreenScore(products[i]);
        printf("\n%d. %s\n", i + 1, products[i].name);
        printf("   Barcode: %s\n", products[i].barcode);
        printf("   Category: %s\n", products[i].category);
        printf("   Green Score: %d/100\n", score);
    }
    printf("\n==================\n\n");
    return 0;
}

int adminViewUsers(void) {
    FILE *f = fopen("users.txt", "r");
    if (!f) {
        printf("No users file found\n");
        return -1;
    }

    printf("\n=== Registered Users ===\n");
    char line[200];
    int count = 0;

    while (fgets(line, sizeof(line), f)) {
        char u[50], p[50], r[20];
        if (sscanf(line, "%49[^|]|%49[^|]|%19[^\n]", u, p, r) == 3) {
            printf("%d. Username: %s | Role: %s\n", ++count, u, r);
        }
    }

    printf("========================\n\n");
    fclose(f);

    if (count == 0) {
        printf("No users registered\n");
        return -1;
    }
    return 0;
}
