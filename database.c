/* database.c - simple file-based database implementation */

#include "database.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char *PRODUCTS_FILE = "products.txt";

int loadProducts(Product arr[], int *count) {
    if (!count || !arr) return -1;
    *count = 0;

    FILE *f = fopen(PRODUCTS_FILE, "r");
    if (!f) {
        printf("Failed to open %s for reading\n", PRODUCTS_FILE);
        return -1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        // Remove trailing newline
        size_t len = strlen(line);
        if (len && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[--len] = '\0';
            if (len && line[len-1] == '\r') line[--len] = '\0';
        }

        if (len == 0) continue;

        // Parse pipe-separated fields
        char *parts[8] = {0};
        char *p = line;
        for (int i = 0; i < 8 && p; ++i) {
            parts[i] = p;
            char *sep = strchr(p, '|');
            if (sep) {
                *sep = '\0';
                p = sep + 1;
            } else {
                p = NULL;
            }
        }

        // Validate we have at least 8 parts
        if (!parts[0] || !parts[1] || !parts[2] || !parts[3] || !parts[4] || !parts[5] || !parts[6] || !parts[7]) {
            printf("Skipping malformed line in %s: %s\n", PRODUCTS_FILE, line);
            continue;
        }

        if (*count >= MAX_PRODUCTS) {
            printf("Reached maximum product capacity (%d), stopping load\n", MAX_PRODUCTS);
            break;
        }

        Product *prod = &arr[*count];
        strncpy(prod->barcode, parts[0], sizeof(prod->barcode)-1);
        prod->barcode[sizeof(prod->barcode)-1] = '\0';
        strncpy(prod->name, parts[1], sizeof(prod->name)-1);
        prod->name[sizeof(prod->name)-1] = '\0';
        strncpy(prod->category, parts[2], sizeof(prod->category)-1);
        prod->category[sizeof(prod->category)-1] = '\0';
        strncpy(prod->subcategory, parts[3], sizeof(prod->subcategory)-1);
        prod->subcategory[sizeof(prod->subcategory)-1] = '\0';
        strncpy(prod->variantGroup, parts[4], sizeof(prod->variantGroup)-1);
        prod->variantGroup[sizeof(prod->variantGroup)-1] = '\0';
        strncpy(prod->ingredients, parts[5], sizeof(prod->ingredients)-1);
        prod->ingredients[sizeof(prod->ingredients)-1] = '\0';
        strncpy(prod->packaging, parts[6], sizeof(prod->packaging)-1);
        prod->packaging[sizeof(prod->packaging)-1] = '\0';
        prod->greenScore = atoi(parts[7]);

        (*count)++;
    }

    fclose(f);
    return 0;
}

int saveProducts(Product arr[], int count) {
    FILE *f = fopen(PRODUCTS_FILE, "w");
    if (!f) {
        printf("Failed to open %s for writing\n", PRODUCTS_FILE);
        return -1;
    }

    for (int i = 0; i < count; ++i) {
        Product *p = &arr[i];
        // Ensure no internal pipes in fields by replacing with spaces
        char name[sizeof(p->name)];
        char category[sizeof(p->category)];
        char subcategory[sizeof(p->subcategory)];
        char variantGroup[sizeof(p->variantGroup)];
        char ingredients[sizeof(p->ingredients)];
        char packaging[sizeof(p->packaging)];

        strncpy(name, p->name, sizeof(name)); name[sizeof(name)-1] = '\0';
        strncpy(category, p->category, sizeof(category)); category[sizeof(category)-1] = '\0';
        strncpy(subcategory, p->subcategory, sizeof(subcategory)); subcategory[sizeof(subcategory)-1] = '\0';
        strncpy(variantGroup, p->variantGroup, sizeof(variantGroup)); variantGroup[sizeof(variantGroup)-1] = '\0';
        strncpy(ingredients, p->ingredients, sizeof(ingredients)); ingredients[sizeof(ingredients)-1] = '\0';
        strncpy(packaging, p->packaging, sizeof(packaging)); packaging[sizeof(packaging)-1] = '\0';

        for (char *c = name; *c; ++c) if (*c == '|') *c = ' ';
        for (char *c = category; *c; ++c) if (*c == '|') *c = ' ';
        for (char *c = subcategory; *c; ++c) if (*c == '|') *c = ' ';
        for (char *c = variantGroup; *c; ++c) if (*c == '|') *c = ' ';
        for (char *c = ingredients; *c; ++c) if (*c == '|') *c = ' ';
        for (char *c = packaging; *c; ++c) if (*c == '|') *c = ' ';

        fprintf(f, "%s|%s|%s|%s|%s|%s|%s|%d\n",
                p->barcode, name, category, subcategory, variantGroup, ingredients, packaging, p->greenScore);
    }

    fflush(f);
    fclose(f);
    return 0;
}

int appendProduct(Product p) {
    FILE *f = fopen(PRODUCTS_FILE, "a");
    if (!f) {
        printf("Failed to open %s for appending\n", PRODUCTS_FILE);
        return -1;
    }

    // Sanitize fields to remove pipes
    char name[sizeof(p.name)];
    char category[sizeof(p.category)];
    char subcategory[sizeof(p.subcategory)];
    char variantGroup[sizeof(p.variantGroup)];
    char ingredients[sizeof(p.ingredients)];
    char packaging[sizeof(p.packaging)];

    strncpy(name, p.name, sizeof(name)); name[sizeof(name)-1] = '\0';
    strncpy(category, p.category, sizeof(category)); category[sizeof(category)-1] = '\0';
    strncpy(subcategory, p.subcategory, sizeof(subcategory)); subcategory[sizeof(subcategory)-1] = '\0';
    strncpy(variantGroup, p.variantGroup, sizeof(variantGroup)); variantGroup[sizeof(variantGroup)-1] = '\0';
    strncpy(ingredients, p.ingredients, sizeof(ingredients)); ingredients[sizeof(ingredients)-1] = '\0';
    strncpy(packaging, p.packaging, sizeof(packaging)); packaging[sizeof(packaging)-1] = '\0';

    for (char *c = name; *c; ++c) if (*c == '|') *c = ' ';
    for (char *c = category; *c; ++c) if (*c == '|') *c = ' ';
    for (char *c = subcategory; *c; ++c) if (*c == '|') *c = ' ';
    for (char *c = variantGroup; *c; ++c) if (*c == '|') *c = ' ';
    for (char *c = ingredients; *c; ++c) if (*c == '|') *c = ' ';
    for (char *c = packaging; *c; ++c) if (*c == '|') *c = ' ';

    int res = fprintf(f, "%s|%s|%s|%s|%s|%s|%s|%d\n",
                      p.barcode, name, category, subcategory, variantGroup, ingredients, packaging, p.greenScore);

    if (res < 0) {
        printf("Failed to write to %s\n", PRODUCTS_FILE);
        fclose(f);
        return -1;
    }

    fflush(f);
    fclose(f);
    return 0;
}
