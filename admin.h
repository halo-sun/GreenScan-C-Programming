/* admin.h - admin features interface */

#ifndef ADMIN_H
#define ADMIN_H

#include "structs.h"

/*
 * Add a new product to products.txt.
 * Green score is calculated automatically.
 * Returns 0 on success, -1 on failure.
 */
int adminAddProduct(void);

/*
 * Edit an existing product by barcode.
 * Green score is recalculated automatically.
 * Returns 0 on success, -1 on failure.
 */
int adminEditProduct(void);

/*
 * Delete a product by barcode.
 * Returns 0 on success, -1 if not found.
 */
int adminDeleteProduct(void);

/*
 * Display all products from products.txt with green scores.
 * Returns 0 on success, -1 on failure.
 */
int adminViewAllProducts(void);

/*
 * Display all users from users.txt.
 * Returns 0 on success, -1 on failure.
 */
int adminViewUsers(void);

#endif // ADMIN_H
