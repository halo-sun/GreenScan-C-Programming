/* greenscore.h - green score calculation interface */

#ifndef GREENSCORE_H
#define GREENSCORE_H

#include "structs.h"

/*
 * Calculate green score (0-100) for a product based on:
 * - Category base score
 * - Ingredient penalties and bonuses
 * - Packaging modifier
 */
int calculateGreenScore(Product p);

#endif // GREENSCORE_H
