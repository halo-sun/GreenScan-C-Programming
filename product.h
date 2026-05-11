/* product.h - product management interface */

#ifndef PRODUCT_H
#define PRODUCT_H

#include "structs.h"
#include "database.h"
#include "greenscore.h"

/*
 * Search for a product by barcode in products.txt.
 * Returns 0 on success, -1 if not found.
 */
int searchByBarcode(char *barcode, Product *result);

/*
 * Display product information with sustainability label.
 * Labels: 0-40=Poor, 41-65=Moderate, 66-100=Good
 */
void displayProduct(Product p);

/*
 * Display other variants of the product.
 */
void displayVariants(Product current);

/*
 * Search for products by name (case-insensitive partial match).
 * Returns 0 on success, -1 on error.
 */
int searchByName(char *query, Product results[], int *count);

/*
 * Show top 3 products in same category with higher green score.
 */
void recommendAlternatives(Product current);

#endif // PRODUCT_H
