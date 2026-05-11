/* history.h - history management interface */

#ifndef HISTORY_H
#define HISTORY_H

#include "structs.h"

/*
 * Save a search history entry for a user.
 * Format in history.txt: username|barcode|productName|timestamp
 */
int saveHistory(char *username, Product p);

/*
 * Display search history for a specific user.
 * Returns 0 on success, -1 on failure.
 */
int viewHistory(char *username);

/*
 * Update search count for a product in searchcount.txt.
 * Increments count if exists, creates entry if new.
 */
int updateSearchCount(Product p);

/*
 * Display the most searched products (top 10).
 * Returns 0 on success, -1 on failure.
 */
int viewMostSearched(void);

#endif // HISTORY_H
