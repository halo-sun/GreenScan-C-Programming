/* product.c - product management implementation */

#include "product.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int searchByBarcode(char *barcode, Product *result) {
    if (!barcode || !result) return -1;

    Product products[MAX_PRODUCTS];
    int count = 0;

    if (loadProducts(products, &count) != 0) {
        printf("Failed to load products\n");
        return -1;
    }

    for (int i = 0; i < count; ++i) {
        if (strcmp(products[i].barcode, barcode) == 0) {
            *result = products[i];
            return 0;
        }
    }

    printf("Product with barcode '%s' not found\n", barcode);
    return -1;
}

void displayProduct(Product p) {
    int score = calculateGreenScore(p);
    const char *label;

    if (score <= 40) {
        label = "Poor";
    } else if (score <= 65) {
        label = "Moderate";
    } else {
        label = "Good";
    }

    printf("\n=== Product Details ===\n");
    printf("Barcode: %s\n", p.barcode);
    printf("Name: %s\n", p.name);
    printf("Category: %s\n", p.category);
    printf("Subcategory: %s\n", p.subcategory);
    printf("Variant Group: %s\n", p.variantGroup);
    printf("Ingredients: %s\n", p.ingredients);
    printf("Packaging: %s\n", p.packaging);
    printf("Green Score: %d/100 [%s]\n", score, label);
    printf("======================\n\n");
}

void toLowerStr(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void displayVariants(Product current) {
    Product all[MAX_PRODUCTS];
    int total = 0;

    if (loadProducts(all, &total) != 0) {
        printf("Failed to load products for variants\n");
        return;
    }

    Product variants[MAX_PRODUCTS];
    int variant_count = 0;

    // Find all variants with matching variantGroup but different barcode
    for (int i = 0; i < total; i++) {
        if (strcmp(all[i].variantGroup, current.variantGroup) == 0 &&
            strcmp(all[i].barcode, current.barcode) != 0) {
            variants[variant_count] = all[i];
            variant_count++;
            if (variant_count >= MAX_PRODUCTS) break;
        }
    }

    if (variant_count == 0) {
        printf("\nNo other variants found for this product.\n");
        return;
    }

    printf("\nOther variants of %s:\n", current.name);
    for (int i = 0; i < variant_count; i++) {
        int score = calculateGreenScore(variants[i]);
        printf("%d. %s (Barcode: %s, Score: %d/100)\n",
               i + 1, variants[i].name, variants[i].barcode, score);
    }
    printf("\n");
}

int searchByName(char *query, Product results[], int *count) {
    Product all[MAX_PRODUCTS];
    int total = 0;

    if (loadProducts(all, &total) != 0) {
        return -1;
    }

    char query_lower[100];
    strcpy(query_lower, query);
    toLowerStr(query_lower);

    *count = 0;
    for (int i = 0; i < total; i++) {
        char name_lower[100];
        strcpy(name_lower, all[i].name);
        toLowerStr(name_lower);

        if (strstr(name_lower, query_lower) != NULL) {
            results[*count] = all[i];
            (*count)++;
            if (*count >= MAX_PRODUCTS) break;
        }
    }
    return 0;
}

void recommendAlternatives(Product current) {
    Product products[MAX_PRODUCTS];
    int count = 0;

    if (loadProducts(products, &count) != 0) {
        printf("Failed to load products for recommendations\n");
        return;
    }

    int current_score = calculateGreenScore(current);

    // Find products in same subcategory with higher score
    typedef struct {
        Product p;
        int score;
    } Recommendation;

    Recommendation recs[MAX_PRODUCTS];
    int rec_count = 0;

    // First, add from same subcategory
    for (int i = 0; i < count; ++i) {
        if (strcmp(products[i].subcategory, current.subcategory) == 0 &&
            strcmp(products[i].barcode, current.barcode) != 0) {
            int score = calculateGreenScore(products[i]);
            if (score > current_score) {
                recs[rec_count].p = products[i];
                recs[rec_count].score = score;
                rec_count++;
            }
        }
    }

    // If less than 3, fill with same category (different subcategory)
    if (rec_count < 3) {
        for (int i = 0; i < count && rec_count < 3; ++i) {
            if (strcmp(products[i].category, current.category) == 0 &&
                strcmp(products[i].subcategory, current.subcategory) != 0 &&
                strcmp(products[i].barcode, current.barcode) != 0) {
                int score = calculateGreenScore(products[i]);
                if (score > current_score) {
                    recs[rec_count].p = products[i];
                    recs[rec_count].score = score;
                    rec_count++;
                }
            }
        }
    }

    if (rec_count == 0) {
        printf("\nNo better alternatives found in this category.\n");
        return;
    }

    // Sort by score descending (simple bubble sort for top 3)
    for (int i = 0; i < rec_count - 1 && i < 2; ++i) {
        for (int j = i + 1; j < rec_count; ++j) {
            if (recs[j].score > recs[i].score) {
                Recommendation temp = recs[i];
                recs[i] = recs[j];
                recs[j] = temp;
            }
        }
    }

    // Display top 3
    printf("\n=== Recommended Alternatives ===\n");
    int limit = rec_count < 3 ? rec_count : 3;
    for (int i = 0; i < limit; ++i) {
        printf("%d. %s (Barcode: %s, Score: %d/100)\n",
               i + 1, recs[i].p.name, recs[i].p.barcode, recs[i].score);
    }
    printf("=================================\n\n");
}
