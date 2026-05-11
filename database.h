/* database.h - simple file-based database interface */

#ifndef DATABASE_H
#define DATABASE_H

#include "structs.h"

#include <stdio.h>

#define MAX_PRODUCTS 1024

/*
 * Load all products from products.txt into arr.
 * On success returns 0 and sets *count to number of products read.
 * On failure returns -1 and sets *count to 0.
 */
int loadProducts(Product arr[], int *count);

/*
 * Save count products from arr into products.txt (overwrite).
 * Returns 0 on success, -1 on failure.
 */
int saveProducts(Product arr[], int count);

/*
 * Append a single product to products.txt. Returns 0 on success, -1 on failure.
 */
int appendProduct(Product p);

#endif // DATABASE_H
